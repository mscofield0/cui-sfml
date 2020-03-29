#ifndef CUI_SCENES_PARSER_HPP
#define CUI_SCENES_PARSER_HPP

#include <scenes/detail/scene_parsing/count_amount_of_blocks.hpp>
#include <scenes/detail/scene_parsing/count_tabs.hpp>
#include <scenes/detail/scene_parsing/count_max_depth.hpp>
#include <scenes/detail/scene_parsing/valid_block_name.hpp>
#include <scenes/detail/scene_parsing/find_parent.hpp>
#include <scenes/scene.hpp>
#include <parse_utils/stream.hpp>
#include <parse_utils/stream_utils.hpp>
#include <compile_time/string_view.hpp>
#include <global/constants.hpp>

#include <compile_time/variant.hpp>
#include <compile_time/stack.hpp>

#ifdef CUI_ENABLE_CONSTEXPR_PARSING

#include <compile_time/format.hpp>

namespace cui::scenes {

namespace {

using namespace cui::ct;

template <std::size_t AOB>
constexpr auto return_value(const Scene<AOB>& result) {
	return result;
}

template <uint64_t StrSize, typename... Args>
constexpr auto return_value(const char (&s)[StrSize], Args&&... args) {
	return create_formatted_string_with_max_size<512>(s, std::move(args)...);
}

}	 // namespace

template <const char* str>
constexpr auto parse() -> cui::Variant<Scene<detail__::count_amount_of_blocks(StringView{str})>, Format<512>> {
	CharStream stream{str};
	constexpr StringView delims{"\"\n"};
	constexpr auto amount_of_blocks = detail__::count_amount_of_blocks(StringView{str});

	constexpr const char invalid_block_name[] =
	  "Invalid block name (must consist of alphanumeric character [a-zA-Z0-9] starting with a letter)";
	constexpr const char invalid_num_of_tabs[] = "Invalid number of tabs";
	constexpr const char empty_child_stack[] = "The child stack was left empty";
	constexpr const char invalid_styles_list_definition[] = "Invalid styles list definition";
	constexpr const char invalid_styles_list[] = "Incorrect style list";

	Scene<amount_of_blocks> result{};
	Stack<std::size_t, 512> block_stack{};
	std::size_t current_depth = 0;
	std::size_t amount_of_zero_depth = 0;

	while (stream.good()) {
		auto line = consume_line(stream);
		auto trimmed = trim_sv(line);
		if (trimmed.empty()) continue;
		auto num_of_tabs = detail__::count_tabs(line);
		auto idx_of_found = trimmed.find('\"');

		result.blocks().emplace_back();
		result.children().emplace_back();
		result.depths().emplace_back();

		if (idx_of_found == StringView::npos) {
			if (!detail__::is_valid_block_name(trimmed)) {
				return return_value("ERROR at LINE:{}, COL:{} :: {}",
									uint64_t{stream.line()},
									uint64_t{stream.col()},
									invalid_block_name);
			}

			result.blocks().back().name() = trimmed;

			if (num_of_tabs != 0) {
				if (current_depth < num_of_tabs - 1) {
					return return_value("ERROR at LINE:{}, COL:{} :: {}",
										uint64_t{stream.line()},
										uint64_t{stream.col()},
										invalid_num_of_tabs);
				}

				result.depths().back() = num_of_tabs;

				const auto parent_idx = detail__::find_parent(block_stack, result.depths(), result.index_of_last());

				if (parent_idx == result.depths().size()) {
					return return_value("ERROR at LINE:{}, COL:{} :: {}",
										uint64_t{stream.line()},
										uint64_t{stream.col()},
										empty_child_stack);
				}

				result.add_child_to(parent_idx, result.index_of_last());

				block_stack.push(result.index_of_last());

				current_depth = num_of_tabs;
			} else {
				++amount_of_zero_depth;
				block_stack.clear();
				block_stack.push(result.index_of_last());

				current_depth = 0;
			}

		} else {
			auto idx_of_space = trimmed.find(' ');
			auto block_name = trim_sv(trimmed.substr(0, idx_of_space));

			if (!detail__::is_valid_block_name(block_name)) {
				return return_value("ERROR at LINE:{}, COL:{} :: {}",
									uint64_t{stream.line()},
									uint64_t{stream.col()},
									invalid_block_name);
			}

			result.blocks().back().name() = block_name;

			if (idx_of_found == StringView::npos) {
				return return_value("ERROR at LINE:{}, COL:{} :: {}",
									uint64_t{stream.line()},
									uint64_t{stream.col()},
									invalid_styles_list_definition);
			}

			auto idx_of_end_mark = trimmed.find('\"', idx_of_found + 1);

			if (idx_of_end_mark == StringView::npos) {
				return return_value("ERROR at LINE:{}, COL:{} :: {}",
									uint64_t{stream.line()},
									uint64_t{stream.col()},
									invalid_styles_list_definition);
			}

			if (!result.blocks().back().parse_style_list(
				  trimmed.substr(idx_of_found + 1, idx_of_end_mark - idx_of_found))) {
				return return_value("ERROR at LINE:{}, COL:{} :: {}",
									uint64_t{stream.line()},
									uint64_t{stream.col()},
									invalid_styles_list);
			}

			if (num_of_tabs != 0) {
				if (current_depth < num_of_tabs - 1) {
					return return_value("ERROR at LINE:{}, COL:{} :: {}",
										uint64_t{stream.line()},
										uint64_t{stream.col()},
										invalid_num_of_tabs);
				}

				result.depths().back() = num_of_tabs;

				const auto parent_idx = detail__::find_parent(block_stack, result.depths(), result.index_of_last());

				if (parent_idx == amount_of_blocks) {
					return return_value("ERROR at LINE:{}, COL:{} :: {}",
										uint64_t{stream.line()},
										uint64_t{stream.col()},
										empty_child_stack);
				}

				result.add_child_to(parent_idx, result.index_of_last());

				block_stack.push(result.index_of_last());

				current_depth = num_of_tabs;
			} else {
				++amount_of_zero_depth;
				block_stack.clear();
				block_stack.push(result.index_of_last());
				current_depth = 0;
			}
		}
	}

	return return_value(result);
}

}	 // namespace cui::scenes

#else	 // Disable constexpr scenes parser

#include <global/format.hpp>

namespace cui::scenes {

namespace {

auto return_value(const Scene& result) {
	return result;
}

template <uint64_t StrSize, typename... Args>
auto return_value(const char (&s)[StrSize], Args&&... args) {
	return create_formatted_string(s, std::move(args)...);
}

}	 // namespace

auto parse(const StringView str) -> cui::Variant<Scene, Format> {
	CharStream stream{str};
	constexpr StringView delims{"\"\n"};
	auto amount_of_blocks = detail__::count_amount_of_blocks(str);

	constexpr const char invalid_block_name[] =
	  "Invalid block name (must consist of alphanumeric character [a-zA-Z0-9] starting with a letter)";
	constexpr const char invalid_num_of_tabs[] = "Invalid number of tabs";
	constexpr const char empty_child_stack[] = "The child stack was left empty";
	constexpr const char invalid_styles_list_definition[] = "Invalid styles list definition";
	constexpr const char invalid_styles_list[] = "Incorrect style list";

	Scene result{amount_of_blocks};
	cui::Stack<std::size_t, 512> block_stack;
	std::size_t current_depth = 0;
	std::size_t amount_of_zero_depth = 0;
	std::size_t current_index = 0;

	while (stream.good()) {
		auto line = consume_line(stream);
		auto trimmed = trim_sv(line);
		if (trimmed.empty()) continue;
		auto num_of_tabs = detail__::count_tabs(line);
		auto idx_of_found = trimmed.find('\"');

		if (idx_of_found == StringView::npos) {
			if (!detail__::is_valid_block_name(trimmed)) {
				return return_value("ERROR at LINE:{}, COL:{} :: {}",
									uint64_t{stream.line()},
									uint64_t{stream.col()},
									invalid_block_name);
			}

			result.blocks()[current_index].name() = trimmed;

			if (num_of_tabs != 0) {
				if (current_depth < num_of_tabs - 1) {
					return return_value("ERROR at LINE:{}, COL:{} :: {}",
										uint64_t{stream.line()},
										uint64_t{stream.col()},
										invalid_num_of_tabs);
				}

				result.depths()[current_index] = num_of_tabs;

				const auto parent_idx = detail__::find_parent(block_stack, result.depths(), current_index);

				if (parent_idx == result.depths().size()) {
					return return_value("ERROR at LINE:{}, COL:{} :: {}",
										uint64_t{stream.line()},
										uint64_t{stream.col()},
										empty_child_stack);
				}

				result.add_child_to(parent_idx, current_index);

				block_stack.push(current_index);

				current_depth = num_of_tabs;
			} else {
				++amount_of_zero_depth;
				block_stack.clear();
				block_stack.push(current_index);

				current_depth = 0;
			}

			++current_index;

		} else {
			auto idx_of_space = trimmed.find(' ');
			auto block_name = trim_sv(trimmed.substr(0, idx_of_space));

			if (!detail__::is_valid_block_name(block_name)) {
				return return_value("ERROR at LINE:{}, COL:{} :: {}",
									uint64_t{stream.line()},
									uint64_t{stream.col()},
									invalid_block_name);
			}

			result.blocks()[current_index].name() = block_name;

			if (idx_of_found == StringView::npos) {
				return return_value("ERROR at LINE:{}, COL:{} :: {}",
									uint64_t{stream.line()},
									uint64_t{stream.col()},
									invalid_styles_list_definition);
			}

			auto idx_of_end_mark = trimmed.find('\"', idx_of_found + 1);

			if (idx_of_end_mark == StringView::npos) {
				return return_value("ERROR at LINE:{}, COL:{} :: {}",
									uint64_t{stream.line()},
									uint64_t{stream.col()},
									invalid_styles_list);
			}

			if (!result.blocks()[current_index].parse_style_list(
				  trimmed.substr(idx_of_found + 1, idx_of_end_mark - idx_of_found))) {
				return return_value("ERROR at LINE:{}, COL:{} :: {}",
									uint64_t{stream.line()},
									uint64_t{stream.col()},
									invalid_num_of_tabs);
			}

			if (num_of_tabs != 0) {
				if (current_depth < num_of_tabs - 1) {
					return return_value("ERROR at LINE:{}, COL:{} :: {}",
										uint64_t{stream.line()},
										uint64_t{stream.col()},
										empty_child_stack);
				}

				result.depths()[current_index] = num_of_tabs;

				const auto parent_idx = detail__::find_parent(block_stack, result.depths(), current_index);

				if (parent_idx == amount_of_blocks) {
					return return_value("ERROR at LINE:{}, COL:{} :: {}",
										uint64_t{stream.line()},
										uint64_t{stream.col()},
										empty_child_stack);
				}

				result.add_child_to(parent_idx, current_index);

				block_stack.push(current_index);

				current_depth = num_of_tabs;
			} else {
				++amount_of_zero_depth;
				block_stack.clear();
				block_stack.push(current_index);
				current_depth = 0;
			}
			++current_index;
		}
	}

	return return_value(result);
}

}	 // namespace cui::scenes

#endif

#endif	  // CUI_SCENES_PARSER_HPP
#ifndef CUI_CT_SCENES_PARSE_SCENES_HPP
#define CUI_CT_SCENES_PARSE_SCENES_HPP

#include <compile_time/scenes/detail/count_amount_of_blocks.hpp>
#include <compile_time/scenes/detail/count_tabs.hpp>
#include <compile_time/scenes/detail/count_max_depth.hpp>
#include <compile_time/scenes/detail/valid_block_name.hpp>
#include <compile_time/scenes/detail/find_parent.hpp>
#include <compile_time/string/string_view.hpp>
#include <compile_time/stream/stream.hpp>
#include <compile_time/stream/stream_utils.hpp>
#include <compile_time/scene.hpp>
#include <compile_time/variant/variant.hpp>
#include <containers/static_stack.hpp>
#include <compile_time/format/fmt_string_view.hpp>
#include <compile_time/format/format.hpp>
#include <compile_time/format/fmt.hpp>
#include <utils/min.hpp>
#include <utils/distance.hpp>
#include <aliases.hpp>

#define CUI_CT_RETURN_SCENES_PARSE_ERROR(msg) \
	return fmt_s<256>("ERROR at LINE:{}, COL:{} :: {}", stream.line(), stream.col(), msg)

#define CUI_CT_RETURN_SCENES_PARSE_ERROR_WITH_INFO(msg, info) \
	return fmt_s<256>("ERROR at LINE:{}, COL:{} :: {}; Found: |{}|", stream.line(), stream.col(), msg, info)

namespace cui::ct::scenes {

template <const char* str>
constexpr auto parse_scenes() -> Variant<Scene<detail::count_amount_of_blocks(StringView{str})>, Format<256>> {
	CharStream stream(str);
	constexpr StringView delims{"\"\n"};
	constexpr auto amount_of_blocks = detail::count_amount_of_blocks(StringView{str});

	constexpr const char invalid_block_name[] =
	  "Invalid block name (must consist of alphanumeric character [a-zA-Z0-9] starting with a letter)";
	constexpr const char invalid_num_of_tabs[] = "Invalid number of tabs";
	constexpr const char empty_child_stack[] = "The child stack was left empty";
	constexpr const char invalid_styles_list[] = "Incorrect style list";
	constexpr const char invalid_text_syntax[] = "Invalid text data syntax; ' > ' found before ' < '";
	constexpr const char invalid_style_list_syntax[] = "Invalid style list syntax; ' < ' found before ' \" '";
	constexpr const char expected_end_of_text_data[] = "Expected end of text data; Did not find ' > ' at the end";
	constexpr const char expected_end_of_style_list[] = "Expected end of style list; Did not find ' \" ' at the end";

	Scene<amount_of_blocks> result;
	StaticStack<u64, 64> block_stack;
	u64 current_depth = 0;
	u64 amount_of_zero_depth = 0;

	while (stream.good()) {
		const auto line = consume_line(stream);
		const auto trimmed = line.trim();
		if (trimmed.empty()) continue;
		const auto num_of_tabs = detail::count_tabs(line);
		const auto idx_of_found = trimmed.find_first_of('\"');
		const auto idx_of_text = trimmed.find_first_of('<');
		const bool no_style_list = idx_of_found > idx_of_text ? true : false;
		const auto end_idx_of_text = trimmed.find_first_of('>');
		if (end_idx_of_text < idx_of_text) {
			CUI_CT_RETURN_SCENES_PARSE_ERROR_WITH_INFO(invalid_text_syntax, trimmed);
		}

		result.blocks().emplace_back();
		result.children().emplace_back();
		result.depths().emplace_back();

		// extract name
		const auto first_token = min(idx_of_found, idx_of_text);
		const auto name = trimmed.substr(0, first_token).trim();

		if (!detail::is_valid_block_name(name)) {
			CUI_CT_RETURN_SCENES_PARSE_ERROR_WITH_INFO(invalid_block_name, name);
		}

		result.blocks().back().name() = name;

		// No need to parse a style list if text data is first
		if (!no_style_list) {
			// Check if the last symbol is " if there's no text data
			if (idx_of_text == StringView::npos) {
				if (trimmed[trimmed.size() - 1] != '"') {
					CUI_CT_RETURN_SCENES_PARSE_ERROR_WITH_INFO(expected_end_of_style_list, trimmed);
				}
			}

			// extract style list

			// This if might be unnecessary, check once everything is working [NOTICE]
			if (idx_of_found != StringView::npos) {
				const auto second_token = trimmed.find_first_of('"', idx_of_found + 1);
				if (second_token >= idx_of_text) {
					CUI_CT_RETURN_SCENES_PARSE_ERROR_WITH_INFO(invalid_style_list_syntax, trimmed);
				}
				const auto style_list = trimmed.substr(first_token + 1, second_token - first_token);

				if (!result.blocks().back().parse_style_list(style_list)) {
					CUI_CT_RETURN_SCENES_PARSE_ERROR_WITH_INFO(invalid_styles_list, style_list);
				}
			}
		}

		// Nothing to extract if no such symbol was found
		if (idx_of_text != StringView::npos) {
			// extract text data

			// Check last symbol
			if (trimmed[trimmed.size() - 1] != '>') {
				CUI_CT_RETURN_SCENES_PARSE_ERROR_WITH_INFO(expected_end_of_text_data, trimmed);
			}

			const auto text_data_size = distance(trimmed.begin() + idx_of_text, trimmed.end() - 2);

			result.blocks().back().text() = trimmed.substr(idx_of_text + 1, text_data_size);
		}

		if (num_of_tabs != 0) {
			if (current_depth < num_of_tabs - 1) {
				CUI_CT_RETURN_SCENES_PARSE_ERROR_WITH_INFO(invalid_num_of_tabs, num_of_tabs);
			}

			result.depths().back() = num_of_tabs;
			const auto parent_idx = detail::find_parent(block_stack, result.depths(), result.index_of_last());

			if (parent_idx == result.depths().size()) {
				CUI_CT_RETURN_SCENES_PARSE_ERROR(empty_child_stack);
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

	return result;
}

}	 // namespace cui::ct::scenes

#endif	  // CUI_CT_SCENES_PARSE_SCENES_HPP
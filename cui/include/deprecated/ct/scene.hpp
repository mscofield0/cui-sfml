#ifndef CUI_SCENE_HPP
#define CUI_SCENE_HPP

#include "stack.hpp"
#include "static_string_stream.hpp"
#include "string.hpp"
#include "variant.hpp"
#include <array>
#include <utility>

namespace cui::ct {

struct block
{
	cui::ct::string block_name;
	std::array<cui::ct::string, 64> style_list;
	uint8_t idx;

	constexpr block() : block_name(""), style_list{}, idx(0) {}
	constexpr block(cui::ct::string block_name) : block_name(block_name), style_list{}, idx(0) {}

	constexpr void add_style(cui::ct::string name) {
		style_list[idx] = (64 > idx ? name : throw std::out_of_range(""));
		++idx;
	}

	constexpr bool parse_style_list(cui::ct::string const& str) {
		if (str.size == 0) return false;
		uint32_t prev = 0;
		uint32_t pos = cui::ct::find(str, ',', prev);
		while (pos != str.size) {
			if (pos == prev) return false;
			auto t = cui::ct::trim(cui::ct::string{str.str + prev, pos - prev});
			if (t.size == 0) return false;
			add_style(t);
			prev = pos + 1;
			pos = cui::ct::find(str, ',', prev);
		}
		if (prev == pos) return false;
		auto t = cui::ct::trim(cui::ct::string{str.str + prev, pos - prev - 1});
		if (t.size == 0) return false;
		add_style(t);
		return true;
	}
};

template <uint32_t AmountOfBlocks>
struct scene_result
{
	static constexpr uint32_t amount_of_blocks = AmountOfBlocks;
	std::array<block, amount_of_blocks> blocks;
	std::array<std::array<int, amount_of_blocks>, amount_of_blocks> children;
	std::array<uint32_t, amount_of_blocks> depths;
	std::array<uint32_t, amount_of_blocks> child_idx;

	constexpr scene_result() : blocks{}, children{}, child_idx{}, depths{} {}

	constexpr void add_child_to(uint32_t to, uint32_t child) {
		if (child_idx[to] == -1) child_idx[to]++;
		children[to][child_idx[to]++] = child;
	}
};

constexpr uint32_t count_tabs(cui::ct::string const& str) {
	uint32_t count = 0;
	while (count < str.size) {
		if (str[count] != '\t') break;
		++count;
	}
	return count;
}

constexpr bool is_valid_block_name(cui::ct::string const& str) {
	constexpr cui::ct::string valid_chars{"abcdefghijklmnopqrstuvwxyz0123456789_"};
	if (str[0] >= '0' && str[0] <= '9') return false;
	uint32_t i = 0;
	while (i < str.size) {
		bool b = false;
		for (uint32_t j = 0; j < valid_chars.size; ++j) {
			if (str[i] == valid_chars[j]) {
				b = true;
				break;
			}
		}
		if (!b) return false;
		++i;
	}
	return true;
}

constexpr uint32_t count_amount_of_blocks(const char* str) {
	auto stream = cui::ct::static_string_stream{str};
	uint32_t amount = 0;
	while (stream.good()) {
		auto line = cui::ct::consume_line(stream);
		auto trimmed = cui::ct::trim(line);
		if (trimmed.size == 0) continue;
		++amount;
	}
	return amount;
}

constexpr uint32_t count_first_layer(const char* str) {
	auto stream = cui::ct::static_string_stream{str};
	uint32_t amount = 0;
	while (stream.good()) {
		auto line = cui::ct::consume_line(stream);
		auto trimmed = cui::ct::trim(line);
		if (trimmed.size == 0) continue;
		if (count_tabs(line) == 0) ++amount;
	}
	return amount;
}

constexpr uint32_t count_max_depth(const char* str) {
	auto stream = cui::ct::static_string_stream{str};
	uint32_t max = 0;
	while (stream.good()) {
		auto line = cui::ct::consume_line(stream);
		auto trimmed = cui::ct::trim(line);
		if (trimmed.size == 0) continue;
		auto t = count_tabs(line);
		if (t > max) max = t;
	}
	return max;
}

template <uint32_t N, uint32_t AOB>
constexpr uint32_t
find_parent(cui::ct::stack<uint32_t, N>& child_stack, std::array<uint32_t, AOB>& depths, uint32_t current_block_idx) {
	while (child_stack.idx != 0) {
		if (depths[child_stack.top()] == depths[current_block_idx] - 1) {
			return child_stack.top();
		} else {
			child_stack.pop();
		}
	}
	return AOB;
}

template <const char* str>
constexpr cui::ct::variant<scene_result<count_amount_of_blocks(str)>, error_report> parse_scene() {
	auto stream = cui::ct::static_string_stream{str};
	constexpr cui::ct::string delims{"\"\n"};
	constexpr auto amount_of_blocks = count_amount_of_blocks(str);
	constexpr auto depth = count_max_depth(str) + 1;

	// Depth denominates the previous depth
	// Amount denominates the current count of 0 depth elements
	// Idx denominates the current index of the block
	int depth_ = 0, amount_ = 0, idx = 0;
	// The final result
	scene_result<amount_of_blocks> result{};
	// Hierarchy block stack
	cui::ct::stack<uint32_t, 512> block_stack{};

	for (auto& el : result.children) cui::util::fill_array(el, -1);

	while (stream.good()) {
		auto line_ = cui::ct::consume_line(stream);
		auto trimmed = cui::ct::trim(line_);
		if (trimmed.size == 0) continue;
		auto num_of_tabs = count_tabs(line_);
		auto idx_of_found = cui::ct::find(trimmed, '\"');

		// If there is a potential styles list
		if (idx_of_found == trimmed.size) {
			// Check if the block name is valid
			if (!is_valid_block_name(trimmed)) {
				auto line = cui::ct::count_until_delim(cui::ct::string{str, stream.current + 1}, 0, '\n');
				auto last_idx = cui::ct::find_last(cui::ct::string{str, stream.current + 1}, '\n');
				auto col = stream.current - last_idx;
				error_report err{"ERROR: Invalid block name.", line, col};

				return err;
			}

			// Assign the next block's name to the parsed block name
			result.blocks[idx].block_name = trimmed;

			// If there are tabs, that means that there is a new child to a
			// parent
			if (num_of_tabs != 0) {
				// If the previous depth is larger than the current line depth +
				// 1 abcdef "abc"          --> Previous depth = 0
				//         hgi "haha"    --> Current line depth = 2
				// abcdef "abcdef"       --> 0
				//     ghji "abc"        --> 1
				//         kli "pop"     --> 2
				//             pop "lol" --> 3
				//     abcd "pop"        --> 1
				if (depth_ < num_of_tabs - 1) {
					auto line = cui::ct::count_until_delim(cui::ct::string{str, stream.current + 1}, 0, '\n');
					auto last_idx = cui::ct::find_last(cui::ct::string{str, stream.current + 1}, '\n');
					auto col = stream.current - last_idx;
					error_report err{"ERROR: Invalid number of tabs.", line, col};

					return err;
				}
				// Store the current blocks' depth
				result.depths[idx] = num_of_tabs;

				// Find the current blocks' parent index
				auto const parent_idx = find_parent(block_stack, result.depths, idx);
				// If the found index is the same as the amount of blocks, then
				// something has gone terribly wrong
				if (parent_idx == amount_of_blocks) {
					auto line = cui::ct::count_until_delim(cui::ct::string{str, stream.current + 1}, 0, '\n');
					auto last_idx = cui::ct::find_last(cui::ct::string{str, stream.current + 1}, '\n');
					auto col = stream.current - last_idx;
					error_report err{"ERROR: The child stack was left empty. "
									 "(should never happen)",
									 line,
									 col};

					return err;
				}

				result.add_child_to(parent_idx, idx);

				// If the expression is false, then the block stack is exceeded
				// and cannot proceed anymore.
				if (!block_stack.push(idx)) {
					auto line = cui::ct::count_until_delim(cui::ct::string{str, stream.current + 1}, 0, '\n');
					auto last_idx = cui::ct::find_last(cui::ct::string{str, stream.current + 1}, '\n');
					auto col = stream.current - last_idx;
					error_report err{"ERROR: The block stack was exceeded.", line, col};

					return err;
				}

				// Set the previous depth to the current one
				depth_ = num_of_tabs;

			}
			// If there are no tabs, that means there is a new base block
			else
			{
				// Increment amount_
				++amount_;
				// Clear the block stack and push the current index
				block_stack.clear();
				block_stack.push(idx);
				// Reset the depth level
				depth_ = 0;
			}
			// Increment the block index
			++idx;
		} else {
			auto idx_of_space = cui::ct::find(trimmed, ' ');
			auto block_name = cui::ct::trim(cui::ct::string{trimmed.str, idx_of_space});

			// Check if the block name is valid
			if (!is_valid_block_name(block_name)) {
				auto line = cui::ct::count_until_delim(cui::ct::string{str, stream.current + 1}, 0, '\n');
				auto last_idx = cui::ct::find_last(cui::ct::string{str, stream.current + 1}, '\n');
				auto col = stream.current - last_idx;
				error_report err{"ERROR: Invalid block name.", line, col};

				return err;
			}

			// Assign the next block's name to the parsed block name
			result.blocks[idx].block_name = block_name;
			// Uh
			if (idx_of_found + 1 >= trimmed.size) {
				auto line = cui::ct::count_until_delim(cui::ct::string{str, stream.current + 1}, 0, '\n');
				auto last_idx = cui::ct::find_last(cui::ct::string{str, stream.current + 1}, '\n');
				auto col = stream.current - last_idx;
				error_report err{"ERROR: Invalid styles list definition.", line, col};

				return err;
			}

			// Find the end quotation marks
			auto idx_of_end_mark = cui::ct::find(trimmed, '\"', idx_of_found + 1);
			// If no quotation marks were found
			if (idx_of_end_mark == trimmed.size) {
				auto line = cui::ct::count_until_delim(cui::ct::string{str, stream.current + 1}, 0, '\n');
				auto last_idx = cui::ct::find_last(cui::ct::string{str, stream.current + 1}, '\n');
				auto col = stream.current - last_idx;
				error_report err{"ERROR: Expected end of styles list.", line, col};

				return err;
			}

			// If there was an error in parsing the style list
			if (!result.blocks[idx].parse_style_list(
				  cui::ct::string{trimmed.str + idx_of_found + 1, idx_of_end_mark - idx_of_found}))
			{
				auto line = cui::ct::count_until_delim(cui::ct::string{str, stream.current + 1}, 0, '\n');
				auto last_idx = cui::ct::find_last(cui::ct::string{str, stream.current + 1}, '\n');
				auto col = stream.current - last_idx;
				error_report err{"ERROR: Incorrect style list.", line, col};

				return err;
			}

			// If there are tabs, that means that there is a new child to a
			// parent
			if (num_of_tabs != 0) {
				// If the previous depth is larger than the current line depth +
				// 1 abcdef "abc"          --> Previous depth = 0
				//         hgi "haha"    --> Current line depth = 2
				// abcdef "abcdef"       --> 0
				//     ghji "abc"        --> 1
				//         kli "pop"     --> 2
				//             pop "lol" --> 3
				//     abcd "pop"        --> 1
				if (depth_ < num_of_tabs - 1) {
					auto line = cui::ct::count_until_delim(cui::ct::string{str, stream.current + 1}, 0, '\n');
					auto last_idx = cui::ct::find_last(cui::ct::string{str, stream.current + 1}, '\n');
					auto col = stream.current - last_idx;
					error_report err{"ERROR: Invalid number of tabs.", line, col};

					return err;
				}

				// Store the current blocks' depth
				result.depths[idx] = num_of_tabs;

				// Find the current blocks' parent index
				auto const parent_idx = find_parent(block_stack, result.depths, idx);
				// If the found index is the same as the amount of blocks, then
				// something has gone terribly wrong
				if (parent_idx == amount_of_blocks) {
					auto line = cui::ct::count_until_delim(cui::ct::string{str, stream.current + 1}, 0, '\n');
					auto last_idx = cui::ct::find_last(cui::ct::string{str, stream.current + 1}, '\n');
					auto col = stream.current - last_idx;
					error_report err{"ERROR: The child stack was left empty. "
									 "(should never happen)",
									 line,
									 col};

					return err;
				}

				result.add_child_to(parent_idx, idx);

				// If the expression is false, then the block stack is exceeded
				// and cannot proceed anymore.
				if (!block_stack.push(idx)) {
					auto line = cui::ct::count_until_delim(cui::ct::string{str, stream.current + 1}, 0, '\n');
					auto last_idx = cui::ct::find_last(cui::ct::string{str, stream.current + 1}, '\n');
					auto col = stream.current - last_idx;
					error_report err{"ERROR: The block stack was exceeded.", line, col};

					return err;
				}

				// Set the previous depth to the current one
				depth_ = num_of_tabs;
			}
			// If there are no tabs, that means there is a new base block
			else
			{
				// Increment amount_
				++amount_;
				// Clear the block stack and push the current index
				block_stack.clear();
				block_stack.push(idx);
				// Reset the depth level
				depth_ = 0;
			}
			++idx;
		}
	}
	return result;
}

}	 // namespace cui::ct

#endif	  // CUI_SCENE_HPP

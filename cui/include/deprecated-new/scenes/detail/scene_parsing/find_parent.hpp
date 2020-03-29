#ifndef CUI_SCENES_DETAIL_FIND_PARENT_HPP
#define CUI_SCENES_DETAIL_FIND_PARENT_HPP

#include <compile_time/stack.hpp>
#include <global/constants.hpp>

namespace cui::scenes::detail__ {

template <std::size_t StackSize, typename Container>
constexpr std::size_t
find_parent(Stack<std::size_t, StackSize>& child_stack, Container& depths, std::size_t current_block_idx) {
	while (!child_stack.empty()) {
		if (depths[child_stack.top()] == depths[current_block_idx] - 1) {
			return child_stack.top();
		} else {
			child_stack.pop();
		}
	}
	return depths.size();
}

}	 // namespace cui::scenes::detail__

#endif	  // CUI_SCENES_DETAIL_FIND_PARENT_HPP
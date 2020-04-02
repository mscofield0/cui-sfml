#ifndef CUI_CT_SCENES_FIND_PARENT_HPP
#define CUI_CT_SCENES_FIND_PARENT_HPP

#include <containers/static_stack.hpp>
#include <aliases.hpp>

namespace cui::ct::scenes::detail {

template <u64 StackSize, typename Container>
constexpr auto find_parent(StaticStack<u64, StackSize>& child_stack, Container& depths, u64 current_block_idx) -> u64 {
	while (!child_stack.empty()) {
		if (depths[child_stack.top()] == depths[current_block_idx] - 1) {
			return child_stack.top();
		} else {
			child_stack.pop();
		}
	}
	return depths.size();
}

}	 // namespace cui::ct::scenes::detail

#endif	  // CUI_CT_SCENES_FIND_PARENT_HPP
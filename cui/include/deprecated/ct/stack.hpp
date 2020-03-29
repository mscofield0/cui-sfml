#ifndef CUI_STACK_HPP
#define CUI_STACK_HPP

#include "../utility/utility.hpp"
#include <array>
#include <exception>

namespace cui::ct {

template <typename T, uint32_t N>
struct stack
{
	std::array<T, N> data;
	uint32_t idx;

	constexpr stack() : data{}, idx(0) {}

	constexpr bool push(T item) noexcept {
		if (idx >= N) return false;
		data[idx] = item;
		++idx;
		return true;
	}

	constexpr T top() noexcept {
		return data[idx - 1];
	}

	constexpr T pop() {
		if (idx == 0) throw std::out_of_range{""};
		return data[idx--];
	}

	constexpr void remove_top(uint32_t N_) {
		if (idx > N_) throw std::out_of_range{""};
		idx -= N_;
	}

	constexpr void clear() {
		idx = 0;
	}
};

}	 // namespace cui::ct

#endif	  // CUI_STACK_HPP
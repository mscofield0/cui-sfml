#ifndef CUI_GLOBAL_MAKE_ARRAY_HPP
#define CUI_GLOBAL_MAKE_ARRAY_HPP


#include <array>

namespace cui {

template <typename T, typename... Args>
constexpr std::array<T, sizeof...(Args)> make_array(Args... args) {
	std::array<T, sizeof...(Args)> arr{args...};
	return arr;
}

template <typename T, typename... Args>
constexpr std::array<T, sizeof...(Args) + 1> make_array(T arg, Args... args) {
	std::array<T, sizeof...(Args) + 1> arr{arg, args...};
	return arr;
}

}

#endif // CUI_GLOBAL_MAKE_ARRAY_HPP
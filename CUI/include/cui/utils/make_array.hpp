#ifndef CUI_UTILS_MAKE_ARRAY_HPP
#define CUI_UTILS_MAKE_ARRAY_HPP

#include <array>

namespace cui {

template <typename T, typename... Args>
constexpr auto make_array(const Args&... args) -> std::array<T, sizeof...(Args)> {
	return {args...};
}

template <typename T, typename... Args>
constexpr auto make_array(const T& arg, const Args&... args) -> std::array<T, sizeof...(Args) + 1> {
	return {arg, args...};
}

}	 // namespace cui

#endif	  // CUI_GLOBAL_MAKE_ARRAY_HPP
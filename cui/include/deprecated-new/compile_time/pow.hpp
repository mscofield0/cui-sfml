#ifndef CUI_COMPILE_TIME_POW_HPP
#define CUI_COMPILE_TIME_POW_HPP

#include <type_traits>
#include <global/constants.hpp>

namespace cui {

template <typename T, std::enable_if_t<std::is_same_v<T, float> || std::is_integral_v<T>>* = nullptr>
constexpr float pow(T v, int pot) {
	if (pot == 0) return 1.f;
	if (pot == 1) return static_cast<float>(v);
	if (pot < 0) {
		float res = 1.f / v;
		for (int i = 1; i < pot; ++i) {
			res /= v;
		}
		return res;
	}
	float res = static_cast<float>(v);
	for (int i = 1; i < pot; ++i) {
		res *= v;
	}
	return res;
}

}	 // namespace cui

#endif	  // CUI_COMPILE_TIME_POW_HPP
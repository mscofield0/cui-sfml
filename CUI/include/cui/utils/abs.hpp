#ifndef CUI_ABS_HPP
#define CUI_ABS_HPP

namespace cui {

template <typename T>
constexpr T abs(const T& x) noexcept {
	return x < 0 ? -x : x;
}

}	 // namespace cui

#endif	  // CUI_ABS_HPP
#ifndef CUI_MIN_HPP
#define CUI_MIN_HPP

namespace cui {

template <typename T>
constexpr T min(const T& lhs, const T& rhs) noexcept {
	return lhs < rhs ? lhs : rhs;
}

}	 // namespace cui

#endif	  // CUI_MIN_HPP
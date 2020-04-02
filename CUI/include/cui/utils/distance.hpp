#ifndef CUI_DISTANCE_HPP
#define CUI_DISTANCE_HPP

#include <aliases.hpp>

namespace cui {

template <typename Type>
constexpr i64 distance(Type* lhs, Type* rhs) noexcept {
	return rhs - lhs;
}

}	 // namespace cui

#endif	  // CUI_DISTANCE_HPP
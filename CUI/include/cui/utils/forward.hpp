#ifndef CUI_FORWARD_HPP
#define CUI_FORWARD_HPP

#include <utils/detail/remove_reference.hpp>
#include <utils/detail/is_lvalue_reference.hpp>

namespace cui {

template <class Type>
constexpr Type&& forward(typename remove_reference<Type>::type& t) noexcept {
	return static_cast<Type&&>(t);
}

template <class Type>
constexpr Type&& forward(typename remove_reference<Type>::type&& t) noexcept {
	static_assert(!is_lvalue_reference<Type>::value, "Can not forward an rvalue as an lvalue.");
	return static_cast<Type&&>(t);
}

}	 // namespace cui

#endif	  // CUI_FORWARD_HPP
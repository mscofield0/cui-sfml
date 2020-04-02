#ifndef CUI_MOVE_HPP
#define CUI_MOVE_HPP

#include <utils/detail/remove_reference.hpp>

namespace cui {

template <class Type>
inline constexpr typename remove_reference<Type>::type&& move(Type&& arg) {
	return ((typename remove_reference<Type>::type &&) arg);
}

}	 // namespace cui

#endif	  // CUI_MOVE_HPP
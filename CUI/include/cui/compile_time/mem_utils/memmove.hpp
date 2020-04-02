#ifndef CUI_CT_MEMMOVE_HPP
#define CUI_CT_MEMMOVE_HPP

#include <aliases.hpp>

namespace cui {

template <typename T>
constexpr void memmove(T* dest, const T* src, u32 length) {
	u32 i = 0;
	while (i++ < length) *dest++ = *src++;
}

}	 // namespace cui

#endif	  // CUI_CT_MEMMOVE_HPP
#ifndef CUI_CT_MEMSET_HPP
#define CUI_CT_MEMSET_HPP

#include <aliases.hpp>

namespace cui {

template <typename T>
constexpr void memset(T* dest, T val, u32 length) {
	u32 i = 0;
	while (i++ < length) *dest++ = val;
}

}	 // namespace cui

#endif	  // CUI_CT_MEMSET_HPP
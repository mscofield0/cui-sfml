#ifndef CUI_COMPILE_TIME_MEMMOVE_HPP
#define CUI_COMPILE_TIME_MEMMOVE_HPP

#include <global/constants.hpp>

namespace cui {

template <typename T>
constexpr void memmove(T* dest, const T* src, std::size_t length) {
    std::size_t i = 0;
    while(i++ < length) *dest++ = *src++;
}

}

#endif // CUI_COMPILE_TIME_MEMMOVE_HPP
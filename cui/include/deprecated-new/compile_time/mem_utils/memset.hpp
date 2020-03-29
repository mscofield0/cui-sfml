#ifndef CUI_COMPILE_TIME_MEMSET_HPP
#define CUI_COMPILE_TIME_MEMSET_HPP

#include <global/constants.hpp>

namespace cui {

template <typename T>
constexpr void memset(T* dest, T val, std::size_t length) {
    std::size_t i = 0;
    while(i++ < length) *dest++ = val;
}

}

#endif // CUI_COMPILE_TIME_MEMSET_HPP
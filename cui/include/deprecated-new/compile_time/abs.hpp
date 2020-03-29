#ifndef CUI_COMPILE_TIME_ABS_HPP
#define CUI_COMPILE_TIME_ABS_HPP

#include <global/constants.hpp>

namespace cui {

template <typename T>
constexpr T abs(T v) {
    return v < 0 ? -v : v; 
}

}

#endif // CUI_COMPILE_TIME_ABS_HPP
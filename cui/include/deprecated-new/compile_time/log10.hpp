#ifndef CUI_COMPILE_TIME_LOG10_HPP
#define CUI_COMPILE_TIME_LOG10_HPP

#include <global/constants.hpp>

namespace cui {

constexpr uint64_t log10(uint64_t v) {
    return (v >= 10000000000000000000u) ? 19 : (v >= 1000000000000000000u) ? 18 :
        (v >= 100000000000000000u) ? 17 : (v >= 10000000000000000u) ? 16 :
        (v >= 1000000000000000u) ? 15 : (v >= 100000000000000u) ? 14 :
        (v >= 10000000000000u) ? 13 : (v >= 1000000000000u) ? 12 :
        (v >= 100000000000u) ? 11 : (v >= 10000000000u) ? 10 :
        (v >= 1000000000u) ? 9 : (v >= 100000000u) ? 8 : 
        (v >= 10000000u) ? 7 : (v >= 1000000u) ? 6 : 
        (v >= 100000u) ? 5 : (v >= 10000u) ? 4 :
        (v >= 1000u) ? 3 : (v >= 100u) ? 2 : (v >= 10u) ? 1u : 0u; 
}

}

#endif // CUI_COMPILE_TIME_LOG10_HPP
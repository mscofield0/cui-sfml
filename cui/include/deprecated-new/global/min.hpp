#ifndef CUI_GLOBAL_MIN_HPP
#define CUI_GLOBAL_MIN_HPP

namespace cui {

template <typename T>
constexpr T min(T a, T b) {
    return a < b ? a : b;
}

}

#endif // CUI_GLOBAL_MIN_HPP
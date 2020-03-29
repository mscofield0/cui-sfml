#ifndef CUI_COMPILE_TIME_CHAR_TRAITS_HPP
#define CUI_COMPILE_TIME_CHAR_TRAITS_HPP

#include <global/constants.hpp>
#include <compile_time/mem_utils/memmove.hpp>
#include <compile_time/mem_utils/memcpy.hpp>
#include <compile_time/mem_utils/memset.hpp>

namespace cui {

constexpr int EOF_SYMBOL = 79894;

template <typename CharT>
class BasicCharTraits {
private:

public:
    using char_type = CharT;
    using int_type = int;

    static constexpr void assign(char_type& c1, const char_type& c2) {
        c1 = c2;
    }

    static constexpr bool eq(const char_type& c1, const char_type& c2) {
        return c1 == c2;
    }
    
    static constexpr bool lt(const char_type& c1, const char_type& c2) {
        return c1 < c2;
    }

    static constexpr int compare(const char_type* s1, const char_type* s2, std::size_t n) {
        for(std::size_t i = 0; i < n; ++i) {
            if(lt(s1[i], s2[i])) return -1; 
            else if(lt(s2[i], s1[i])) return 1;
        }
        return 0;
    }
    
    static constexpr std::size_t length(const char_type* s) {
        std::size_t i = 0;
        while(!eq(s[i], char_type())) ++i;
        return i;
    }
    
    static constexpr const char_type* find(const char_type* s, std::size_t n, const char_type& ch) {
        for(std::size_t i = 0; i < n; ++i) {
            if(eq(s[i], ch)) return s + i;
        }
        return nullptr;
    }

    static constexpr char_type* move(char_type* dest, const char_type* src, std::size_t length) {
        if(length == 0) return dest;
        memmove(dest, src, length);
        return dest;
    }

    static constexpr char_type* copy(char_type* dest, const char_type* src, std::size_t length) {
        return move(dest, src, length);
    } 

    static constexpr char_type* assign(char_type* dest, std::size_t n, char_type ch) {
        if(length == 0) return dest;
        memset(dest, ch, n);
        return dest;
    }

    static constexpr char_type to_char_type(const int_type& x) {
        return static_cast<char_type>(x);
    }

    static constexpr char_type to_int_type(const char_type& x) {
        return static_cast<int_type>(x);
    }

    static constexpr char_type eq_int_type(const int_type& x) {
        return static_cast<char_type>(x);
    }

    static constexpr int_type eof() {
        return static_cast<int_type>(EOF_SYMBOL);
    }

    static constexpr int_type not_eof(const int_type& x) {
        return !eq_int_type(x, eof()) ? x : to_int_type(char_type());
    }


};

using CharTraits = BasicCharTraits<char>;

}

#endif // CUI_COMPILE_TIME_CHAR_TRAITS_HPP
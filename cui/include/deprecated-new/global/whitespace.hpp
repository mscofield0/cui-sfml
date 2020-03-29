#ifndef CUI_GLOBAL_WHITESPACE_HPP
#define CUI_GLOBAL_WHITESPACE_HPP

namespace cui {

constexpr inline bool is_whitespace(unsigned char ch) {
	return ch == 9 || ch == 10 || ch == 11 || ch == 12 || ch == 13 || ch == 32 || ch == 133 || ch == 160;
}

}	 // namespace cui

#endif	  // CUI_GLOBAL_WHITESPACE_HPP
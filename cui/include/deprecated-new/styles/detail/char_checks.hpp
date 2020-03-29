#ifndef CUI_STYLES_DETAIL_CHAR_CHECKS_HPP
#define CUI_STYLES_DETAIL_CHAR_CHECKS_HPP

#include <global/whitespace.hpp>
#include <compile_time/string_view.hpp>

namespace cui::styles::detail__ {

constexpr bool is_minus(char ch) {
	return ch == '-';
}

constexpr bool is_whitespace(char ch) {
	return cui::is_whitespace(ch);
}

constexpr bool is_digit(char ch) {
	return (ch >= '0' && ch <= '9');
}

constexpr bool is_point(char ch) {
	return (ch == '.' || ch == ',');
}

constexpr bool is_percent(char ch) {
	return ch == '%';
}

constexpr bool is_open_parenthesis(char ch) {
	return ch == '(';
}

constexpr bool is_closed_parenthesis(char ch) {
	return ch == ')';
}

constexpr bool is_colon(char ch) {
	return ch == ',';
}

constexpr bool is_valid_char(char ch) {
	constexpr StringView valid_chars{"abcdefghijklmnopqrstuvwxyz_"};
	for (auto el : valid_chars) {
		if (el == ch) return true;
	}
	return false;
}

}	 // namespace cui::styles::detail__

#endif	  // CUI_STYLES_DETAIL_CHAR_CHECKS_HPP
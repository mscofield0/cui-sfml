#ifndef CUI_COMPILE_TIME_STRING_UTILS_TRIM_SV_HPP
#define CUI_COMPILE_TIME_STRING_UTILS_TRIM_SV_HPP

#include <compile_time/string_view.hpp>
#include <global/whitespace.hpp>
#include <global/constants.hpp>

namespace cui {

constexpr cui::StringView::size_type find_end_of_whitespace(cui::StringView str) {
	auto it = str.begin();
	for (; it != str.end(); ++it) {
		if (!cui::is_whitespace(*it)) {
			break;
		}
	}
	return std::distance(str.begin(), it);
}

constexpr cui::StringView::size_type rfind_end_of_whitespace(cui::StringView str) {
	auto it = str.rbegin();
	for (; it != str.rend(); ++it) {
		if (!cui::is_whitespace(*it)) {
			break;
		}
	}
	return std::distance(str.rbegin(), it);
}

constexpr cui::StringView trim_sv(cui::StringView str) {
	const auto left = find_end_of_whitespace(str);
	const auto right = rfind_end_of_whitespace(str);
	str.remove_prefix(left);
	if(str.empty()) return str;
	str.remove_suffix(right);
	return str;
}

}	 // namespace cui

#endif	  // CUI_COMPILE_TIME_STRING_UTILS_TRIM_SV_HPP
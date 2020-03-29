#ifndef CUI_STYLES_DETAIL_LOOKUP_STRING_HPP
#define CUI_STYLES_DETAIL_LOOKUP_STRING_HPP

#include <styles/data.hpp>
#include <styles/constants.hpp>
#include <compile_time/string_view.hpp>

namespace cui::styles::detail__ {

constexpr auto lookup_string(const StringView str) -> ValueData {
	for (size_t i = 0; i < styles::string_names.size(); ++i) {
		if (styles::string_names[i] == str) {
			return styles::string_values[i];
		}
	}
	return ValueData{};
}

}	 // namespace cui::styles::detail__

#endif	  // CUI_STYLES_DETAIL_LOOKUP_STRING_HPP
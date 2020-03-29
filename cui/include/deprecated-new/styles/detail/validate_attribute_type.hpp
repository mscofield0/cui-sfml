#ifndef CUI_STYLES_DETAIL_VALIDATE_ATTRIBUTE_TYPE_HPP
#define CUI_STYLES_DETAIL_VALIDATE_ATTRIBUTE_TYPE_HPP

#include <styles/constants.hpp>
#include <compile_time/string_view.hpp>

namespace cui::styles::detail__ {

constexpr auto validate_attribute_type(const StringView p_type) -> bool {
	for (const auto type : styles::attribute_names) {
		if (type == p_type) return true;
	}
	return false;
}

}	 // namespace cui::styles::detail__

#endif	  // CUI_STYLES_DETAIL_VALIDATE_ATTRIBUTE_TYPE_HPP
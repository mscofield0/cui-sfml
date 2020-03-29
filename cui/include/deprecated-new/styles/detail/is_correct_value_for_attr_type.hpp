#ifndef CUI_STYLES_DETAIL_IS_CORRECT_VALUE_FOR_ATTR_TYPE_HPP
#define CUI_STYLES_DETAIL_IS_CORRECT_VALUE_FOR_ATTR_TYPE_HPP

#include <styles/constants.hpp>
#include <styles/detail/dt_to_vat.hpp>

namespace cui::styles::detail__ {

constexpr auto is_correct_value_for_attr_type(const StringView type, const ValueData& value) -> bool {
	constexpr auto size = styles::attribute_types.size();
	auto idx = size;

	for (size_t i = 0; i < styles::attribute_names.size(); ++i) {
		if (type == styles::attribute_names[i]) {
			idx = i;
			break;
		}
	}

	if (idx == size) return false;
	auto valid_value_types = styles::attribute_types[idx];

	return valid_value_types & detail__::dt_to_vat(value.active());
}

}	 // namespace cui::styles::detail__

#endif	  // CUI_STYLES_DETAIL_IS_CORRECT_VALUE_FOR_ATTR_TYPE_HPP

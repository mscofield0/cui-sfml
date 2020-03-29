#ifndef CUI_STYLES_DETAIL_GET_ATTRIBUTE_INDEX_HPP
#define CUI_STYLES_DETAIL_GET_ATTRIBUTE_INDEX_HPP

#include <compile_time/string_view.hpp>
#include <styles/constants.hpp>

namespace cui::styles {

enum class AttributeIndexes : uint32_t
{
	Color = 0
};

constexpr auto get_attribute_index(const StringView name) -> uint32_t {
	uint32_t i = 0;
	for (; i < styles::attribute_names.size(); ++i) {
		if (name == styles::attribute_names[i]) {
			break;
		}
	}
	return i;
}

}	 // namespace cui::styles

#endif	  // CUI_STYLES_DETAIL_GET_ATTRIBUTE_INDEX_HPP
#ifndef CUI_CT_STYLES_GET_ATTRIBUTE_INDEX_HPP
#define CUI_CT_STYLES_GET_ATTRIBUTE_INDEX_HPP

#include <compile_time/string/string_view.hpp>
#include <compile_time/styles/detail/constants.hpp>
#include <aliases.hpp>

namespace cui::ct::styles::detail {

enum class AttributeIndexes : u64
{
	Color = 0,
	X,
	Y,
	Width,
	Height
};

constexpr auto get_attribute_index(const StringView name) -> u64 {
	u64 i = 0;
	for (; i < attribute_names.size(); ++i) {
		if (name == attribute_names[i]) {
			break;
		}
	}
	return i;
}

}	 // namespace cui::ct::styles::detail

#endif	  // CUI_CT_STYLES_GET_ATTRIBUTE_INDEX_HPP
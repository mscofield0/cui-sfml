#ifndef CUI_STYLES_DETAIL_INDEX_OF_FUNCTION_NAME_HPP
#define CUI_STYLES_DETAIL_INDEX_OF_FUNCTION_NAME_HPP

#include <styles/constants.hpp>
#include <compile_time/string_view.hpp>

namespace cui::styles::detail__ {

constexpr auto index_of_function_name(const StringView fn_name) -> size_t {
	auto idx = styles::function_names.size();
	for (size_t i = 0; i < styles::function_names.size(); ++i) {
		if (fn_name == styles::function_names[i]) {
			idx = i;
			break;
		}
	}
	return idx;
}

}	 // namespace cui::styles::detail__

#endif	  // CUI_STYLES_DETAIL_INDEX_OF_FUNCTION_NAME_HPP
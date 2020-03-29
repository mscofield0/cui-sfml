#ifndef CUI_CT_STYLES_INDEX_OF_FUNCTION_NAME_HPP
#define CUI_CT_STYLES_INDEX_OF_FUNCTION_NAME_HPP

#include <compile_time/styles/detail/constants.hpp>
#include <compile_time/string/string_view.hpp>
#include <aliases.hpp>

namespace cui::ct::styles::detail {

constexpr auto index_of_function_name(const StringView fn_name) -> u64 {
	auto idx = function_names.size();
	for (u64 i = 0; i < function_names.size(); ++i) {
		if (fn_name == function_names[i]) {
			idx = i;
			break;
		}
	}
	return idx;
}

}	 // namespace cui::ct::styles::detail

#endif	  // CUI_CT_STYLES_INDEX_OF_FUNCTION_NAME_HPP
#ifndef CUI_CT_SCENES_COUNT_TABS_HPP
#define CUI_CT_SCENES_COUNT_TABS_HPP

#include <compile_time/string/string_view.hpp>
#include <aliases.hpp>

namespace cui::ct::scenes::detail {

constexpr auto count_tabs(const StringView str) -> u64 {
	u64 count = 0;
	while (count < str.size()) {
		if (str[count] != '\t') break;
		++count;
	}
	return count;
}

}	 // namespace cui::ct::scenes::detail

#endif	  // CUI_CT_SCENES_COUNT_TABS_HPP
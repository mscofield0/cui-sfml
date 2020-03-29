#ifndef CUI_SCENES_DETAIL_COUNT_TABS_HPP
#define CUI_SCENES_DETAIL_COUNT_TABS_HPP

#include <compile_time/string_view.hpp>

namespace cui::scenes::detail__ {

constexpr auto count_tabs(const StringView str) -> StringView::size_type {
	StringView::size_type count = 0;
	while (count < str.size()) {
		if (str[count] != '\t') break;
		++count;
	}
	return count;
}

}	 // namespace cui::scenes::detail__

#endif	  // CUI_SCENES_DETAIL_COUNT_TABS_HPP
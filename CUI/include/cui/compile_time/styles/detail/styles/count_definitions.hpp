#ifndef CUI_CT_STYLES_COUNT_DEFINITIONS_HPP
#define CUI_CT_STYLES_COUNT_DEFINITIONS_HPP

#include <compile_time/string/string_view.hpp>
#include <compile_time/stream/stream.hpp>
#include <compile_time/stream/stream_utils.hpp>
#include <aliases.hpp>

namespace cui::ct::styles {

constexpr auto count_definitions(const StringView str) -> u32 {
	CharStream stream{str};
	u32 count = 0;

	while (stream.good()) {
		if (StringView::traits_type::eq(*stream.current(), '}')) ++count;
		stream.next();
	}
	return count;
}

}	 // namespace cui::ct::styles

#endif	  // CUI_CT_STYLES_DETAIL_COUNT_DEFINITIONS_HPP
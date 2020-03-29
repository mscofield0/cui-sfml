#ifndef CUI_STYLES_COUNT_DEFINITIONS_HPP
#define CUI_STYLES_COUNT_DEFINITIONS_HPP

#include <global/constants.hpp>
#include <compile_time/string_view.hpp>
#include <parse_utils/stream.hpp>
#include <parse_utils/stream_utils.hpp>

namespace cui::styles {

constexpr auto count_definitions(const StringView str) -> std::size_t {
	CharStream stream{str};
	std::size_t count = 0;

	while (stream.good()) {
		if (StringView::traits_type::eq(*stream.current(), '}')) ++count;
		stream.next();
	}
	return count;
}

}	 // namespace cui::styles

#endif	  // CUI_STYLES_COUNT_DEFINITIONS_HPP
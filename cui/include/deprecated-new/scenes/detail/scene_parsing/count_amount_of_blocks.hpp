#ifndef CUI_SCENES_DETAIL_COUNT_AMOUNT_OF_BLOCKS_HPP
#define CUI_SCENES_DETAIL_COUNT_AMOUNT_OF_BLOCKS_HPP

#include <parse_utils/stream.hpp>
#include <parse_utils/stream_utils.hpp>
#include <compile_time/string_utils/trim_sv.hpp>
#include <compile_time/string_view.hpp>

namespace cui::scenes::detail__ {

constexpr auto count_amount_of_blocks(const StringView str) -> StringView::size_type {
	CharStream stream{str};
	StringView::size_type amount = 0;
	while (stream.good()) {
		auto line = consume_line(stream);
		auto trimmed = trim_sv(line);
		if (trimmed.empty()) continue;
		++amount;
	}
	return amount;
}

}	 // namespace cui::scenes::detail__

#endif	  // CUI_SCENES_DETAIL_COUNT_AMOUNT_OF_BLOCKS_HPP
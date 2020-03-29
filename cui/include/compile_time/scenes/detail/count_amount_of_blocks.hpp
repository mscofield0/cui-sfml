#ifndef CUI_CT_SCENES_COUNT_AMOUNT_OF_BLOCKS_HPP
#define CUI_CT_SCENES_COUNT_AMOUNT_OF_BLOCKS_HPP

#include <compile_time/stream/stream.hpp>
#include <compile_time/stream/stream_utils.hpp>
#include <compile_time/string/string_view.hpp>
#include <aliases.hpp>

namespace cui::ct::scenes::detail {

constexpr auto count_amount_of_blocks(const StringView str) -> u64 {
	CharStream stream(str);
	u64 amount = 0;
	while (stream.good()) {
		const auto line = consume_line(stream);
		const auto trimmed = line.trim();
		if (trimmed.empty()) continue;
		++amount;
	}

	return amount;
}

}	 // namespace cui::ct::scenes::detail

#endif	  // CUI_CT_SCENES_COUNT_AMOUNT_OF_BLOCKS_HPP
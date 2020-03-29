#ifndef CUI_SCENES_DETAIL_COUNT_MAX_DEPTH_HPP
#define CUI_SCENES_DETAIL_COUNT_MAX_DEPTH_HPP

#include <scenes/detail/scene_parsing/count_tabs.hpp>
#include <parse_utils/stream.hpp>
#include <parse_utils/stream_utils.hpp>
#include <compile_time/string_utils/trim_sv.hpp>
#include <compile_time/string_view.hpp>

namespace cui::scenes::detail__ {

constexpr auto count_max_depth(const StringView str) -> StringView::size_type {
	CharStream stream{str};
	StringView::size_type max = 0;
	while (stream.good()) {
		auto line = consume_line(stream);
		auto trimmed = trim_sv(line);
		if (trimmed.empty()) continue;
		auto t = detail__::count_tabs(line);
		if (t > max) max = t;
	}
	return max;
}

}	 // namespace cui::scenes::detail__

#endif	  // CUI_SCENES_DETAIL_COUNT_MAX_DEPTH_HPP
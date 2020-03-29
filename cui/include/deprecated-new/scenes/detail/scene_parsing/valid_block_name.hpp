#ifndef CUI_SCENES_DETAIL_VALID_BLOCK_NAME_HPP
#define CUI_SCENES_DETAIL_VALID_BLOCK_NAME_HPP

#include <compile_time/string_view.hpp>

namespace cui::scenes::detail__ {

constexpr auto is_valid_block_name(const StringView str) -> bool {
	constexpr StringView valid_chars{"abcdefghijklmnopqrstuvwxyz0123456789_"};
	if (str.empty()) return false;
	if (str[0] >= '0' && str[0] <= '9') return false;
	for (auto el : str) {
		bool b = false;
		for (auto vc : valid_chars) {
			if (el == vc) {
				b = true;
				break;
			}
		}
		if (!b) return false;
	}
	return true;
}

}	 // namespace cui::scenes::detail__

#endif	  // CUI_SCENES_DETAIL_VALID_BLOCK_NAME_HPP
#ifndef CUI_STYLES_DETAIL_PARSE_INT_HPP
#define CUI_STYLES_DETAIL_PARSE_INT_HPP

#include <styles/data.hpp>
#include <compile_time/string_view.hpp>
#include <compile_time/pow.hpp>

namespace cui::styles::detail__ {

constexpr auto parse_int(const StringView str) -> ValueData {
	bool minus = (str[0] == '-') ? true : false;
	int num = 0;
	for (size_t i = minus ? 1 : 0; i < str.size(); ++i) {
		const auto n = str[i] - '0';
		num += n * cui::pow(10, str.size() - i - 1);
	}
	return num * (minus ? -1 : 1);
}

}	 // namespace cui::styles::detail__

#endif	  // CUI_STYLES_DETAIL_PARSE_INT_HPP
#ifndef CUI_STYLES_DETAIL_PARSE_FLOAT_HPP
#define CUI_STYLES_DETAIL_PARSE_FLOAT_HPP

#include <styles/data.hpp>
#include <compile_time/string_view.hpp>
#include <compile_time/pow.hpp>

namespace cui::styles::detail__ {

constexpr auto parse_float(const StringView str) -> ValueData {
	const bool minus = (str[0] == '-') ? true : false;
	const auto point_comma = str.find('.');
	const auto point_colon = str.find(',');
	auto point_idx = point_comma > point_colon ? point_colon : point_comma;
	point_idx = (point_idx == StringView::npos) ? str.size() : point_idx;
	float num = 0;
	for (size_t i = minus ? 1 : 0; i < point_idx; ++i) {
		const auto n = str[i] - '0';
		const auto size = point_idx;
		num += n * cui::pow(10, size - i - 1);
	}
	if (point_idx == str.size()) {
		return num * (minus ? -1 : 1);
	}
	for (size_t i = point_idx + 1 + (minus ? 1 : 0); i < str.size(); ++i) {
		auto const n = str[i] - '0';
		num += n * cui::pow(0.1f, i - point_idx);
	}
	return num * (minus ? -1 : 1);
}

}	 // namespace cui::styles::detail__

#endif	  // CUI_STYLES_DETAIL_PARSE_FLOAT_HPP
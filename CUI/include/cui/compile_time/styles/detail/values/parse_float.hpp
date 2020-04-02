#ifndef CUI_CT_STYLES_PARSE_FLOAT_HPP
#define CUI_CT_STYLES_PARSE_FLOAT_HPP

#include <compile_time/value_data.hpp>
#include <compile_time/string/string_view.hpp>
#include <compile_time/styles/detail/values/pow.hpp>
#include <aliases.hpp>

namespace cui::ct::styles::detail {

constexpr auto parse_float(const StringView str) -> ValueData {
	const bool minus = (str[0] == '-') ? true : false;
	const auto point_comma = str.find('.');
	const auto point_colon = str.find(',');
	auto point_idx = point_comma > point_colon ? point_colon : point_comma;
	point_idx = (point_idx == StringView::npos) ? str.size() : point_idx;
	float num = 0;
	for (u64 i = minus ? 1 : 0; i < point_idx; ++i) {
		const auto n = str[i] - '0';
		const auto size = point_idx;
		num += n * pow(10, size - i - 1);
	}
	if (point_idx == str.size()) {
		return num * (minus ? -1 : 1);
	}
	for (u64 i = point_idx + 1 + (minus ? 1 : 0); i < str.size(); ++i) {
		auto const n = str[i] - '0';
		num += n * pow(0.1f, i - point_idx);
	}
	return num * (minus ? -1 : 1);
}

}	 // namespace cui::ct::styles::detail

#endif	  // CUI_CT_STYLES_PARSE_FLOAT_HPP
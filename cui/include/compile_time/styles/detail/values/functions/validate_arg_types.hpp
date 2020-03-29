#ifndef CUI_CT_STYLES_FUNCTIONS_VALIDATE_ARG_TYPES_HPP
#define CUI_CT_STYLES_FUNCTIONS_VALIDATE_ARG_TYPES_HPP

#include <compile_time/value_data.hpp>
#include <containers/static_vector.hpp>
#include <aliases.hpp>

namespace cui::ct::styles::detail {

constexpr bool all_int(const ValueData& val) {
	return val.is_int();
}

constexpr bool all_float(const ValueData& val) {
	return val.is_float();
}

template <u64 Size, typename Func>
constexpr bool validate_args(const StaticVector<ValueData, Size>& args, u64 size, Func fn) {
	u64 i = 0;
	while (i < size) {
		if (!fn(args[i++])) return false;
	}
	return true;
}

}	 // namespace cui::ct::styles::detail

#endif	  // CUI_CT_STYLES_FUNCTIONS_VALIDATE_ARG_TYPES_HPP
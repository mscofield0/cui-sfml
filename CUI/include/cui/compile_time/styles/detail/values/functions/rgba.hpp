#ifndef CUI_CT_STYLES_FUNCTIONS_RGBA_HPP
#define CUI_CT_STYLES_FUNCTIONS_RGBA_HPP

#include <compile_time/styles/detail/values/functions/dependencies.hpp>

namespace cui::ct::styles::detail {

DEFINE_FUNCTION(rgba) {
	const auto& args = fal.list();
	IF_NOT_IN_SET(4) {
		RETURN_ERROR("ERROR: Invalid amount of arguments; Supplied {} arguments", args.size());
	}
	if (validate_args(args, 4, all_int)) {
		RETURN_ERROR("ERROR: Expected a set of int RGBA value.");
	}
	const auto& r = args[0].integer_value();
	const auto& g = args[1].integer_value();
	const auto& b = args[2].integer_value();
	const auto& a = args[3].integer_value();
	RETURN_VALUE(Color(r, g, b, a), ValueType::Color);
}

}	 // namespace cui::ct::styles::detail

#endif	  // CUI_CT_STYLES_FUNCTIONS_RGBA_HPP
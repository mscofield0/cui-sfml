#ifndef CUI_CT_STYLES_FUNCTIONS_RGB_HPP
#define CUI_CT_STYLES_FUNCTIONS_RGB_HPP

#include <compile_time/styles/detail/values/functions/dependencies.hpp>

namespace cui::ct::styles::detail {

DEFINE_FUNCTION(rgb) {
	const auto& args = fal.list();
	IF_NOT_IN_SET(3) {
		RETURN_ERROR("ERROR: Invalid amount of arguments;\n\tSupplied {} arguments", args.size());
	}
	if (!validate_args(args, 3, all_int)) {
		RETURN_ERROR("ERROR: Expected a set of int RGBA value.");
	}
	const auto& r = args[0].integer_value();
	const auto& g = args[1].integer_value();
	const auto& b = args[2].integer_value();
	RETURN_VALUE(Color(r, g, b), ValueType::Color);
}

}	 // namespace cui::ct::styles::detail

#endif	  // CUI_CT_STYLES_FUNCTIONS_RGB_HPP
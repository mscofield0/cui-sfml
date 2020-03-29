#ifndef CUI_STYLES_DETAIL_LOOKUP_FUNCTION_HPP
#define CUI_STYLES_DETAIL_LOOKUP_FUNCTION_HPP

#include <styles/detail/function_argument_list.hpp>
#include <compile_time/variant.hpp>
#include <styles/function_ids.hpp>
#include <styles/data_with_rule.hpp>

#ifdef CUI_ENABLE_CONSTEXPR_PARSING

#include <compile_time/format.hpp>

namespace cui::styles::detail__ {

constexpr auto lookup_function(size_t fn_name_idx, const detail__::FunctionArgumentList& fal)
  -> cui::Variant<styles::DataWithRule, ct::Format<512>> {
	const auto& args = fal.list();
	switch (styles::function_ids[fn_name_idx]) {
		case styles::FunctionId::RGB: {
			if (args.size() != 3 && args.size() != 1) {
				return ct::create_formatted_string_with_max_size<512>(
				  "ERROR: Invalid amount of arguments; Supplied {} arguments",
				  uint64_t{args.size()});
			}

			if (args.size() == 1) {
				if (!args[0].is_rgba()) {
					return ct::create_formatted_string_with_max_size<512>("ERROR: Expected a color RGBA value.");
				}
				return DataWithRule{ValueData{cui::Color(args[0].integer_value())}, ValueType::Color};
			}

			if (args.size() == 3) {
				if (!args[0].is_int() || !args[1].is_int() || !args[2].is_int()) {
					return ct::create_formatted_string_with_max_size<512>("ERROR: Expected a set of int RGBA value.");
				}
				return DataWithRule{
				  ValueData{cui::Color(args[0].integer_value(), args[1].integer_value(), args[2].integer_value())},
				  ValueType::Color};
			}
			break;
		}
		case styles::FunctionId::RGBA: {
			if (args.size() != 4 && args.size() != 1) {
				return ct::create_formatted_string_with_max_size<512>(
				  "ERROR: Invalid amount of arguments; Supplied {} arguments",
				  uint64_t{args.size()});
			}

			if (args.size() == 1) {
				if (!args[0].is_rgba()) {
					return ct::create_formatted_string_with_max_size<512>("ERROR: Expected a color RGBA value.");
				}
				return DataWithRule{ValueData{cui::Color(args[0].integer_value())}, ValueType::Color};
			} else if (args.size() == 4) {
				if (!args[0].is_int() || !args[1].is_int() || !args[2].is_int() || !args[3].is_int()) {
					return ct::create_formatted_string_with_max_size<512>("ERROR: Expected a set of int RGBA value.");
				}
				return DataWithRule{ValueData{cui::Color(args[0].integer_value(),
														 args[1].integer_value(),
														 args[2].integer_value(),
														 args[3].integer_value())},
									ValueType::Color};
			}
			break;
		}
		case styles::FunctionId::Vec: {
			if (args.size() < 2) {
				return ct::create_formatted_string_with_max_size<512>(
				  "ERROR: Expected two or more values in the vector. Received: {} values ",
				  uint64_t{args.size()});
			}
			switch (args.size()) {
				case 2: {
					if (args[0].is_int() && args[1].is_int()) {
						return DataWithRule{ValueData{Vector2f{args[0].integer_value(), args[1].integer_value()}},
											ValueType::VectorInt};
					} else if (args[0].is_float() && args[1].is_float()) {
						return DataWithRule{ValueData{Vector2f{args[0].float_value(), args[1].float_value()}},
											ValueType::VectorFloat};
					} else {
						return ct::create_formatted_string_with_max_size<512>(
						  "ERROR: Expected the vector values to be int or float types.");
					}
				}
				case 3: {
					if (args[0].is_int() && args[1].is_int() && args[2].is_int()) {
						return DataWithRule{
						  ValueData{
							Vector3f{args[0].integer_value(), args[1].integer_value(), args[2].integer_value()}},
						  ValueType::VectorInt};
					} else if (args[0].is_float() && args[1].is_float() && args[2].is_float()) {
						return DataWithRule{
						  ValueData{Vector3f{args[0].float_value(), args[1].float_value(), args[2].float_value()}},
						  ValueType::VectorFloat};
					} else {
						return ct::create_formatted_string_with_max_size<512>(
						  "ERROR: Expected the vector values to be int or float types.");
					}
				}
				default: {
					if (args[0].is_int() && args[1].is_int() && args[2].is_int() && args[3].is_int()) {
						return DataWithRule{ValueData{Vector4f{args[0].integer_value(),
															   args[1].integer_value(),
															   args[2].integer_value(),
															   args[3].integer_value()}},
											ValueType::VectorInt};
					} else if (args[0].is_float() && args[1].is_float() && args[2].is_float() && args[3].is_float()) {
						return DataWithRule{ValueData{Vector4f{args[0].float_value(),
															   args[1].float_value(),
															   args[2].float_value(),
															   args[3].float_value()}},
											ValueType::VectorFloat};
					} else {
						return ct::create_formatted_string_with_max_size<512>(
						  "ERROR: Expected the vector values to be int or float types.");
					}
				} break;
			}
			// Next case
		}
		default: {
			return ct::create_formatted_string_with_max_size<512>("ERROR: Invalid function index.");
		}
	}
}

}	 // namespace cui::styles::detail__

#else

#include <global/format.hpp>

namespace cui::styles::detail__ {

auto lookup_function(size_t fn_name_idx, const detail__::FunctionArgumentList& fal)
  -> cui::Variant<styles::DataWithRule, Format> {
	const auto& args = fal.list();
	switch (styles::function_ids[fn_name_idx]) {
		case styles::FunctionId::RGB: {
			if (args.size() != 3 && args.size() != 1) {
				return create_formatted_string("ERROR: Invalid amount of arguments; Supplied {} arguments",
											   uint64_t{args.size()});
			}

			if (args.size() == 1) {
				if (!args[0].is_rgba()) {
					return create_formatted_string("ERROR: Expected a color RGBA value.");
				}
				return DataWithRule{ValueData{cui::Color(args[0].integer_value())}, ValueType::Color};
			}

			if (args.size() == 3) {
				if (!args[0].is_int() || !args[1].is_int() || !args[2].is_int()) {
					return create_formatted_string("ERROR: Expected a set of int RGBA value.");
				}
				return DataWithRule{
				  ValueData{cui::Color(args[0].integer_value(), args[1].integer_value(), args[2].integer_value())},
				  ValueType::Color};
			}
			break;
		}
		case styles::FunctionId::RGBA: {
			if (args.size() != 4 && args.size() != 1) {
				return create_formatted_string("ERROR: Invalid amount of arguments; Supplied {} arguments",
											   uint64_t{args.size()});
			}

			if (args.size() == 1) {
				if (!args[0].is_rgba()) {
					return create_formatted_string("ERROR: Expected a color RGBA value.");
				}
				return DataWithRule{ValueData{cui::Color(args[0].integer_value())}, ValueType::Color};
			} else if (args.size() == 4) {
				if (!args[0].is_int() || !args[1].is_int() || !args[2].is_int() || !args[3].is_int()) {
					return create_formatted_string("ERROR: Expected a set of int RGBA value.");
				}
				return DataWithRule{ValueData{cui::Color(args[0].integer_value(),
														 args[1].integer_value(),
														 args[2].integer_value(),
														 args[3].integer_value())},
									ValueType::Color};
			}
			break;
		}
		case styles::FunctionId::Vec: {
			if (args.size() < 2) {
				return create_formatted_string("ERROR: Expected two or more values in the vector. Received: {} values ",
											   uint64_t{args.size()});
			}
			switch (args.size()) {
				case 2: {
					if (args[0].is_int() && args[1].is_int()) {
						return DataWithRule{ValueData{Vector2f{args[0].integer_value(), args[1].integer_value()}},
											ValueType::VectorInt};
					} else if (args[0].is_float() && args[1].is_float()) {
						return DataWithRule{ValueData{Vector2f{args[0].float_value(), args[1].float_value()}},
											ValueType::VectorFloat};
					} else {
						return create_formatted_string("ERROR: Expected the vector values to be int or float types.");
					}
				}
				case 3: {
					if (args[0].is_int() && args[1].is_int() && args[2].is_int()) {
						return DataWithRule{
						  ValueData{
							Vector3f{args[0].integer_value(), args[1].integer_value(), args[2].integer_value()}},
						  ValueType::VectorInt};
					} else if (args[0].is_float() && args[1].is_float() && args[2].is_float()) {
						return DataWithRule{
						  ValueData{Vector3f{args[0].float_value(), args[1].float_value(), args[2].float_value()}},
						  ValueType::VectorFloat};
					} else {
						return create_formatted_string("ERROR: Expected the vector values to be int or float types.");
					}
				}
				default: {
					if (args[0].is_int() && args[1].is_int() && args[2].is_int() && args[3].is_int()) {
						return DataWithRule{ValueData{Vector4f{args[0].integer_value(),
															   args[1].integer_value(),
															   args[2].integer_value(),
															   args[3].integer_value()}},
											ValueType::VectorInt};
					} else if (args[0].is_float() && args[1].is_float() && args[2].is_float() && args[3].is_float()) {
						return DataWithRule{ValueData{Vector4f{args[0].float_value(),
															   args[1].float_value(),
															   args[2].float_value(),
															   args[3].float_value()}},
											ValueType::VectorFloat};
					} else {
						return create_formatted_string("ERROR: Expected the vector values to be int or float types.");
					}
				} break;
			}
		}
	}

	return create_formatted_string("ERROR: Invalid function index.");
}

}	 // namespace cui::styles::detail__

#endif

#endif	  // CUI_STYLES_DETAIL_LOOKUP_FUNCTION_HPP
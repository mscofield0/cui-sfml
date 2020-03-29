#ifndef CUI_STYLES_DETAIL_PARSE_ATTRIBUTE_VALUE_HPP
#define CUI_STYLES_DETAIL_PARSE_ATTRIBUTE_VALUE_HPP

#include <compile_time/string_view.hpp>
#include <compile_time/variant.hpp>
#include <parse_utils/stream.hpp>
#include <styles/data.hpp>
#include <styles/detail/char_checks.hpp>
#include <styles/detail/parse_int.hpp>
#include <styles/detail/parse_float.hpp>
#include <styles/detail/parse_percentage.hpp>
#include <styles/detail/lookup_string.hpp>
#include <styles/detail/lookup_function.hpp>
#include <styles/detail/index_of_function_name.hpp>
#include <styles/detail/function_argument_list.hpp>

#include <styles/data_with_rule.hpp>

#ifdef CUI_ENABLE_CONSTEXPR_PARSING

#include <compile_time/format.hpp>

namespace cui::styles::detail__ {

constexpr auto parse_attribute_value(const StringView str) -> cui::Variant<styles::DataWithRule, ct::Format<512>> {
	enum class States : uint8_t
	{
		None = 0,
		Int,
		Minus,
		Float,
		Percentage,
		String,
		Function,
		FunctionEnd
	} state = States::None;

	CharStream stream{str};

	while (stream.good()) {
		const auto ch = *stream.current();
		if (detail__::is_digit(ch)) {
			switch (state) {
				case States::None:
				case States::Minus: {
					state = States::Int;
					break;
				}
				case States::Int:
				case States::Float:
				case States::String: {
					break;
				}
				case States::Function: {
					return ct::create_formatted_string_with_max_size<512>(
					  "ERROR: Missing end parenthesis to the function.");
				}
				default: {
					return ct::create_formatted_string_with_max_size<512>("ERROR: End of value expected.");
				}
			}
		} else if (detail__::is_minus(ch)) {
			switch (state) {
				case States::None: {
					state = States::Minus;
					break;
				}
				default: {
					return ct::create_formatted_string_with_max_size<512>(
					  "ERROR: Minus expected at the beginning of the value.");
				}
			}
		} else if (detail__::is_percent(ch)) {
			switch (state) {
				case States::Int:
				case States::Float: {
					state = States::Percentage;
					break;
				}
				case States::Function: {
					return ct::create_formatted_string_with_max_size<512>(
					  "ERROR: Missing end parenthesis to the function.");
				}
				default: {
					return ct::create_formatted_string_with_max_size<512>(
					  "ERROR: Expected int or float value before percent sign.");
				}
			}
		} else if (detail__::is_point(ch)) {
			switch (state) {
				case States::None:
				case States::Int: {
					state = States::Float;
					break;
				}
				case States::Function: {
					return ct::create_formatted_string_with_max_size<512>(
					  "ERROR: Missing end parenthesis to the function.");
				}
				default: {
					return ct::create_formatted_string_with_max_size<512>(
					  "ERROR: Expected int value or point starting the value.");
				}
			}
		} else if (detail__::is_open_parenthesis(ch)) {
			switch (state) {
				case States::String: {
					state = States::Function;
					if (str.find_last_of(',') != StringView::npos) {
						state = States::FunctionEnd;
					}

					while (stream.good()) stream.next();

					break;
				}
				case States::Function: {
					return ct::create_formatted_string_with_max_size<512>(
					  "ERROR: Missing end parenthesis to the function.");
				}
				default: {
					return ct::create_formatted_string_with_max_size<512>("ERROR: Expected function name.");
				}
			}
		} else if (detail__::is_closed_parenthesis(ch)) {
			switch (state) {
				case States::Function: {
					state = States::FunctionEnd;
					break;
				}
				default: {
					return ct::create_formatted_string_with_max_size<512>("ERROR: Expected a function.");
				}
			}
		} else if (detail__::is_valid_char(ch)) {
			switch (state) {
				case States::None: {
					state = States::String;
					break;
				}
				case States::String: {
					break;
				}
				case States::Function: {
					return ct::create_formatted_string_with_max_size<512>("ERROR: Expected a function.");
				}
				default: {
					return ct::create_formatted_string_with_max_size<512>("ERROR: Expected a char or an int.");
				}
			}
		} else {
			return ct::create_formatted_string_with_max_size<512>("ERROR: Invalid char in value.");
		}

		stream.next();
	}

	switch (state) {
		case States::Int: {
			return DataWithRule{detail__::parse_int(str), ValueType::Int};
		}
		case States::Float: {
			return DataWithRule{detail__::parse_float(str), ValueType::Float};
		}
		case States::Percentage: {
			return DataWithRule{detail__::parse_percentage(str), ValueType::Percentage};
		}
		case States::String: {
			return DataWithRule{detail__::lookup_string(str), ValueType::String};
		}
		case States::FunctionEnd: {
			const auto open_parenthesis_idx = str.find_first_of('(');
			auto fn_name = str.substr(0, open_parenthesis_idx);
			const auto fn_name_idx = detail__::index_of_function_name(fn_name);
			if (fn_name_idx == styles::function_names.size()) {
				return ct::create_formatted_string_with_max_size<512>("ERROR: Function name invalid.\nName: {}.",
																	  fn_name);
			}
			const auto closed_parenthesis_idx = str.find_last_of(')');
			const auto args_variant = detail__::FunctionArgumentList::parse(
			  str.substr(open_parenthesis_idx + 1, closed_parenthesis_idx - open_parenthesis_idx - 1));
			if (args_variant.is_type_b()) {
				return args_variant.type_b();
			}

			const auto fn_processed_val = detail__::lookup_function(fn_name_idx, args_variant.type_a());
			if (fn_processed_val.is_type_b()) {
				return fn_processed_val.type_b();
			}

			return fn_processed_val.type_a();
		}
	}

	return ct::create_formatted_string_with_max_size<512>("ERROR: Unexpected error.");
}

}	 // namespace cui::styles::detail__

#else	 // Disable constexpr parse_attribute_value()

#include <global/format.hpp>

namespace cui::styles::detail__ {

auto parse_attribute_value(const StringView str) -> cui::Variant<styles::DataWithRule, Format> {
	enum class States : uint8_t
	{
		None = 0,
		Int,
		Minus,
		Float,
		Percentage,
		String,
		Function,
		FunctionEnd
	} state = States::None;

	CharStream stream{str};

	while (stream.good()) {
		const auto ch = *stream.current();
		if (detail__::is_digit(ch)) {
			switch (state) {
				case States::None:
				case States::Minus: {
					state = States::Int;
					break;
				}
				case States::Int:
				case States::Float:
				case States::String: {
					break;
				}
				case States::Function: {
					return create_formatted_string("ERROR: Missing end parenthesis to the function.");
				}
				default: {
					return create_formatted_string("ERROR: End of value expected.");
				}
			}
		} else if (detail__::is_minus(ch)) {
			switch (state) {
				case States::None: {
					state = States::Minus;
					break;
				}
				default: {
					return create_formatted_string("ERROR: Minus expected at the beginning of the value.");
				}
			}
		} else if (detail__::is_percent(ch)) {
			switch (state) {
				case States::Int:
				case States::Float: {
					state = States::Percentage;
					break;
				}
				case States::Function: {
					return create_formatted_string("ERROR: Missing end parenthesis to the function.");
				}
				default: {
					return create_formatted_string("ERROR: Expected int or float value before percent sign.");
				}
			}
		} else if (detail__::is_point(ch)) {
			switch (state) {
				case States::None:
				case States::Int: {
					state = States::Float;
					break;
				}
				case States::Function: {
					return create_formatted_string("ERROR: Missing end parenthesis to the function.");
				}
				default: {
					return create_formatted_string("ERROR: Expected int value or point starting the value.");
				}
			}
		} else if (detail__::is_open_parenthesis(ch)) {
			switch (state) {
				case States::String: {
					state = States::Function;
					if (str.find_last_of(',') != StringView::npos) {
						state = States::FunctionEnd;
					}

					while (stream.good()) stream.next();

					break;
				}
				case States::Function: {
					return create_formatted_string("ERROR: Missing end parenthesis to the function.");
				}
				default: {
					return create_formatted_string("ERROR: Expected function name.");
				}
			}
		} else if (detail__::is_closed_parenthesis(ch)) {
			switch (state) {
				case States::Function: {
					state = States::FunctionEnd;
					break;
				}
				default: {
					return create_formatted_string("ERROR: Expected a function.");
				}
			}
		} else if (detail__::is_valid_char(ch)) {
			switch (state) {
				case States::None: {
					state = States::String;
					break;
				}
				case States::String: {
					break;
				}
				case States::Function: {
					return create_formatted_string("ERROR: Expected a function.");
				}
				default: {
					return create_formatted_string("ERROR: Expected a char or an int.");
				}
			}
		} else {
			return create_formatted_string("ERROR: Invalid char in value.");
		}

		stream.next();
	}

	switch (state) {
		case States::Int: {
			return DataWithRule{detail__::parse_int(str), ValueType::Int};
		}
		case States::Float: {
			return DataWithRule{detail__::parse_float(str), ValueType::Float};
		}
		case States::Percentage: {
			return DataWithRule{detail__::parse_percentage(str), ValueType::Percentage};
		}
		case States::String: {
			return DataWithRule{detail__::lookup_string(str), ValueType::String};
		}
		case States::FunctionEnd: {
			const auto open_parenthesis_idx = str.find_first_of('(');
			auto fn_name = str.substr(0, open_parenthesis_idx);
			const auto fn_name_idx = detail__::index_of_function_name(fn_name);
			if (fn_name_idx == styles::function_names.size()) {
				return create_formatted_string("ERROR: Function name invalid.\nName: {}.", fn_name);
			}
			const auto closed_parenthesis_idx = str.find_last_of(')');
			const auto args_variant = detail__::FunctionArgumentList::parse(
			  str.substr(open_parenthesis_idx + 1, closed_parenthesis_idx - open_parenthesis_idx - 1));
			if (args_variant.is_type_b()) {
				return args_variant.type_b();
			}

			const auto fn_processed_val = detail__::lookup_function(fn_name_idx, args_variant.type_a());
			if (fn_processed_val.is_type_b()) {
				return fn_processed_val.type_b();
			}

			return fn_processed_val.type_a();
		}
		default: {
			return create_formatted_string("ERROR: Shouldn't ever happen.");
		}
	}

	return create_formatted_string("ERROR: Unexpected error.");
}

}	 // namespace cui::styles::detail__

#endif

#endif	  // CUI_STYLES_DETAIL_PARSE_ATTRIBUTE_VALUE_HPP
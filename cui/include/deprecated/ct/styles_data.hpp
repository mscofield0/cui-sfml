#ifndef CUI_STYLES_DATA_HPP
#define CUI_STYLES_DATA_HPP

#include <array>
#include "string.hpp"
#include "styles.hpp"
#include "variant.hpp"
#include "static_string_stream.hpp"

namespace cui {
enum block_instructions : uint32_t
{
	CONTAIN = 0
};

enum attr_valid_type : uint64_t
{
	INVALID = 0,
	RGBA = 1,
	VEC2 = 1 << 1,
	VEC3 = 1 << 2,
	VEC4 = 1 << 3,
	FLOAT = 1 << 4,
	INT = 1 << 5,
	INSTR = 1 << 6
};

}	 // namespace cui

namespace cui::ct {

struct color_rgba
{
	uint32_t rgba;

	static constexpr uint32_t to_rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) {
		uint32_t val = 0;
		val = r << 24;
		val |= g << 16;
		val |= b << 8;
		val |= a;
		return val;
	}

	constexpr color_rgba() : rgba(to_rgba(0, 0, 0)) {}
	constexpr color_rgba(uint8_t r, uint8_t g, uint8_t b) : rgba(to_rgba(r, g, b)) {}
	constexpr color_rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : rgba(to_rgba(r, g, b, a)) {}

	constexpr int r() const {
		return static_cast<uint8_t>(rgba >> 24);
	}
	constexpr int g() const {
		return static_cast<uint8_t>(rgba >> 16);
	}
	constexpr int b() const {
		return static_cast<uint8_t>(rgba >> 8);
	}
	constexpr int a() const {
		return static_cast<uint8_t>(rgba);
	}
};

struct empty
{
	constexpr empty() {}
};

using empty_t = empty;

struct vec2
{
	enum active_ : uint8_t
	{
		NONE = 0,
		INT,
		FLOAT
	} active;

	union
	{
		empty_t none;
		int val_i[2];
		float val_f[2];
	};

	constexpr vec2() : active{NONE}, none{} {}
	constexpr vec2(int x, int y) : active{INT}, val_i{x, y} {}
	constexpr vec2(float x, float y) : active{FLOAT}, val_f{x, y} {}
};

struct vec3
{
	enum active_ : uint8_t
	{
		NONE = 0,
		INT,
		FLOAT
	} active;

	union
	{
		empty_t none;
		int val_i[3];
		float val_f[3];
	};

	constexpr vec3() : active{NONE}, none{} {}
	constexpr vec3(int x, int y, int z) : active{INT}, val_i{x, y, z} {}
	constexpr vec3(float x, float y, float z) : active{FLOAT}, val_f{x, y, z} {}
};

struct vec4
{
	enum active_ : uint8_t
	{
		NONE = 0,
		INT,
		FLOAT
	} active;

	union
	{
		empty_t none;
		int val_i[4];
		float val_f[4];
	};

	constexpr vec4() : active{NONE}, none{} {}
	constexpr vec4(int x, int y, int z, int w) : active{INT}, val_i{x, y, z, w} {}
	constexpr vec4(float x, float y, float z, float w) : active{FLOAT}, val_f{x, y, z, w} {}
};

struct block_instruction
{
	cui::block_instructions instr;

	constexpr block_instruction(uint8_t instr_code) : instr{static_cast<cui::block_instructions>(instr_code)} {}
};

struct styles_value_data
{
	enum active_ : uint8_t
	{
		NONE = 0,
		COLOR_RGBA,
		FLOAT,
		INT,
		VEC2,
		VEC3,
		VEC4,
		INSTR
	} active;

	union
	{
		empty_t none;
		color_rgba rgba;
		float val_f;
		int val_i;
		vec2 v2;
		vec3 v3;
		vec4 v4;
		block_instruction binstr;
	};

	constexpr styles_value_data() : active(NONE), none() {}
	constexpr styles_value_data(color_rgba color) : active(COLOR_RGBA), rgba(color) {}
	constexpr styles_value_data(float val) : active(FLOAT), val_f(val) {}
	constexpr styles_value_data(int val) : active(INT), val_i(val) {}
	constexpr styles_value_data(vec2 val) : active(VEC2), v2(val) {}
	constexpr styles_value_data(vec3 val) : active(VEC3), v3(val) {}
	constexpr styles_value_data(vec4 val) : active(VEC4), v4(val) {}
	constexpr styles_value_data(block_instruction instr) : active(VEC4), binstr(instr) {}
};

struct attribute_data
{
	cui::ct::string type;
	styles_value_data value;

	constexpr attribute_data() : type(""), value() {}
	constexpr attribute_data(cui::ct::string type, styles_value_data value) : type(type), value(value) {}
};

}	 // namespace cui::ct

namespace cui {

constexpr auto attr_types__ = cui::util::make_array<cui::ct::string>("color", "size", "position");
constexpr auto fn_names__ = cui::util::make_array<cui::ct::string>("rgb", "rgba", "vec");
constexpr auto str_names__ = cui::util::make_array<cui::ct::string>("red", "green", "blue", "transparent", "contain");
constexpr auto values__ =
  cui::util::make_array<cui::ct::styles_value_data>(ct::color_rgba{255, 0, 0},
													ct::color_rgba{0, 255, 0},
													ct::color_rgba{0, 0, 255},
													ct::color_rgba{0, 0, 0, 0},
													ct::block_instruction{block_instructions::CONTAIN});

enum fn_names_id__ : uint8_t
{
	rgb = 0,
	rgba,
	vec
};

constexpr auto val_type__ = cui::util::make_array<uint64_t>(attr_valid_type::RGBA,
															attr_valid_type::VEC2 | attr_valid_type::INSTR,
															attr_valid_type::VEC2 | attr_valid_type::INSTR);

}	 // namespace cui

namespace cui::ct {

struct styles_data
{
private:
	struct detail
	{
		struct function_argument_list
		{
			std::array<styles_value_data, 64> list;
			uint32_t idx;

			constexpr function_argument_list() : list{}, idx(0) {}
			constexpr bool add_argument(styles_value_data arg) {
				if (idx >= 64) return false;
				list[idx] = arg;
				++idx;
				return true;
			}
		};

		static constexpr bool is_minus(char ch) {
			return ch == '-';
		}

		static constexpr bool is_whitespace(char ch) {
			return cui::util::is_whitespace(ch);
		}

		static constexpr bool is_digit(char ch) {
			return (ch >= '0' && ch <= '9');
		}

		static constexpr bool is_point(char ch) {
			return (ch == '.' || ch == ',');
		}

		static constexpr bool is_percent(char ch) {
			return ch == '%';
		}

		static constexpr bool is_open_parenthesis(char ch) {
			return ch == '(';
		}

		static constexpr bool is_closed_parenthesis(char ch) {
			return ch == ')';
		}

		static constexpr bool is_colon(char ch) {
			return ch == ',';
		}

		static constexpr bool is_valid_char(char ch) {
			constexpr auto valid_chars = cui::ct::string{"abcdefghijklmnopqrstuvwxyz_"};
			for (int i = 0; i < valid_chars.size; ++i) {
				if (valid_chars[i] == ch) return true;
			}
			return false;
		}

		static constexpr styles_value_data parse_int(cui::ct::string const& str) {
			int num = 0;
			bool minus = false;
			if (str[0] == '-') minus = true;
			for (int i = minus ? 1 : 0; i < str.size; ++i) {
				auto const n = str[i] - '0';
				auto const s = str.size;
				num += n * cui::util::pow(10, s - i - 1);
			}
			return num * (minus ? -1 : 1);
		}

		static constexpr styles_value_data parse_float(cui::ct::string const& str) {
			float num = 0;
			bool minus = false;
			if (str[0] == '-') minus = true;
			auto const point_comma = cui::ct::find(str, '.');
			auto const point_colon = cui::ct::find(str, ',');
			auto const point_idx = point_comma > point_colon ? point_colon : point_comma;
			for (int i = minus ? 1 : 0; i < point_idx; ++i) {
				auto const n = str[i] - '0';
				auto const s = point_idx;
				num += n * cui::util::pow(10, s - i - 1);
			}
			for (int i = point_idx + 1 + (minus ? 1 : 0); i < str.size; ++i) {
				auto const n = str[i] - '0';
				num += n * cui::util::pow(0.1f, i - point_idx);
			}
			return num * (minus ? -1 : 1);
		}

		static constexpr styles_value_data parse_percentage(cui::ct::string const& str) {
			float num = 0;
			bool minus = false;
			if (str[0] == '-') minus = true;
			auto const point_comma = cui::ct::find(str, '.');
			auto const point_colon = cui::ct::find(str, ',');
			auto const point_idx = point_comma > point_colon ? point_colon : point_comma;
			for (int i = minus ? 1 : 0; i < point_idx; ++i) {
				auto const n = str[i] - '0';
				auto const s = point_idx;
				num += n * cui::util::pow(10, s - i - 1);
			}
			if (point_idx == str.size) {
				return (num / 100) * (minus ? -1 : 1);
			}
			for (int i = point_idx + 1 + (minus ? 1 : 0); i < str.size; ++i) {
				auto const n = str[i] - '0';
				num += n * cui::util::pow(0.1f, i - point_idx);
			}
			return num * (minus ? -1 : 1);
		}

		static constexpr bool validate_attribute_type(cui::ct::string& type) {
			for (auto const& type_ : cui::attr_types__) {
				if (type_ == type) return true;
			}
			return false;
		}

		static constexpr styles_value_data lookup_string(cui::ct::string const& str) {
			for (int i = 0; i < str_names__.size(); ++i) {
				if (str_names__[i] == str) {
					return values__[i];
				}
			}
			return styles_value_data{};
		}

		static constexpr cui::ct::variant<function_argument_list, cui::ct::error_report>
		parse_function_argument_list(cui::ct::string const& str) {
			function_argument_list list{};
			if (str.size == 0) return list;

			auto prev = 0;
			auto pos = cui::ct::find(str, ',');
			while (pos != str.size) {
				if (prev == pos) {
					return cui::ct::error_report{"ERROR: Function argument list is invalid.\n~~~List: ", str};
				}
				auto trimmed = cui::ct::trim(cui::ct::string{str.str + prev, pos - prev});
				auto variant_val = parse_styles_value(trimmed);
				if (variant_val.active == variant_val.active_::T2_) {
					return variant_val.type2;
				}
				list.add_argument(variant_val.type1);
				prev = pos + 1;
				pos = cui::ct::find(str, ',', prev);
			}
			if (prev == pos) {
				return cui::ct::error_report{"ERROR: Function argument list is invalid.\n~~~List: ", str};
			}
			auto trimmed = cui::ct::trim(cui::ct::string{str.str + prev, str.size - prev});
			auto variant_val = parse_styles_value(trimmed);
			if (variant_val.active == variant_val.active_::T2_) {
				return variant_val.type2;
			}
			list.add_argument(variant_val.type1);
			return list;
		}

		static constexpr uint32_t index_of_function_name(cui::ct::string const& fn_name) {
			auto idx = fn_names__.size();
			for (int i = 0; i < fn_names__.size(); ++i) {
				if (fn_name == fn_names__[i]) {
					idx = i;
					break;
				}
			}
			return idx;
		}

		static constexpr cui::ct::variant<styles_value_data, cui::ct::error_report>
		lookup_function(uint32_t fn_name_idx, function_argument_list list) {
			using type_enum = styles_value_data::active_;
			auto const& l = list.list;
			switch (fn_name_idx) {
				case fn_names_id__::rgb: {
					if (list.idx != 3 && list.idx != 1) {
						return cui::ct::error_report{"ERROR: Invalid amount of arguments; Supplied ",
													 cui::ct::num_to_string(list.idx)};
					}

					if (list.idx == 1) {
						if (l[0].active != type_enum::COLOR_RGBA) {
							return cui::ct::error_report{"ERROR: Expected a color RGBA value."};
						}
						return l[0];
					} else if (list.idx == 3) {
						if (l[0].active != type_enum::INT && l[1].active != type_enum::INT &&
							l[2].active != type_enum::INT) {
							return cui::ct::error_report{"ERROR: Expected a set of int RGBA value."};
						}
						return styles_value_data{color_rgba{static_cast<uint8_t>(l[0].val_i),
															static_cast<uint8_t>(l[1].val_i),
															static_cast<uint8_t>(l[2].val_i)}};
					}
					break;
				}
				case fn_names_id__::rgba: {
					if (list.idx != 4 && list.idx != 1) {
						return cui::ct::error_report{"ERROR: Invalid amount of arguments; Supplied ",
													 cui::ct::num_to_string(list.idx)};
					}

					if (list.idx == 1) {
						if (l[0].active != type_enum::COLOR_RGBA) {
							return cui::ct::error_report{"ERROR: Expected a color RGBA value."};
						}
						return l[0];
					} else if (list.idx == 4) {
						if (l[0].active != type_enum::INT && l[1].active != type_enum::INT &&
							l[2].active != type_enum::INT && l[3].active != type_enum::INT)
						{
							return cui::ct::error_report{"ERROR: Expected a set of int RGBA value."};
						}
						return styles_value_data{color_rgba{static_cast<uint8_t>(l[0].val_i),
															static_cast<uint8_t>(l[1].val_i),
															static_cast<uint8_t>(l[2].val_i),
															static_cast<uint8_t>(l[3].val_i)}};
					}
					break;
				}
				case fn_names_id__::vec: {
					if (list.idx < 2) {
						return cui::ct::error_report{"ERROR: Expected two or more values in the vector. Received: ",
													 cui::ct::num_to_string(list.idx)};
					}
					switch (list.idx) {
						case 2: {
							if (l[0].active == type_enum::INT && l[1].active == type_enum::INT) {
								return styles_value_data{vec2{l[0].val_i, l[1].val_i}};
							} else if (l[0].active == type_enum::FLOAT && l[1].active == type_enum::FLOAT) {
								return styles_value_data{vec2{l[0].val_f, l[1].val_f}};
							} else {
								return cui::ct::error_report{
								  "ERROR: Expected the vector values to be int or float types."};
							}
						}
						case 3: {
							if (l[0].active == type_enum::INT && l[1].active == type_enum::INT &&
								l[2].active == type_enum::INT) {
								return styles_value_data{vec3{l[0].val_i, l[1].val_i, l[2].val_i}};
							} else if (l[0].active == type_enum::FLOAT && l[1].active == type_enum::FLOAT &&
									   l[2].active == type_enum::FLOAT) {
								return styles_value_data{vec3{l[0].val_f, l[1].val_f, l[2].val_f}};
							} else {
								return cui::ct::error_report{
								  "ERROR: Expected the vector values to be int or float types."};
							}
						}
						default: {
							if (l[0].active == type_enum::INT && l[1].active == type_enum::INT &&
								l[2].active == type_enum::INT && l[3].active == type_enum::INT)
							{
								return styles_value_data{vec4{l[0].val_i, l[1].val_i, l[2].val_i, l[3].val_i}};
							} else if (l[0].active == type_enum::FLOAT && l[1].active == type_enum::FLOAT &&
									   l[2].active == type_enum::FLOAT && l[3].active == type_enum::FLOAT)
							{
								return styles_value_data{vec4{l[0].val_f, l[1].val_f, l[2].val_f, l[3].val_f}};
							} else {
								return cui::ct::error_report{
								  "ERROR: Expected the vector values to be int or float types."};
							}
						} break;
					}
					// Next case
				}
			}

			return cui::ct::error_report{"ERROR: Invalid function index."};
		}

		static constexpr attr_valid_type sdvt_to_avt(styles_value_data::active_ sdvt) {
			using sdv_types = styles_value_data::active_;
			switch (sdvt) {
				case sdv_types::COLOR_RGBA: {
					return attr_valid_type::RGBA;
				}
				case sdv_types::FLOAT: {
					return attr_valid_type::FLOAT;
				}
				case sdv_types::INT: {
					return attr_valid_type::INT;
				}
				case sdv_types::INSTR: {
					return attr_valid_type::INSTR;
				}
				case sdv_types::VEC2: {
					return attr_valid_type::VEC2;
				}
				case sdv_types::VEC3: {
					return attr_valid_type::VEC3;
				}
				case sdv_types::VEC4: {
					return attr_valid_type::VEC4;
				}
			}

			// Impossible
			return attr_valid_type::INVALID;
		}

		static constexpr bool is_correct_value_for_attr_type(cui::ct::string const& type,
															 cui::ct::styles_value_data const& value) {
			uint8_t idx = val_type__.size();

			for (uint8_t i = 0; i < cui::attr_types__.size(); ++i) {
				if (type == cui::attr_types__[i]) {
					idx = i;
					break;
				}
			}

			auto const& active_type = val_type__[idx];

			return active_type & sdvt_to_avt(value.active);
		}

	};	  // namespace detail
public:
	cui::ct::string name;
	std::array<cui::ct::string, 128> events;
	std::array<attribute_data, 128> attributes;
	uint32_t events_size;
	uint32_t attributes_size;

	constexpr styles_data() : name(""), events{}, attributes{}, events_size(0), attributes_size(0) {}

	static constexpr cui::ct::variant<styles_value_data, cui::ct::error_report>
	parse_styles_value(cui::ct::styles_value_t const& sv) {
		auto const& str = sv;
		uint32_t i = 0;

		enum states : uint8_t
		{
			NONE = 0,
			INT,
			MINUS,
			FLOAT,
			PERCENTAGE,
			STRING,
			FUNCTION,
			FUNCTION_END
		} state = NONE;

		while (i < str.size) {
			auto ch = str[i];
			if (detail::is_digit(ch)) {
				switch (state) {
					case NONE: {
						state = INT;
						break;
					}
					case INT: {
						break;
					}
					case MINUS: {
						state = INT;
						break;
					}
					case FLOAT: {
						break;
					}
					case STRING: {
						break;
					}
					case FUNCTION: {
						error_report err{"ERROR: Missing end parenthesis to the function."};

						return err;
					}
					default: {
						error_report err{"ERROR: End of value expected."};

						return err;
					}
				}
			} else if (detail::is_minus(ch)) {
				switch (state) {
					case NONE: {
						state = MINUS;
						break;
					}
					default: {
						error_report err{"ERROR: Minus expected at the beginning of the value."};

						return err;
					}
				}
			} else if (detail::is_percent(ch)) {
				switch (state) {
					case INT: {
						state = PERCENTAGE;
						break;
					}
					case FLOAT: {
						state = PERCENTAGE;
						break;
					}
					case FUNCTION: {
						error_report err{"ERROR: Missing end parenthesis to the function."};

						return err;
					}
					default: {
						error_report err{"ERROR: Expected int or float value before percent sign."};

						return err;
					}
				}
			} else if (detail::is_point(ch)) {
				switch (state) {
					case NONE: {
						state = FLOAT;
						break;
					}
					case INT: {
						state = FLOAT;
						break;
					}
					case FUNCTION: {
						error_report err{"ERROR: Missing end parenthesis to the function."};

						return err;
					}
					default: {
						error_report err{"ERROR: Expected an int value or the point starting the value."};

						return err;
					}
				}
			} else if (detail::is_open_parenthesis(ch)) {
				switch (state) {
					case STRING: {
						state = FUNCTION;
						if (cui::ct::find_last(str, ')') != str.size) {
							state = FUNCTION_END;
						}

						i = str.size;

						break;
					}
					case FUNCTION: {
						error_report err{"ERROR: Missing end parenthesis to the function."};

						return err;
					}
					default: {
						error_report err{"ERROR: Expected function name."};

						return err;
					}
				}
			} else if (detail::is_closed_parenthesis(ch)) {
				switch (state) {
					case FUNCTION: {
						state = FUNCTION_END;
						break;
					}
					default: {
						error_report err{"ERROR: Expected a function."};

						return err;
					}
				}
			} else if (detail::is_valid_char(ch)) {
				switch (state) {
					case NONE: {
						state = STRING;
						break;
					}
					case STRING: {
						break;
					}
					case FUNCTION: {
						error_report err{"ERROR: Expected a function."};

						break;
					}
					default: {
						error_report err{"ERROR: Expected a char or an int."};

						return err;
					}
				}
			} else {
				error_report err{"ERROR: Invalid char in value."};

				return err;
			}
			++i;
		}

		switch (state) {
			case INT: {
				return detail::parse_int(str);
			}
			case FLOAT: {
				return detail::parse_float(str);
			}
			case PERCENTAGE: {
				return detail::parse_percentage(str);
			}
			case STRING: {
				return detail::lookup_string(str);
			}
			case FUNCTION_END: {
				auto const first_paren = cui::ct::find(str, '(');
				auto fn_name = cui::ct::string{str.str, first_paren};
				auto const fn_name_idx = detail::index_of_function_name(fn_name);
				if (fn_name_idx == fn_names__.size()) {
					return cui::ct::error_report{"ERROR: Function name invalid.\n~~~Name: ", fn_name};
				}
				auto const second_paren = cui::ct::find_last(str, ')');
				auto const arg_list = detail::parse_function_argument_list(
				  cui::ct::string{str.str + first_paren + 1, second_paren - first_paren - 1});
				if (arg_list.active == arg_list.active_::T2_) {
					return arg_list.type2;
				}

				auto const fn_processed_val = detail::lookup_function(fn_name_idx, arg_list.type1);
				if (fn_processed_val.active == fn_processed_val.active_::T2_) {
					return fn_processed_val.type2;
				}

				return fn_processed_val.type1;
			}
			case FUNCTION: {
				error_report err{"ERROR: Missing end parenthesis to the function."};
				break;
			}
		}

		return cui::ct::error_report{"ERROR: Unexpected error."};
	}

	static cui::ct::variant<styles_data, cui::ct::error_report> create(cui::ct::styles_definition def) {
		styles_data data;
		data.name = def.styles_identifier;
		data.events_size = def.event_idx;
		data.attributes_size = def.attr_idx;
		for (int i = 0; i < data.events_size; ++i) {
			data.events[i] = def.events[i];
		}

		for (int i = 0; i < data.attributes_size; ++i) {
			if (!detail::validate_attribute_type(def.attributes[i].type)) {
				return cui::ct::error_report{"ERROR: Attribute type is invalid. Provided: ", def.attributes[i].type};
			}
			data.attributes[i].type = def.attributes[i].type;
			auto const val = parse_styles_value(def.attributes[i].value);
			if (val.active == val.active_::T2_) {
				return val.type2;
			}

			if (!detail::is_correct_value_for_attr_type(def.attributes[i].type, val.type1)) {
				return cui::ct::error_report{"ERROR: Received wrong value for attribute."};
			}
			data.attributes[i].value = val.type1;
		}

		return data;
	}
};

}	 // namespace cui::ct

#endif	  // CUI_STYLES_DATA_HPP
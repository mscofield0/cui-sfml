#ifndef CUI_CT_STYLES_CONSTANTS_HPP
#define CUI_CT_STYLES_CONSTANTS_HPP

#include <utils/make_array.hpp>
#include <compile_time/string/string_view.hpp>
#include <compile_time/styles/detail/valid_attribute_types.hpp>
#include <compile_time/value_data.hpp>
#include <data_types/color.hpp>
#include <data_types/instruction.hpp>
#include <data_types/vec.hpp>

#include <aliases.hpp>

// clang-format off
namespace cui::ct::styles {

template <typename... Args>
constexpr auto make_sv_array(Args... args) -> std::array<StringView, sizeof...(Args)> {
	return {StringView{args}...};
}

template <typename T, typename... Args>
constexpr auto make_sv_array(T arg, Args... args) -> std::array<StringView, sizeof...(Args) + 1>{
	return {StringView{arg}, StringView{args}...};
}

// Enum class representing function names
enum class FunctionId : byte
{
	RGBA = 0,
	RGB,
	Vec
};

// Group: Attribute name <-> Attribute type

constexpr auto attribute_names =
    make_sv_array(
        "color",
        "x",
        "y",
        "width",
        "height"
    );

constexpr auto attribute_types = 
    make_array<u64>(
        ValidAttributeType::RGBA,
        ValidAttributeType::Int | ValidAttributeType::Float | ValidAttributeType::Instruction,
        ValidAttributeType::Int | ValidAttributeType::Float | ValidAttributeType::Instruction,
        ValidAttributeType::Int | ValidAttributeType::Float | ValidAttributeType::Instruction,
        ValidAttributeType::Int | ValidAttributeType::Float | ValidAttributeType::Instruction
    );


// Group: Function name <-> Function id

constexpr auto function_names =
    make_sv_array(
        "rgba",
        "vec"
    );

constexpr auto function_ids = 
    make_array<FunctionId>(
        FunctionId::RGBA,
        FunctionId::Vec
    );


// Group: String names <-> String values

constexpr auto string_names = 
    make_sv_array(
        "red",
        "green",
        "blue",
        "transparent",
        "left",
        "right"
    );

constexpr auto string_values = 
    make_array<ValueData>(
        ValueData{Color(255, 0, 0 )},
        ValueData{Color(0, 255, 0 )},
        ValueData{Color(0, 0, 255 )},
        ValueData{Color(0, 0, 0, 0)},
        ValueData{Instruction(Functions::Left)},
        ValueData{Instruction(Functions::Right)}
    );



}	 // namespace cui::styles

#endif	  // CUI_CT_STYLES_CONSTANTS_HPP
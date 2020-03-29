#ifndef CUI_STYLES_CONSTANTS_HPP
#define CUI_STYLES_CONSTANTS_HPP

#include <global/constants.hpp>
#include <global/make_array.hpp>
#include <compile_time/string_view.hpp>
#include <styles/valid_attribute_types.hpp>
#include <styles/function_ids.hpp>
#include <styles/data.hpp>
#include <data_types/color.hpp>
#include <data_types/instruction.hpp>
#include <data_types/vector.hpp>

// clang-format off
namespace cui::styles {

constexpr uint32_t MAX_NUMBER_OF_EVENTS_IN_A_STYLE = 128;
constexpr uint32_t MAX_NUMBER_OF_ATTRIBUTES_IN_A_STYLE = 128;

template <typename... Args>
constexpr auto make_sv_array(Args... args) -> std::array<StringView, sizeof...(Args)> {
	std::array<StringView, sizeof...(Args)> arr{cui::StringView{args}...};
	return arr;
}

template <typename T, typename... Args>
constexpr auto make_sv_array(T arg, Args... args) -> std::array<StringView, sizeof...(Args) + 1>{
	std::array<StringView, sizeof...(Args) + 1> arr{cui::StringView{arg}, cui::StringView{args}...};
	return arr;
}

// Group: Attribute name <-> Attribute type

constexpr auto attribute_names =
    make_sv_array(
        "color"
    );

constexpr auto attribute_types = 
    make_array<uint64_t>(
        ValidAttributeType::RGBA
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
        "contain"
    );

constexpr auto string_values = 
    make_array<ValueData>(
        ValueData{cui::Color{255, 0, 0}},
        ValueData{cui::Color{0, 255, 0}},
        ValueData{cui::Color{0, 0, 255}},
        ValueData{cui::Color{0, 0, 0, 0}},
        ValueData{cui::Instruction{cui::Instruction::Functions::Contain}}
    );

}	 // namespace cui::styles

#endif	  // CUI_STYLES_CONSTANTS_HPP
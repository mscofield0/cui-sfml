#ifndef CUI_UTILITY_HPP
#define CUI_UTILITY_HPP

#include <type_traits>
#include <array>

/// @file utility.hpp
/// @brief Utility and helper functions

using uint8_t = unsigned char;
using uint16_t = unsigned short;
using uint32_t = unsigned int;
using uint64_t = unsigned long long;
using int8_t = signed char;
using int16_t = signed short;
using int32_t = signed int;
using int64_t = signed long long;

#define STATIC_STRING_HOLDER(name) static constexpr const char name[] =
#define END_STATIC_STRING_HOLDER ;

namespace cui::util {

template <typename T, uint32_t N>
constexpr uint32_t arrsize(const T (&)[N]) {
	return N;
}

template <const char* str>
constexpr uint32_t strlen() {
	uint32_t current = 0;
	while (str[current] != 0) ++current;
	return current;
}

constexpr uint32_t strlen(const char* str) {
	uint32_t current = 0;
	while (str[current] != 0) ++current;
	return current;
}

constexpr int pow(int num, int exp) {
	int orig = num;
	if (exp == 0) return 1;
	if (exp == 1) return num;
	for (int i = 0; i < exp - 1; ++i) {
		num *= orig;
	}
	return num;
}

constexpr float pow(float num, int exp) {
	float orig = num;
	if (exp == 0) return 1;
	if (exp == 1) return num;
	for (int i = 0; i < exp - 1; ++i) {
		num *= orig;
	}
	return num;
}

template <typename Number, std::enable_if_t<std::is_integral_v<Number>>* = nullptr>
constexpr uint64_t abs(Number num) {
	uint64_t result = num;
	if (num < 0) result = num * -1;
	return result;
}

constexpr uint32_t num_of_whole_digits(int const num) {
	uint32_t nod = 1;
	while (num / pow(10, nod) != 0) ++nod;
	return nod;
}

template <typename Container, typename T>
constexpr void fill_array(Container& arr, T val) {
	for (int i = 0; i < arr.size(); ++i) {
		arr[i] = val;
	}
}

template <typename T, typename... Args>
constexpr std::array<T, sizeof...(Args)> make_array(Args... args) {
	std::array<T, sizeof...(Args)> arr{args...};
	return arr;
}

template <typename T, typename... Args>
constexpr std::array<T, sizeof...(Args) + 1> make_array(T arg, Args... args) {
	std::array<T, sizeof...(Args) + 1> arr{arg, args...};
	return arr;
}

/// @brief Checks if a character is a whitespace character
constexpr bool is_whitespace(uint8_t ch) {
	return ch == 9 || ch == 10 || ch == 11 || ch == 12 || ch == 13 || ch == 32 || ch == 133 || ch == 160;
}

}	 // namespace cui::util

#endif	  //CUI_UTILITY_HPP

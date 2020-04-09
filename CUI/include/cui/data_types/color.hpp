#ifndef CUI_CT_DATA_TYPES_COLOR_HPP
#define CUI_CT_DATA_TYPES_COLOR_HPP

#include <aliases.hpp>

namespace cui::data_types {

class Color
{
public:
	using color_type = u32;

	constexpr Color() : data_{0} {}
	constexpr Color(color_type data) : data_{data} {}
	constexpr Color(i32 r, i32 g, i32 b, i32 a = 255) : data_{to_color(r, g, b, a)} {}

	[[nodiscard]] constexpr auto data() noexcept -> color_type& {
		return data_;
	}

	[[nodiscard]] constexpr auto data() const noexcept -> color_type {
		return data_;
	}

	[[nodiscard]] constexpr auto red() const noexcept -> i32 {
		return (data() & (255 << 24)) >> 24;
	}

	[[nodiscard]] constexpr auto green() const noexcept -> i32 {
		return (data() & (255 << 16)) >> 16;
	}

	[[nodiscard]] constexpr auto blue() const noexcept -> i32 {
		return (data() & (255 << 8)) >> 8;
	}

	[[nodiscard]] constexpr auto alpha() const noexcept -> i32 {
		return data() & 255;
	}

	static const Color transparent, black, white;

private:
	static constexpr auto to_color(i32 r, i32 g, i32 b, i32 a) -> color_type {
		color_type data = a;
		data |= b << 8;
		data |= g << 16;
		data |= r << 24;
		return data;
	}

	color_type data_;
};

constexpr Color Color::transparent = Color(0), Color::black = Color(0, 0, 0), Color::white = Color(255, 255, 255);

}	 // namespace cui::data_types

#endif	  // CUI_CT_DATA_TYPES_COLOR_HPP
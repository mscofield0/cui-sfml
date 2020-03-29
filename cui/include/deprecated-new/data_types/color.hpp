#ifndef CUI_DATA_TYPES_COLOR_HPP
#define CUI_DATA_TYPES_COLOR_HPP

#include <global/constants.hpp>

namespace cui {

class Color
{
public:
	using color_type = uint32_t;

	static constexpr auto to_color(int r, int g, int b, int a) -> color_type {
		color_type data = a;
		data |= b << 8;
		data |= g << 16;
		data |= r << 24;
		return data;
	}

	constexpr Color() : data_{0} {}
	constexpr Color(color_type data) : data_{data} {}
	constexpr Color(int r, int g, int b, int a = 255) : data_{to_color(r, g, b, a)} {}

	[[nodiscard]] constexpr auto data() noexcept -> color_type& {
		return data_;
	}

	[[nodiscard]] constexpr auto data() const noexcept -> color_type {
		return data_;
	}

	[[nodiscard]] constexpr auto red() const noexcept -> int {
		return (data() & (255 << 24)) >> 24;
	}

	[[nodiscard]] constexpr auto green() const noexcept -> int {
		return (data() & (255 << 16)) >> 16;
	}

	[[nodiscard]] constexpr auto blue() const noexcept -> int {
		return (data() & (255 << 8)) >> 8;
	}

	[[nodiscard]] constexpr auto alpha() const noexcept -> int {
		return data() & 255;
	}

private:
	color_type data_;
};

}	 // namespace cui

#endif	  // CUI_DATA_TYPES_COLOR_HPP
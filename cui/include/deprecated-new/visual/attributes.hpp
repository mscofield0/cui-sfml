#ifndef CUI_VISUAL_ATTRIBUTES_HPP
#define CUI_VISUAL_ATTRIBUTES_HPP

#include <styles/data.hpp>
#include <styles/constants.hpp>
#include <data_types/color.hpp>

namespace cui {

using styles::ValueData;

class Attributes
{
public:
	constexpr Attributes() noexcept
		: x_(0), y_(0), width_(0), height_(0), background_(cui::Color(0)), text_color_(cui::Color(0, 0, 0)),
		  font_size_(0) {}

	auto x() noexcept -> ValueData& {
		return x_;
	}

	auto x() const noexcept -> const ValueData& {
		return x_;
	}

	auto y() noexcept -> ValueData& {
		return y_;
	}

	auto y() const noexcept -> const ValueData& {
		return y_;
	}

	auto width() noexcept -> ValueData& {
		return width_;
	}

	auto width() const noexcept -> const ValueData& {
		return width_;
	}

	auto height() noexcept -> ValueData& {
		return height_;
	}

	auto height() const noexcept -> const ValueData& {
		return height_;
	}

	auto background() noexcept -> ValueData& {
		return background_;
	}

	auto background() const noexcept -> const ValueData& {
		return background_;
	}

	auto text_color() noexcept -> ValueData& {
		return text_color_;
	}

	auto text_color() const noexcept -> const ValueData& {
		return text_color_;
	}

	auto font_size() noexcept -> ValueData& {
		return font_size_;
	}

	auto font_size() const noexcept -> const ValueData& {
		return font_size_;
	}

protected:
	ValueData x_;
	ValueData y_;
	ValueData width_;
	ValueData height_;
	ValueData background_;
	ValueData text_color_;
	ValueData font_size_;
};

}	 // namespace cui

#endif	  // CUI_VISUAL_ATTRIBUTES_HPP
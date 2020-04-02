#ifndef CUI_VISUAL_ATTRIBUTES_HPP
#define CUI_VISUAL_ATTRIBUTES_HPP

#include <value_data.hpp>
#include <compile_time/data_types/color.hpp>

namespace cui {

class Attributes
{
public:
	Attributes() noexcept
		: x_(0), y_(0), width_(0), height_(0), background_(cui::Color(0)), text_color_(cui::Color(0, 0, 0)),
		  font_size_(0) {}

	[[nodiscard]] auto x() noexcept -> ValueData& {
		return x_;
	}

	[[nodiscard]] auto x() const noexcept -> const ValueData& {
		return x_;
	}

	[[nodiscard]] auto y() noexcept -> ValueData& {
		return y_;
	}

	[[nodiscard]] auto y() const noexcept -> const ValueData& {
		return y_;
	}

	[[nodiscard]] auto width() noexcept -> ValueData& {
		return width_;
	}

	[[nodiscard]] auto width() const noexcept -> const ValueData& {
		return width_;
	}

	[[nodiscard]] auto height() noexcept -> ValueData& {
		return height_;
	}

	[[nodiscard]] auto height() const noexcept -> const ValueData& {
		return height_;
	}

	[[nodiscard]] auto background() noexcept -> ValueData& {
		return background_;
	}

	[[nodiscard]] auto background() const noexcept -> const ValueData& {
		return background_;
	}

	[[nodiscard]] auto text_color() noexcept -> ValueData& {
		return text_color_;
	}

	[[nodiscard]] auto text_color() const noexcept -> const ValueData& {
		return text_color_;
	}

	[[nodiscard]] auto font_size() noexcept -> ValueData& {
		return font_size_;
	}

	[[nodiscard]] auto font_size() const noexcept -> const ValueData& {
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
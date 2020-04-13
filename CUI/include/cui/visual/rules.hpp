#ifndef CUI_RULES_HPP
#define CUI_RULES_HPP

#include <aliases.hpp>

namespace cui {

class Rules
{
public:
	Rules() noexcept : enabled_rules_(0) {}

	void set_x_rule(bool val) noexcept {
		set_nth_bit(val, ID::X);
	}

	void set_y_rule(bool val) noexcept {
		set_nth_bit(val, ID::Y);
	}

	void set_width_rule(bool val) noexcept {
		set_nth_bit(val, ID::Width);
	}

	void set_height_rule(bool val) noexcept {
		set_nth_bit(val, ID::Height);
	}

	void set_text_position_rule(bool val) noexcept {
		set_nth_bit(val, ID::TextPosition);
	}

	[[nodiscard]] bool x_rule() const noexcept {
		return get_nth_bit(ID::X);
	}

	[[nodiscard]] bool y_rule() const noexcept {
		return get_nth_bit(ID::Y);
	}

	[[nodiscard]] bool width_rule() const noexcept {
		return get_nth_bit(ID::Width);
	}

	[[nodiscard]] bool height_rule() const noexcept {
		return get_nth_bit(ID::Height);
	}

	[[nodiscard]] bool text_position_rule() const noexcept {
		return get_nth_bit(ID::TextPosition);
	}

protected:
	void set_nth_bit(bool val, int n) noexcept {
		if (val) {
			enabled_rules_ |= (1 << n);
			return;
		}
		enabled_rules_ &= ~(1 << n);
	}

	bool get_nth_bit(int n) const noexcept {
		return (enabled_rules_ & (1 << n));
	}

	enum ID : u32
	{
		X = 0,
		Y,
		Width,
		Height,
		TextPosition
	};

	u32 enabled_rules_;
};

}	 // namespace cui

#endif	  // CUI_RULES_HPP
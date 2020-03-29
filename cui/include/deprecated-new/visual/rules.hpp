#ifndef CUI_VISUAL_RULES_HPP
#define CUI_VISUAL_RULES_HPP

#include <global/constants.hpp>

namespace cui {

class Rules
{
public:
	constexpr Rules() noexcept : enabled_rules_(0) {}

	void set_x_rule(bool val) noexcept {
		set_nth_bit(val, 0);
	}

	void set_y_rule(bool val) noexcept {
		set_nth_bit(val, 1);
	}

	void set_width_rule(bool val) noexcept {
		set_nth_bit(val, 2);
	}

	void set_height_rule(bool val) noexcept {
		set_nth_bit(val, 3);
	}

	bool x_rule() const noexcept {
		return get_nth_bit(ID::X);
	}

	bool y_rule() const noexcept {
		return get_nth_bit(ID::Y);
	}

	bool width_rule() const noexcept {
		return get_nth_bit(ID::Width);
	}

	bool height_rule() const noexcept {
		return get_nth_bit(ID::Height);
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

	enum ID : uint32_t
	{
		X = 0,
		Y,
		Width,
		Height
	};

	uint32_t enabled_rules_;
};

}	 // namespace cui

#endif	  // CUI_VISUAL_RULES_HPP
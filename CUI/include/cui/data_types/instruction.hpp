#ifndef CUI_CT_DATA_TYPES_INSTRUCTION_HPP
#define CUI_CT_DATA_TYPES_INSTRUCTION_HPP

#include <aliases.hpp>

namespace cui::data_types {

enum class Functions : u8
{
	TopLeft = 0,
	Top,
	TopRight,
	Left,
	Center,
	Right,
	BottomLeft,
	Bottom,
	BottomRight
};

class Instruction
{
public:
	constexpr Instruction(Functions chosen) : active_(chosen) {}

	[[nodiscard]] constexpr auto active() noexcept -> Functions& {
		return active_;
	}

	[[nodiscard]] constexpr auto active() const noexcept -> Functions {
		return active_;
	}

private:
	Functions active_;
};

}	 // namespace cui::data_types

#endif	  // CUI_CT_DATA_TYPES_INSTRUCTION_HPP
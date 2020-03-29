#ifndef CUI_CT_DATA_TYPES_INSTRUCTION_HPP
#define CUI_CT_DATA_TYPES_INSTRUCTION_HPP

#include <aliases.hpp>

namespace cui::ct::data_types {

class Instruction
{
public:
	enum class Functions : byte
	{
		Contain
	};

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

}	 // namespace cui::ct::data_types

#endif	  // CUI_CT_DATA_TYPES_INSTRUCTION_HPP
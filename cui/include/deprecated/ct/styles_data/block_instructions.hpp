#ifndef CUI_BLOCK_INSTRUCTIONS_HPP
#define CUI_BLOCK_INSTRUCTIONS_HPP

#include "../../utility/utility.hpp"

namespace cui {


struct block_instruction
{
	enum : uint32_t
	{
		CONTAIN = 0
	};
	cui::block_instructions instr;

	constexpr block_instruction(uint8_t instr_code) : instr{static_cast<cui::block_instructions>(instr_code)} {}
};

}	 // namespace cui

#endif	  // CUI_BLOCK_INSTRUCTIONS_HPP
#ifndef CUI_DATA_TYPES_INSTRUCTION_HPP
#define CUI_DATA_TYPES_INSTRUCTION_HPP

#include <global/constants.hpp>

namespace cui {

class Instruction
{
public:
	enum class Functions
	{
		Contain
	};

	constexpr Instruction(Functions chosen) : active_(chosen) {}

	template <typename RenderContext>
	void Execute(RenderContext& ctx) const {}

private:
	Functions active_;
};

}	 // namespace cui

#endif	  // CUI_DATA_TYPES_INSTRUCTION_HPP
#ifndef CUI_DATA_TYPES_HPP
#define CUI_DATA_TYPES_HPP

#include <aliases.hpp>

namespace cui::data_types {

enum class DataTypes : byte
{
	None = 0,
	Color,
	Float,
	Int,
	Vec2,
	Vec3,
	Vec4,
	Instruction,
	String
};

}	 // namespace cui::data_types

#endif	  // CUI_DATA_TYPES_HPP
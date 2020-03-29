#ifndef CUI_STYLES_DETAIL_VALUE_TYPE_HPP
#define CUI_STYLES_DETAIL_VALUE_TYPE_HPP

namespace cui::styles {

enum class ValueType : unsigned char
{
	Int,
	Float,
	Percentage,
	String,
	Color,
	VectorInt,
	VectorFloat,
	Instruction
};

}	 // namespace cui::styles

#endif	  // CUI_STYLES_DETAIL_VALUE_TYPE_HPP
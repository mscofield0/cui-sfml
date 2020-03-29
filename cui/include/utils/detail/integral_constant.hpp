#ifndef CUI_INTEGRAL_CONSTANT_HPP
#define CUI_INTEGRAL_CONSTANT_HPP

namespace cui {

template <typename Type, Type Value>
struct integral_constant
{
	static constexpr Type value = Value;
	using value_type = Type;
	using type = integral_constant<Type, Value>;
};

using true_type = integral_constant<bool, true>;
using false_type = integral_constant<bool, false>;

}	 // namespace cui

#endif	  //CUI_INTEGRAL_CONSTANT_HPP
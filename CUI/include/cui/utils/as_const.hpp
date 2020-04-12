#ifndef CUI_AS_CONST_HPP
#define CUI_AS_CONST_HPP

namespace cui {

template <typename Type>
auto as_const(Type& v) noexcept -> const Type& {
	return v;
}

}	 // namespace cui

#endif	  // CUI_AS_CONST_HPP
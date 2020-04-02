#ifndef CUI_REMOVE_REFERENCE_HPP
#define CUI_REMOVE_REFERENCE_HPP

namespace cui {

template <typename Type>
struct remove_reference
{
	using type = Type;
};

template <typename Type>
struct remove_reference<Type&>
{
	using type = Type;
};

template <typename Type>
struct remove_reference<Type&&>
{
	using type = Type;
};

}	 // namespace cui

#endif	  // CUI_REMOVE_REFERENCE_HPP
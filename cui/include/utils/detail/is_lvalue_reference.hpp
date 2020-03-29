#ifndef CUI_IS_LVALUE_REFERENCE_HPP
#define CUI_IS_LVALUE_REFERENCE_HPP

#include <utils/detail/integral_constant.hpp>

namespace cui {

template <typename>
struct is_lvalue_reference : public false_type
{};

template <typename Type>
struct is_lvalue_reference<Type&> : public true_type
{};

}	 // namespace cui

#endif
#ifndef CUI_DECLVAL_IMPL_HPP
#define CUI_DECLVAL_IMPL_HPP

namespace cui {

template <typename Type, typename Ret = Type&&>
auto declval_impl(int) -> Ret;

template <typename Type>
auto declval_impl(long) -> Type;

}	 // namespace cui

#endif	  // CUI_DECLVAL_IMPL_HPP
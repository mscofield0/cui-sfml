#ifndef CUI_DECLVAL_HPP
#define CUI_DECLVAL_HPP

#include <utils/detail/declval_impl.hpp>

namespace cui {

template <typename Type>
auto declval() noexcept -> decltype(declval_impl<Type>(0));

}

#endif	  //  CUI_DECLVAL_HPP
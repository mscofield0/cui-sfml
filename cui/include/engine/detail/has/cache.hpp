#ifndef CUI_ENGINE_HAS_CACHE_HPP
#define CUI_ENGINE_HAS_CACHE_HPP

#include <utils/void_t.hpp>
#include <utils/detail/integral_constant.hpp>

namespace cui {

template <typename Type, typename = void>
struct HasCache : false_type
{};

template <typename Type>
struct HasCache<Type, void_t<decltype((void)Type::cache_)>> : true_type
{};

}	 // namespace cui

#endif	  // CUI_ENGINE_HAS_CACHE_HPP
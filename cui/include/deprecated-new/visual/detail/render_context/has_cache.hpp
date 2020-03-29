#ifndef CUI_VISUAL_DETAIL_HAS_CACHE_HPP
#define CUI_VISUAL_DETAIL_HAS_CACHE_HPP

#include <type_traits>

namespace cui::visual::detail__ {

template <typename T, typename = void>
struct HasCache : std::false_type
{};

template <typename T>
struct HasCache<T, std::void_t<decltype(std::declval<T&>().cache())>> : std::true_type
{};

}	 // namespace cui::visual::detail__

#endif	  // CUI_VISUAL_DETAIL_HAS_CACHE_HPP
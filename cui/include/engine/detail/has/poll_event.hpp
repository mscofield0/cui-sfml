#ifndef CUI_ENGINE_HAS_POLL_EVENT_HPP
#define CUI_ENGINE_HAS_POLL_EVENT_HPP

#include <utils/void_t.hpp>
#include <utils/declval.hpp>
#include <utils/detail/integral_constant.hpp>

namespace cui {

template <typename Type, typename = void>
struct HasPollEvent : false_type
{};

template <typename Type>
struct HasPollEvent<Type, void_t<decltype(declval<Type&>().poll_event())>> : true_type
{};

}	 // namespace cui

#endif	  // CUI_ENGINE_HAS_CACHE_HPP
#ifndef CUI_SFML_ANY_IS_SAME_HPP
#define CUI_SFML_ANY_IS_SAME_HPP

#include <type_traits>

namespace cui {

template <typename T, typename... Us>
struct is_any
{
	static constexpr bool value = std::disjunction_v<std::is_same<T, Us>...>;
};

template <typename T, typename... Us>
constexpr bool is_any_v = is_any<T, Us...>::value;

}	 // namespace cui

#endif	  // CUI_SFML_ANY_IS_SAME_HPP
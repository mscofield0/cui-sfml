#ifndef CUI_SIZE_INFORMATION_HPP
#define CUI_SIZE_INFORMATION_HPP

#include <type_traits>

#include <aliases.hpp>

namespace cui::ct::detail {

template <typename T, typename Enabler = void>
struct SizeInformation
{
	static constexpr u32 size = 0;
};

template <typename T>
struct SizeInformation<T, std::enable_if_t<std::is_integral_v<T>>>
{
	static constexpr u32 size = std::numeric_limits<T>::digits10 + 1;
};

template <u32 N>
struct SizeInformation<const char (&)[N]>
{
	static constexpr u32 size = N - 1;
};

template <unsigned int StrSize, typename... Args>
static constexpr u32 calculate_buffer_size() {
	return (1 + StrSize + (SizeInformation<Args>::size + ... + 0));
}

}	 // namespace cui::ct::detail

#endif	  // CUI_SIZE_INFORMATION_HPP
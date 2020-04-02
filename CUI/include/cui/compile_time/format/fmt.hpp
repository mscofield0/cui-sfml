#ifndef CUI_CT_FMT_HPP
#define CUI_CT_FMT_HPP

#include <compile_time/format/format.hpp>
#include <compile_time/format/detail/size_information.hpp>
#include <compile_time/format/detail/consume.hpp>
#include <compile_time/stream/stream.hpp>
#include <compile_time/string/string_view.hpp>
#include <utils/move.hpp>
#include <aliases.hpp>

namespace cui::ct {

template <u32 StrSize, typename... Args>
constexpr auto fmt(const char (&s)[StrSize], Args&&... args)
  -> Format<detail::calculate_buffer_size<StrSize, Args...>()> {
	constexpr auto size = detail::calculate_buffer_size<StrSize, Args...>();
	Format<size> buffer;
	StringView str{s};
	CharStream stream{str};
	((detail::consume_until(buffer, stream), buffer << cui::move(args)), ...);
	detail::consume_all(buffer, stream);
	buffer.append(0);
	return buffer;
}

template <u32 MaxSize, u32 StrSize, typename... Args>
constexpr auto fmt_s(const char (&s)[StrSize], Args&&... args) -> Format<MaxSize> {
	constexpr auto size = MaxSize;
	Format<size> buffer;
	StringView str{s};
	CharStream stream{str};
	((detail::consume_until(buffer, stream), buffer << cui::move(args)), ...);
	detail::consume_all(buffer, stream);
	buffer.append(0);
	return buffer;
}

}	 // namespace cui::ct

#endif	  // CUI_CT_FMT_HPP
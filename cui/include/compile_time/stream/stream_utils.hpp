#ifndef CUI_CT_STREAM_UTILS_HPP
#define CUI_CT_STREAM_UTILS_HPP

#include <utility>

#include <compile_time/string/string_view.hpp>
#include <compile_time/stream/stream.hpp>

namespace cui::ct {

template <typename CharT>
constexpr BasicStringView<CharT>
stream_substr(const Stream<CharT>& stream, typename Stream<CharT>::iterator from, typename Stream<CharT>::iterator to) {
	return stream.data().substr(std::distance(stream.begin(), from), std::distance(from, to));
}

template <typename CharT>
constexpr std::pair<BasicStringView<CharT>, char> consume(Stream<CharT>& stream, BasicStringView<CharT> delims) {
	using traits_t = typename BasicStringView<CharT>::traits_type;

	auto start_it = stream.current();
	CharT delim = CharT{};
	while (stream.good()) {
		const auto it = stream.current();
		for (const auto ch : delims) {
			if (traits_t::eq(*it, ch)) {
				delim = ch;
				break;
			}
		}
		if (delim != CharT{}) {
			stream.next();
			return {stream_substr(stream, start_it, it), delim};
		}
		stream.next();
	}
	return {stream_substr(stream, start_it, stream.end()), 0};
}

template <typename CharT>
constexpr BasicStringView<CharT> consume_line(Stream<CharT>& stream) {
	auto start_it = stream.current();
	enum class Newlines
	{
		NoNewline = 0,
		PlainNewline,
		NewlineCarriage,
		CarriageNewline
	};

	while (stream.good()) {
		const auto it = stream.current();

		Newlines sw = Newlines::NoNewline;
		if (*it == '\n') sw = Newlines::PlainNewline;
		if (stream.data().substr(stream.distance()).starts_with("\n\r")) sw = Newlines::NewlineCarriage;
		if (stream.data().substr(stream.distance()).starts_with("\r\n")) sw = Newlines::CarriageNewline;
		if (sw != Newlines::NoNewline) {
			if (sw == Newlines::PlainNewline) stream.next();
			else
			{
				stream.next();
				stream.next();
			}

			return stream_substr(stream, start_it, it);
		}
		stream.next();
	}
	return stream_substr(stream, start_it, stream.end());
}

}	 // namespace cui::ct

#endif	  // CUI_PARSE_UTILS_STREAM_UTILS_HPP
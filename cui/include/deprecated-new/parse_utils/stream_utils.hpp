#ifndef CUI_PARSE_UTILS_STREAM_UTILS_HPP
#define CUI_PARSE_UTILS_STREAM_UTILS_HPP

#include <utility>
#include <string_view>
#include <parse_utils/stream.hpp>

namespace cui {

/// \brief Gets the \see Stream underlying \a cui::BasicStringView<CharT,Traits> substring
///			from the two given iterators
/// \param stream A \see Stream object from which to create the substring
/// \param from A \a cui::BasicStringView<CharT,Traits>::iterator indicating the beginning of the substring
/// \param to A \a cui::BasicStringView<CharT,Traits>::iterator indicating the end of the substring
/// \return Returns a \a cui::StringView
template <typename CharT, typename Traits = BasicCharTraits<CharT>>
constexpr cui::StringView stream_substr(Stream<CharT, Traits> const& stream, typename Stream<CharT, Traits>::sv_iterator from, typename Stream<CharT, Traits>::sv_iterator to) {
	return stream.data().substr(std::distance(stream.begin(), from), std::distance(from, to));
}

/// \brief Advances the \see Stream object until it hits one of the specified delimiters
/// \param stream A \see Stream object from which to consume
/// \param delims A \a cui::BasicStringView<CharT,Traits> that holds the delimiter characters
/// \return Returns a \a std::pair<cui::BasicStringView<CharT,Traits>, char> where \a cui::BasicStringView<CharT,Traits> is the
/// 		consumed substring and \a CharT is the delimiter that stopped further consumption
template <typename CharT, typename Traits = BasicCharTraits<CharT>>
constexpr std::pair<cui::BasicStringView<CharT, Traits>, char> consume(Stream<CharT, Traits>& stream, cui::BasicStringView<CharT, Traits> delims) {
	auto start_it = stream.current();
	CharT delim = CharT{};
	while (stream.good()) {
		const auto it = stream.current();
		for (const auto ch : delims) {
			if (Traits::eq(*it, ch)) {
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

/// \brief Advances the \see Stream object until it hits a newline character
/// \param stream A \see Stream object from which to consume
/// \return Returns a \a cui::BasicStringView<CharT,Traits> that is the consumed substring
template <typename CharT, typename Traits = BasicCharTraits<CharT>>
constexpr cui::BasicStringView<CharT, Traits> consume_line(Stream<CharT, Traits>& stream) {
	auto start_it = stream.current();
	enum class Newlines {
		NoNewline = 0,
		PlainNewline,
		NewlineCarriage,
		CarriageNewline
	};

	while (stream.good()) {
		const auto it = stream.current();

		Newlines sw = Newlines::NoNewline;
		if(*it == '\n') sw = Newlines::PlainNewline;
		if(stream.data().substr(stream.distance()).starts_with("\n\r")) sw = Newlines::NewlineCarriage;
		if(stream.data().substr(stream.distance()).starts_with("\r\n")) sw = Newlines::CarriageNewline;
		if (sw != Newlines::NoNewline) {
			if(sw == Newlines::PlainNewline) stream.next();
			else {
				stream.next();
				stream.next();
			}

			return stream_substr(stream, start_it, it);
		}
		stream.next();
	}
	return stream_substr(stream, start_it, stream.end());
}

}	 // namespace cui

#endif	  // CUI_PARSE_UTILS_STREAM_UTILS_HPP
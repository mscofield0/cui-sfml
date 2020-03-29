#ifndef CUI_STATIC_STRING_STREAM_HPP
#define CUI_STATIC_STRING_STREAM_HPP

#include "../utility/utility.hpp"
#include "string.hpp"

namespace cui::ct {

struct static_string_stream
{
	const char* str;
	uint32_t current;
	uint32_t size;
	uint8_t state;

	constexpr explicit static_string_stream(const char* str)
		: str(str), current(0), size(cui::util::strlen(str)), state(0) {}

	[[nodiscard]] constexpr const char* peek() noexcept {
		if (current + 1 == size) {
			state = 1;
			return nullptr;
		}
		return str + current + 1;
	}

	constexpr char const* next() noexcept {
		if (peek() == nullptr) {
			return nullptr;
		}
		++current;
		return str + current;
	}

	[[nodiscard]] constexpr bool good() const noexcept {
		return !state;
	}
};

constexpr std::pair<cui::ct::string, char> consume(cui::ct::static_string_stream& stream, cui::ct::string delims) {
	const char* start = stream.str + stream.current;
	const uint32_t size = stream.current;
	char delim = 0;
	while (stream.good()) {
		auto const t = *(stream.str + stream.current);
		for (uint32_t i = 0; i < delims.size; ++i) {
			if (t == delims[i]) {
				delim = t;
				break;
			}
		}
		if (delim != 0) {
			stream.next();
			return {cui::ct::string{start, stream.current - size - 1}, delim};
		}
		stream.next();
	}
	return {cui::ct::string{start, stream.current - size}, 0};
}

constexpr cui::ct::string consume_line(cui::ct::static_string_stream& stream) {
	const char* start = stream.str + stream.current;
	const uint32_t size = stream.current;
	while (stream.good()) {
		auto const t = *(stream.str + stream.current);
		if (t == '\n') {
			auto ptr = stream.next();
			if (ptr == nullptr) {
				return {start, stream.current - size};
			}
			return {start, stream.current - size - 1};
		}
		stream.next();
	}
	return {start, stream.size - size};
}

}	 // namespace cui::ct

#endif	  //CUI_STATIC_STRING_STREAM_HPP

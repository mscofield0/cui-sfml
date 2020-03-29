#ifndef CUI_CT_FMT_CONSUME_HPP
#define CUI_CT_FMT_CONSUME_HPP

#include <compile_time/format/format.hpp>
#include <compile_time/stream/stream.hpp>
#include <aliases.hpp>

namespace cui::ct::detail {

template <u32 Size>
constexpr void consume_until(Format<Size>& buffer, CharStream& stream) {
	while (stream.good()) {
		const auto consumed = *stream.current();
		if (consumed == '{') {
			const auto peeked_it = stream.peek();
			if (peeked_it != stream.end() && *peeked_it == '}') {
				stream.next();
				stream.next();
				return;
			}
		}
		buffer.append(consumed);
		stream.next();
	}
}

template <u32 Size>
constexpr void consume_all(Format<Size>& buffer, CharStream& stream) {
	while (stream.good()) {
		buffer.append(*stream.current());
		stream.next();
	}
}

}	 // namespace cui::ct::detail

#endif	  // CUI_CT_FMT_CONSUME_HPP
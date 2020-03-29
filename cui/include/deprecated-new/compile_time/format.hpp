#ifndef CUI_COMPILE_TIME_FORMAT_HPP
#define CUI_COMPILE_TIME_FORMAT_HPP

#include <limits>
#include <cstdint>

#include <compile_time/static_vector.hpp>
#include <compile_time/string_view.hpp>
#include <compile_time/log10.hpp>
#include <compile_time/abs.hpp>
#include <compile_time/pow.hpp>
#include <parse_utils/stream.hpp>

namespace cui::ct {

template <uint64_t N>
class Format
{
public:
	using size_type = uint64_t;
	using value_type = static_vector<char, N>;
	using element_type = typename value_type::value_type;
	using reference = value_type&;
	using const_reference = const value_type&;
	using iterator = typename value_type::iterator;
	using const_iterator = typename value_type::const_iterator;

	constexpr size_type size() const noexcept {
		return N;
	}

	constexpr reference data() noexcept {
		return data_;
	}

	constexpr const_reference data() const noexcept {
		return data_;
	}

	constexpr void append(const element_type& el) {
		data_.push_back(el);
	}

	constexpr void append(element_type&& el) {
		data_.push_back(std::move(el));
	}

private:
	static_vector<char, N> data_;
};

namespace fmt {

struct detail__
{
	template <typename T, typename Enabler = void>
	struct SizeInformation
	{
		static constexpr uint64_t size = 0;
	};

	template <typename T>
	struct SizeInformation<T, std::enable_if_t<std::is_arithmetic_v<T>, void>>
	{
		static constexpr uint64_t size = std::numeric_limits<T>::digits10;
	};

	template <size_t N>
	struct SizeInformation<const char (&)[N]>
	{
		static constexpr uint64_t size = N - 1;
	};

	template <uint64_t StrSize, typename... Args>
	static constexpr uint64_t calculate_buffer_size() {
		return (StrSize + (SizeInformation<Args>::size + ... + 0));
	}

	template <uint64_t Size>
	static constexpr void consume_until(Format<Size>& buffer, CharStream& stream) {
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

	template <uint64_t Size>
	static constexpr void consume_all(Format<Size>& buffer, CharStream& stream) {
		while (stream.good()) {
			buffer.append(*stream.current());
			stream.next();
		}
	}

	template <uint64_t Size>
	static constexpr void parse_into(Format<Size>& buffer, int64_t x) {
		const auto minus = x < 0 ? true : false;
		x = cui::abs(x);
		auto digits = cui::log10(x) + 1;
		if (minus) buffer.append('-');
		do {
			const uint16_t digit = x / pow(10, digits - 1);
			const auto div = static_cast<uint64_t>(pow(10, --digits));
			x %= div;
			buffer.append(digit + '0');
		} while (x > 0);
	}

	template <uint64_t Size>
	static constexpr void parse_into(Format<Size>& buffer, uint64_t x) {
		auto digits = cui::log10(x) + 1;
		do {
			const uint16_t digit = x / pow(10, digits - 1);
			const auto div = static_cast<uint64_t>(pow(10, --digits));
			x %= div;
			buffer.append(digit + '0');
		} while (x > 0);
	}

	template <uint64_t Size>
	static constexpr void parse_into(Format<Size>& buffer, StringView x) {
		for (const auto ch : x) buffer.append(ch);
	}

	template <uint64_t Size, uint64_t N>
	static constexpr void parse_into(Format<Size>& buffer, const char (&s)[N]) {
		uint64_t i = 0;
		while (i < N - 1) {
			buffer.append(s[i++]);
		}
	}
};

}	 // namespace fmt

template <uint64_t StrSize, typename... Args>
constexpr Format<fmt::detail__::calculate_buffer_size<StrSize, Args...>()>
create_formatted_string(const char (&s)[StrSize], Args&&... args) {
	constexpr auto size = fmt::detail__::calculate_buffer_size<StrSize, Args...>();
	Format<size> buffer;
	StringView str{s};
	CharStream stream{str};
	((fmt::detail__::consume_until(buffer, stream), fmt::detail__::parse_into(buffer, std::move(args))), ...);
	fmt::detail__::consume_all(buffer, stream);
	buffer.append(0);
	return buffer;
}

template <uint64_t MaxSize, uint64_t StrSize, typename... Args>
constexpr Format<MaxSize> create_formatted_string_with_max_size(const char (&s)[StrSize], Args&&... args) {
	constexpr auto size = MaxSize;
	Format<size> buffer;
	StringView str{s};
	CharStream stream{str};
	((fmt::detail__::consume_until(buffer, stream), fmt::detail__::parse_into(buffer, std::move(args))), ...);
	fmt::detail__::consume_all(buffer, stream);
	buffer.append(0);
	return buffer;
}

}	 // namespace cui::ct

#endif	  // CUI_COMPILE_TIME_FORMAT_HPP
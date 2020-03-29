#ifndef CUI_GLOBAL_FORMAT_HPP
#define CUI_GLOBAL_FORMAT_HPP

#include <limits>
#include <cstdint>

#include <string>
#include <compile_time/string_view.hpp>
#include <compile_time/log10.hpp>
#include <compile_time/abs.hpp>
#include <compile_time/pow.hpp>
#include <parse_utils/stream.hpp>

namespace cui {

class Format
{
public:
	using size_type = uint64_t;
	using value_type = std::string;
	using element_type = typename value_type::value_type;
	using reference = value_type&;
	using const_reference = const value_type&;
	using iterator = typename value_type::iterator;
	using const_iterator = typename value_type::const_iterator;

	[[nodiscard]] auto size() const noexcept -> size_type {
		return data_.size();
	}

	[[nodiscard]] auto data() noexcept -> reference {
		return data_;
	}

	[[nodiscard]] auto data() const noexcept -> const_reference {
		return data_;
	}

	void append(const element_type& el) {
		data_.append(1, el);
	}

	void append(element_type&& el) {
		data_.append(1, std::move(el));
	}

private:
	std::string data_;
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

	template <typename T, typename = void>
	struct HasSize : std::false_type
	{};

	template <typename T>
	struct HasSize<T, std::void_t<decltype(std::declval<T>().size())>> : std::true_type
	{};

	template <typename T>
	static uint64_t strsize(const T& str) {
		if constexpr (HasSize<T>::value) return str.size();
		return 0;
	}

	template <uint64_t StrSize, typename... Args>
	static uint64_t calculate_buffer_size() {
		return (StrSize + (SizeInformation<Args>::size + ... + 0));
	}

	static void consume_until(Format& buffer, CharStream& stream) {
		while (stream.good()) {
			const auto consumed = *stream.current();
			if (consumed == '{') {
				const auto peeked_it = stream.peek();
				if (peeked_it == stream.end()) return;
				if (*peeked_it == '}') {
					stream.next();
					stream.next();
					return;
				}
			}
			buffer.append(consumed);
			stream.next();
		}
	}

	static void consume_all(Format& buffer, CharStream& stream) {
		while (stream.good()) {
			buffer.append(*stream.current());
			stream.next();
		}
	}

	static void parse_into(Format& buffer, int64_t x) {
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

	static void parse_into(Format& buffer, uint64_t x) {
		auto digits = cui::log10(x) + 1;
		do {
			const uint16_t digit = x / pow(10, digits - 1);
			const auto div = static_cast<uint64_t>(pow(10, --digits));
			x %= div;
			buffer.append(digit + '0');
		} while (x > 0);
	}

	static void parse_into(Format& buffer, StringView x) {
		for (const auto ch : x) buffer.append(ch);
	}

	template <uint64_t N>
	static void parse_into(Format& buffer, const char (&s)[N]) {
		uint64_t i = 0;
		while (i < N - 1) {
			buffer.append(s[i++]);
		}
	}
};

}	 // namespace fmt

template <uint64_t StrSize, typename... Args>
Format create_formatted_string(const char (&s)[StrSize], Args&&... args) {
	auto size = fmt::detail__::calculate_buffer_size<StrSize, Args...>();
	size += (fmt::detail__::strsize(std::move(args)) + ... + 0);
	Format buffer;
	buffer.data().reserve(size);
	StringView str{s};
	CharStream stream{str};
	((fmt::detail__::consume_until(buffer, stream), fmt::detail__::parse_into(buffer, std::move(args))), ...);
	fmt::detail__::consume_all(buffer, stream);
	buffer.append(0);
	return buffer;
}

template <uint64_t MaxSize, uint64_t StrSize, typename... Args>
constexpr Format create_formatted_string_with_max_size(const char (&s)[StrSize], Args&&... args) {
	constexpr auto size = MaxSize;
	Format buffer;
	buffer.data().reserve(size);
	StringView str{s};
	CharStream stream{str};
	((fmt::detail__::consume_until(buffer, stream), fmt::detail__::parse_into(buffer, std::move(args))), ...);
	fmt::detail__::consume_all(buffer, stream);
	buffer.append(0);
	return buffer;
}

}	 // namespace cui

#endif	  // CUI_GLOBAL_FORMAT_HPP
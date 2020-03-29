#ifndef CUI_STRING_HPP
#define CUI_STRING_HPP

#include "../utility/utility.hpp"
#include <array>

namespace cui::ct {

template <uint32_t N>
struct string_with_size;

struct string
{
	const char* str;
	uint32_t size;
	constexpr string() : str(""), size(0) {}
	constexpr string(const char* str) : str(str), size(cui::util::strlen(str)) {
		str != nullptr ? 0 : throw std::runtime_error("");
	}
	constexpr string(const char* str, uint32_t size) : str(str), size(size) {}
	constexpr char operator[](uint32_t const idx) const {
		return idx < size ? str[idx] : throw std::out_of_range("");
	}

	constexpr bool operator==(string rhs) const {
		if (size != rhs.size) return false;
		for (int i = 0; i < size; ++i) {
			if (str[i] != rhs[i]) return false;
		}
		return true;
	}

	constexpr bool operator!=(string rhs) const {
		if (size != rhs.size) return false;
		for (int i = 0; i < size; ++i) {
			if (str[i] == rhs[i]) return false;
		}
		return true;
	}

	template <uint32_t N1>
	constexpr bool operator==(string_with_size<N1> rhs) const {
		if (size != N1) return false;
		for (int i = 0; i < size; ++i) {
			if (str[i] != rhs[i]) return false;
		}
		return true;
	}

	template <uint32_t N1>
	constexpr bool operator!=(string_with_size<N1> rhs) const {
		if (size != N1) return false;
		for (int i = 0; i < size; ++i) {
			if (str[i] == rhs[i]) return false;
		}
		return true;
	}
};

template <uint32_t N>
struct string_with_size
{
	char str[N];
	static constexpr uint32_t size = N;
	constexpr string_with_size() : str("") {}
	constexpr char operator[](uint32_t idx) const {
		return idx < size ? str[idx] : throw std::out_of_range("");
	}

	template <uint32_t N1>
	constexpr bool operator==(string_with_size<N1> rhs) const {
		if constexpr (N != N1) return false;
		for (int i = 0; i < N; ++i) {
			if (str[i] != rhs[i]) return false;
		}
		return true;
	}

	template <uint32_t N1>
	constexpr bool operator!=(string_with_size<N1> rhs) const {
		if constexpr (N != N1) return false;
		for (int i = 0; i < N; ++i) {
			if (str[i] == rhs[i]) return false;
		}
		return true;
	}

	constexpr bool operator==(string rhs) const {
		if (size != rhs.size) return false;
		for (int i = 0; i < size; ++i) {
			if (str[i] != rhs[i]) return false;
		}
		return true;
	}

	constexpr bool operator!=(string rhs) const {
		if (size != rhs.size) return false;
		for (int i = 0; i < size; ++i) {
			if (str[i] == rhs[i]) return false;
		}
		return true;
	}
};

template <uint32_t N>
constexpr string_with_size<N - 1> construct_string_with_size(char const (&str)[N]) {
	string_with_size<N - 1> str_;
	for (int i = 0; i < N - 1; ++i) {
		*(str_.str + i) = str[i];
	}
	return str_;
}

template <uint32_t N1, uint32_t N2>
constexpr string_with_size<N1> construct_string_with_size(char const (&str)[N2]) {
	string_with_size<N1> str_;
	for (int i = 0; i < N2; ++i) {
		*(str_.str + i) = str[i];
	}
	return str_;
}

constexpr string operator"" _ctstr(const char* str, uint32_t size) {
	return string{str, cui::util::strlen(str)};
}

template <typename String>
constexpr uint32_t find_amount_of(String str, char C) {
	uint32_t amount = 0;
	uint32_t current = 0;
	while (current < str.size) {
		if (*(str.str + current) == C) ++amount;
		++current;
	}
	return amount;
}

template <typename String>
constexpr uint32_t find(String str, char C) {
	uint32_t current = 0;
	while (current < str.size) {
		if (str[current] == C) {
			return current;
		}
		++current;
	}
	return str.size;
}

template <typename String>
constexpr uint32_t find(String str, char C, uint32_t pos) {
	if (pos >= str.size) return str.size;
	uint32_t current = pos;
	while (current < str.size) {
		if (str[current] == C) {
			return current;
		}
		++current;
	}
	return str.size;
}

template <typename String>
constexpr uint32_t find_last(String str, char C) {
	uint32_t current = str.size - 1;
	while (current > 0) {
		if (str[current] == C) {
			return current;
		}
		--current;
	}
	if (str[current] == C) return 0;
	return str.size;
}

constexpr string trim_left(string str) {
	uint32_t current = 0;
	while (current < str.size) {
		if (!cui::util::is_whitespace(str[current])) {
			return {str.str + current, str.size - current};
		}
		++current;
	}
	return {"", 0};
}

constexpr string trim_right(string str) {
	if (str.size != 0) {
		uint32_t current = str.size - 1;
		do {
			if (!cui::util::is_whitespace(str[current])) {
				return {str.str, current + 1};
			}
			if (current == 0) break;
			--current;
		} while (current);
	}

	return {"", 0};
}

constexpr string trim(string str) {
	return trim_left(trim_right(str));
}

template <typename String>
constexpr uint32_t count_until_delim(String str, char delim, char count_me) {
	uint32_t current = 0;
	uint32_t count = 0;
	while (current < str.size) {
		if (str[current] == delim) break;
		if (str[current] == count_me) ++count;
		++current;
	}
	return count;
}

template <uint32_t N>
constexpr uint32_t str_size(char const (&)[N]) {
	return N - 1;
}

template <uint32_t N>
constexpr uint32_t str_size(string_with_size<N> str) {
	uint32_t i = 0;
	while (str.str != 0) ++i;
	return i;
}

template <typename Container, std::size_t N>
constexpr void concat_impl(Container& c, uint32_t& pos, char const (&str)[N]) {
	for (uint32_t i = 0; i < N - 1 && str[i] != 0; ++i) {
		*(c.str + pos++) = str[i];
	}
}

template <typename Container>
constexpr void concat_impl(Container& c, uint32_t& pos, cui::ct::string str) {
	for (uint32_t i = 0; i < str.size; ++i) {
		*(c.str + pos++) = str[i];
	}
}

template <typename Container, uint32_t N>
constexpr void concat_impl(Container& c, uint32_t& pos, cui::ct::string_with_size<N> str) {
	for (uint32_t i = 0; i < N && str[i] != 0; ++i) {
		*(c.str + pos++) = str[i];
	}
}

template <typename... Args>
constexpr auto concat_literals(Args&&... args) {
	string_with_size<1024> arr;
	uint32_t pos = 0;
	(concat_impl(arr, pos, args), ...);
	return arr;
}

constexpr auto num_to_string(int num) {
	auto size = cui::util::num_of_whole_digits(num);
	if (size > 10) throw std::out_of_range{""};
	string_with_size<10> str;
	for (uint32_t i = 0; i < 10; ++i) {
		if (i < size) *(str.str + i) = '0' + (num % cui::util::pow(10, size - i)) / cui::util::pow(10, size - i - 1);
		else
			*(str.str + i) = 0;
	}
	return str;
}

template <typename String>
constexpr bool contains_only(String const& str, String const& only) {
	for (int i = 0; i < str.size; ++i) {
		bool contained = false;
		for (int j = 0; j < only.size; ++j) {
			if (str[i] == only[i]) contained = true;
		}

		if (!contained) return false;
	}
	return true;
}

template <typename String, uint32_t N>
constexpr bool contains_only(String const& str, const char (&only)[N]) {
	for (int i = 0; i < str.size; ++i) {
		bool contained = false;
		for (int j = 0; j < N; ++j) {
			if (str[i] == only[i]) contained = true;
		}

		if (!contained) return false;
	}
	return true;
}

class error_report
{
	cui::ct::string_with_size<1024> msg;

public:
	template <uint32_t N>
	constexpr error_report(const char (&msg)[N], uint32_t line, uint32_t col)
		: msg(cui::ct::concat_literals(
			"LINE:", cui::ct::num_to_string(line), " COL:", cui::ct::num_to_string(col), "\n", msg)) {}

	template <typename... Strings>
	constexpr error_report(Strings... strs) : msg(cui::ct::concat_literals(strs...)) {}
	constexpr const cui::ct::string_with_size<1024> what() const {
		return msg;
	}
};

}	 // namespace cui::ct

namespace cui {

void from_ct_string(std::string& to, cui::ct::string const& from) {
	if (to.capacity() < from.size) {
		to.reserve(from.size);
	}
	for (int i = 0; i < from.size; ++i) {
		to.push_back(from[i]);
	}
}

bool operator==(std::string const& a, cui::ct::string b) {
	if (a.size() != b.size) return false;
	for (int i = 0; i < b.size; ++i) {
		if (a[i] != b[i]) return false;
	}
	return true;
}

bool operator==(cui::ct::string a, std::string const& b) {
	if (a.size != b.size()) return false;
	for (int i = 0; i < a.size; ++i) {
		if (a[i] != b[i]) return false;
	}
	return true;
}

bool operator!=(std::string const& a, cui::ct::string b) {
	if (a.size() != b.size) return true;
	for (int i = 0; i < b.size; ++i) {
		if (a[i] == b[i]) return false;
	}
	return true;
}

bool operator!=(cui::ct::string a, std::string const& b) {
	if (a.size != b.size()) return true;
	for (int i = 0; i < a.size; ++i) {
		if (a[i] == b[i]) return false;
	}
	return true;
}

}	 // namespace cui

#endif	  //CUI_STRING_HPP

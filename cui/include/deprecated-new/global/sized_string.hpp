#ifndef CUI_GLOBAL_SIZED_STRING_HPP
#define CUI_GLOBAL_SIZED_STRING_HPP

#include <global/constants.hpp>
#include <global/min.hpp>
#include <compile_time/static_vector.hpp>
#include <compile_time/string_utils/char_traits.hpp>

namespace cui {

template <std::size_t Size>
using SizedString = cui::static_vector<char, Size>;

}

// const char[N]

template <std::size_t Size, std::size_t N>
constexpr bool operator==(const cui::SizedString<Size>& lhs, const char (&rhs)[N]) {
	constexpr std::size_t size = cui::min(Size, N - 1);
	std::size_t i = 0;
	while (i < size) {
		if (lhs[i] != rhs[i++]) return false;
	}
	return true;
}

template <std::size_t Size, std::size_t N>
constexpr bool operator!=(const cui::SizedString<Size>& lhs, const char (&rhs)[N]) {
	constexpr std::size_t size = cui::min(Size, N - 1);
	std::size_t i = 0;
	while (i < size) {
		if (lhs[i] == rhs[i++]) return false;
	}
	return true;
}

template <std::size_t Size, std::size_t N>
constexpr bool operator>(const cui::SizedString<Size>& lhs, const char (&rhs)[N]) {
	constexpr std::size_t size = cui::min(Size, N - 1);
	std::size_t i = 0;
	while (i < size) {
		if (lhs[i] <= rhs[i++]) return false;
	}
	return true;
}

template <std::size_t Size, std::size_t N>
constexpr bool operator<(const cui::SizedString<Size>& lhs, const char (&rhs)[N]) {
	constexpr std::size_t size = cui::min(Size, N - 1);
	std::size_t i = 0;
	while (i < size) {
		if (lhs[i] >= rhs[i++]) return false;
	}
	return true;
}

template <std::size_t Size, std::size_t N>
constexpr bool operator>=(const cui::SizedString<Size>& lhs, const char (&rhs)[N]) {
	constexpr std::size_t size = cui::min(Size, N - 1);
	std::size_t i = 0;
	while (i < size) {
		if (lhs[i] < rhs[i++]) return false;
	}
	return true;
}

template <std::size_t Size, std::size_t N>
constexpr bool operator<=(const cui::SizedString<Size>& lhs, const char (&rhs)[N]) {
	constexpr std::size_t size = cui::min(Size, N - 1);
	std::size_t i = 0;
	while (i < size) {
		if (lhs[i] > rhs[i++]) return false;
	}
	return true;
}

// const char*

template <std::size_t Size>
constexpr bool operator==(const cui::SizedString<Size>& lhs, const char* rhs) {
	std::size_t size = cui::min(Size, cui::CharTraits::length(rhs));
	std::size_t i = 0;
	while (i < size) {
		if (lhs[i] != rhs[i++]) return false;
	}
	return true;
}

template <std::size_t Size>
constexpr bool operator!=(const cui::SizedString<Size>& lhs, const char* rhs) {
	std::size_t size = cui::min(Size, cui::CharTraits::length(rhs));
	std::size_t i = 0;
	while (i < size) {
		if (lhs[i] == rhs[i++]) return false;
	}
	return true;
}

template <std::size_t Size>
constexpr bool operator>(const cui::SizedString<Size>& lhs, const char* rhs) {
	std::size_t size = cui::min(Size, cui::CharTraits::length(rhs));
	std::size_t i = 0;
	while (i < size) {
		if (lhs[i] <= rhs[i++]) return false;
	}
	return true;
}

template <std::size_t Size>
constexpr bool operator<(const cui::SizedString<Size>& lhs, const char* rhs) {
	std::size_t size = cui::min(Size, cui::CharTraits::length(rhs));
	std::size_t i = 0;
	while (i < size) {
		if (lhs[i] >= rhs[i++]) return false;
	}
	return true;
}

template <std::size_t Size>
constexpr bool operator>=(const cui::SizedString<Size>& lhs, const char* rhs) {
	std::size_t size = cui::min(Size, cui::CharTraits::length(rhs));
	std::size_t i = 0;
	while (i < size) {
		if (lhs[i] < rhs[i++]) return false;
	}
	return true;
}

template <std::size_t Size>
constexpr bool operator<=(const cui::SizedString<Size>& lhs, const char* rhs) {
	std::size_t size = cui::min(Size, cui::CharTraits::length(rhs));
	std::size_t i = 0;
	while (i < size) {
		if (lhs[i] > rhs[i++]) return false;
	}
	return true;
}

// Generic non-sized string

template <std::size_t Size, typename StringT>
constexpr bool operator==(const cui::SizedString<Size>& lhs, const StringT& rhs) {
	std::size_t size = cui::min(Size, rhs.size());
	std::size_t i = 0;
	while (i < size) {
		if (lhs[i] != rhs[i++]) return false;
	}
	return true;
}

template <std::size_t Size, typename StringT>
constexpr bool operator!=(const cui::SizedString<Size>& lhs, const StringT& rhs) {
	std::size_t size = cui::min(Size, rhs.size());
	std::size_t i = 0;
	while (i < size) {
		if (lhs[i] == rhs[i++]) return false;
	}
	return true;
}

template <std::size_t Size, typename StringT>
constexpr bool operator>(const cui::SizedString<Size>& lhs, const StringT& rhs) {
	std::size_t size = cui::min(Size, rhs.size());
	std::size_t i = 0;
	while (i < size) {
		if (lhs[i] <= rhs[i++]) return false;
	}
	return true;
}

template <std::size_t Size, typename StringT>
constexpr bool operator<(const cui::SizedString<Size>& lhs, const StringT& rhs) {
	std::size_t size = cui::min(Size, rhs.size());
	std::size_t i = 0;
	while (i < size) {
		if (lhs[i] >= rhs[i++]) return false;
	}
	return true;
}

template <std::size_t Size, typename StringT>
constexpr bool operator>=(const cui::SizedString<Size>& lhs, const StringT& rhs) {
	std::size_t size = cui::min(Size, rhs.size());
	std::size_t i = 0;
	while (i < size) {
		if (lhs[i] < rhs[i++]) return false;
	}
	return true;
}

template <std::size_t Size, typename StringT>
constexpr bool operator<=(const cui::SizedString<Size>& lhs, const StringT& rhs) {
	std::size_t size = cui::min(Size, rhs.size());
	std::size_t i = 0;
	while (i < size) {
		if (lhs[i] > rhs[i++]) return false;
	}
	return true;
}

// Generic sized string

template <std::size_t Size, template <std::size_t> typename StringT, std::size_t N>
constexpr bool operator==(const cui::SizedString<Size>& lhs, const StringT<N>& rhs) {
	constexpr std::size_t size = cui::min(Size, N - 1);
	std::size_t i = 0;
	while (i < size) {
		if (lhs[i] != rhs[i++]) return false;
	}
	return true;
}

template <std::size_t Size, template <std::size_t> typename StringT, std::size_t N>
constexpr bool operator!=(const cui::SizedString<Size>& lhs, const StringT<N>& rhs) {
	constexpr std::size_t size = cui::min(Size, N - 1);
	std::size_t i = 0;
	while (i < size) {
		if (lhs[i] == rhs[i++]) return false;
	}
	return true;
}

template <std::size_t Size, template <std::size_t> typename StringT, std::size_t N>
constexpr bool operator>(const cui::SizedString<Size>& lhs, const StringT<N>& rhs) {
	constexpr std::size_t size = cui::min(Size, N - 1);
	std::size_t i = 0;
	while (i < size) {
		if (lhs[i] <= rhs[i++]) return false;
	}
	return true;
}

template <std::size_t Size, template <std::size_t> typename StringT, std::size_t N>
constexpr bool operator<(const cui::SizedString<Size>& lhs, const StringT<N>& rhs) {
	constexpr std::size_t size = cui::min(Size, N - 1);
	std::size_t i = 0;
	while (i < size) {
		if (lhs[i] >= rhs[i++]) return false;
	}
	return true;
}

template <std::size_t Size, template <std::size_t> typename StringT, std::size_t N>
constexpr bool operator>=(const cui::SizedString<Size>& lhs, const StringT<N>& rhs) {
	constexpr std::size_t size = cui::min(Size, N - 1);
	std::size_t i = 0;
	while (i < size) {
		if (lhs[i] < rhs[i++]) return false;
	}
	return true;
}

template <std::size_t Size, template <std::size_t> typename StringT, std::size_t N>
constexpr bool operator<=(const cui::SizedString<Size>& lhs, const StringT<N>& rhs) {
	constexpr std::size_t size = cui::min(Size, N - 1);
	std::size_t i = 0;
	while (i < size) {
		if (lhs[i] > rhs[i++]) return false;
	}
	return true;
}

#endif	  //  CUI_GLOBAL_SIZED_STRING_HPP
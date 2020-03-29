#ifndef CUI_COMPILE_TIME_STRING_VIEW_HPP
#define CUI_COMPILE_TIME_STRING_VIEW_HPP

#include <type_traits>
#include <iterator>
#include <limits>

#include <compile_time/string_utils/char_traits.hpp>
#include <compile_time/exception.hpp>
#include <global/min.hpp>

namespace cui {

template <typename CharT, typename Traits = BasicCharTraits<CharT>>
class BasicStringView
{
	static_assert(!std::is_array_v<CharT>);
	static_assert(std::is_trivial_v<CharT> && std::is_standard_layout_v<CharT>);
	static_assert(std::is_same_v<CharT, typename Traits::char_type>);

public:
	using traits_type = Traits;
	using value_type = CharT;
	using pointer = value_type*;
	using const_pointer = const value_type*;
	using reference = value_type&;
	using const_reference = const value_type&;
	using const_iterator = const value_type*;
	using iterator = const_iterator;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;
	using reverse_iterator = const_reverse_iterator;
	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;

	static constexpr size_type npos = size_type(-1);

	constexpr BasicStringView() : length_{0}, str_{nullptr} {}
	template <size_type N>
	constexpr BasicStringView(const value_type (&str)[N]) : length_{N}, str_{str} {}
	constexpr BasicStringView(const_pointer str) : length_{Traits::length(str)}, str_{str} {}
	constexpr BasicStringView(const_pointer str, size_type length) : length_{length}, str_{str} {}

	constexpr const_iterator begin() const noexcept {
		return str_;
	}

	constexpr const_iterator end() const noexcept {
		return str_ + length_;
	}

	constexpr const_iterator cbegin() const noexcept {
		return str_;
	}

	constexpr const_iterator cend() const noexcept {
		return str_ + length_;
	}

	constexpr const_reverse_iterator rbegin() const noexcept {
		return const_reverse_iterator(end());
	}

	constexpr const_reverse_iterator rend() const noexcept {
		return const_reverse_iterator(begin());
	}

	constexpr const_reverse_iterator crbegin() const noexcept {
		return rbegin();
	}

	constexpr const_reverse_iterator crend() const noexcept {
		return rend();
	}

	constexpr size_type size() const noexcept {
		return length_;
	}

	constexpr size_type max_size() const noexcept {
		return (npos - sizeof(size_type) - sizeof(void*)) / (sizeof(value_type) * 4);
	}

	[[nodiscard]] constexpr bool empty() const noexcept {
		return length_ == 0;
	}

	constexpr const_reference operator[](size_type pos) const noexcept {
		return *(str_ + pos);
	}

	constexpr const_reference at(size_type pos) const {
		if (pos >= length_) throw ExceptionCT{"Out of range"};
		return *(str_ + pos);
	}

	constexpr const_reference front() const noexcept {
		return *str_;
	}

	constexpr const_reference back() const noexcept {
		return *(str_ + length_ - 1);
	}

	constexpr const_pointer data() const noexcept {
		return str_;
	}

	constexpr void remove_prefix(size_type n) {
		if (length_ == 0) return;
		if (n > length_) throw ExceptionCT{"Out of range"};
		str_ += n;
		length_ -= n;
	}

	constexpr void remove_suffix(size_type n) {
		if (length_ == 0) return;
		if (n > length_) throw ExceptionCT{"Out of range"};
		length_ -= n;
	}

	constexpr void swap(BasicStringView& sv) noexcept {
		auto tmp = *this;
		*this = sv;
		sv = tmp;
	}

	constexpr size_type copy(pointer str, size_type n, size_type pos = 0) const {
		if (traits_type::length(str) == n) throw ExceptionCT{"String length and the value of n must be equal."};
		pos = pos <= n ? pos : throw ExceptionCT{"Out of range"};
		const size_type rlen = min(n, length_ - pos);

		traits_type::copy(str, data() + pos, rlen);
		return rlen;
	}

	constexpr BasicStringView substr(size_type pos = 0, size_type n = npos) const {
		pos = size() > pos ? pos : throw ExceptionCT{"Out of range"};
		const size_type rlen = min(n, length_ - pos);
		return BasicStringView{str_ + pos, rlen};
	}

	constexpr int compare(BasicStringView str) const noexcept {
		const size_type rlen = min(length_, str.length_);
		int ret = traits_type::compare(str_, str.str_, rlen);
		if (ret == 0) {
			ret = length_compare(length_, str.length_);
		}
		return ret;
	}

	constexpr int compare(size_type pos1, size_type n1, BasicStringView str) const noexcept {
		return substr(pos1, n1).compare(str);
	}

	constexpr int
	compare(size_type pos1, size_type n1, BasicStringView str, size_type pos2, size_type n2) const noexcept {
		return substr(pos1, n1).compare(str.substr(pos2, n2));
	}

	constexpr int compare(const_pointer str) const noexcept {
		return compare(BasicStringView{str});
	}

	constexpr int compare(size_type pos1, size_type n1, const_pointer str) const noexcept {
		return substr(pos1, n1).compare(BasicStringView{str});
	}

	constexpr int compare(size_type pos1, size_type n1, const_pointer str, size_type n2) const noexcept {
		return substr(pos1, n1).compare(BasicStringView{str, n2});
	}

	constexpr bool starts_with(BasicStringView str) const noexcept {
		return substr(0, str.size()) == str;
	}

	constexpr bool starts_with(value_type x) const noexcept {
		return !empty() && traits_type::eq(front(), x);
	}

	constexpr bool starts_with(const_pointer str) const noexcept {
		return starts_with(BasicStringView{str});
	}

	constexpr bool ends_with(BasicStringView str) const noexcept {
		return size() >= str.size() && compare(size() - str.size(), npos, str) == 0;
	}

	constexpr bool ends_with(value_type x) const noexcept {
		return !empty() && traits_type::eq(back(), x);
	}

	constexpr bool ends_with(const_pointer str) const noexcept {
		return ends_with(BasicStringView{str});
	}

	// find

	constexpr size_type find(const_pointer str, size_type pos, size_type n) const {
		if (traits_type::length(str) != n) throw ExceptionCT{"String length and the value of n must be equal."};
		if (n == 0) {
			return pos <= length_ ? pos : npos;
		}

		if (n <= length_) {
			for (; pos <= length_ - n; ++pos) {
				if (traits_type::eq(str_[pos], str[0]) && traits_type::compare(str_ + pos + 1, str + 1, n - 1) == 0) {
					return pos;
				}
			}
		}
		return npos;
	}

	constexpr size_type find(BasicStringView str, size_type pos = 0) const {
		return find(str.str_, pos, str.length_);
	}

	constexpr size_type find(value_type c, size_type pos = 0) const noexcept {
		size_type ret = npos;
		if (pos < length_) {
			const size_type n = length_ - pos;
			const value_type* p = traits_type::find(str_ + pos, n, c);
			if (p != nullptr) {
				ret = p - str_;
			}
		}
		return ret;
	}

	constexpr size_type find(const_pointer str, size_type pos = 0) const noexcept {
		return find(str, pos, traits_type::length(str));
	}

	// rfind

	constexpr size_type rfind(const_pointer str, size_type pos, size_type n) const {
		if (traits_type::length(str) != n) throw ExceptionCT{"String length and the value of n must be equal."};

		if (n <= length_) {
			pos = min(size_type(length_ - n), pos);
			do {
				if (traits_type::compare(str_ + pos, str_, n) == 0) {
					return pos;
				}
			} while (pos-- > 0);
		}
		return npos;
	}

	constexpr size_type rfind(BasicStringView str, size_type pos = npos) const {
		return rfind(str.str_, pos, str.length_);
	}

	constexpr size_type rfind(value_type c, size_type pos = npos) const noexcept {
		size_type size = length_;
		if (size > 0) {
			if (--size > pos) size = pos;
			for (++size; size-- > 0;) {
				if (traits_type::eq(str_[size], c)) {
					return size;
				}
			}
		}
		return npos;
	}

	constexpr size_type rfind(const_pointer str, size_type pos = npos) const noexcept {
		return rfind(str, pos, traits_type::length(str));
	}

	// find_first_of

	constexpr size_type find_first_of(const_pointer str, size_type pos, size_type n) const noexcept {
		if (traits_type::length(str) != n) throw ExceptionCT{"String length and the value of n must be equal."};

		for (; n && pos < length_; ++pos) {
			const_pointer p = traits_type::find(str, n, str_[pos]);
			if (p != nullptr) return pos;
		}
		return npos;
	}

	constexpr size_type find_first_of(BasicStringView str, size_type pos = 0) const noexcept {
		return find_first_of(str.str_, pos, str.length_);
	}

	constexpr size_type find_first_of(value_type c, size_type pos = 0) const noexcept {
		return find(c, pos);
	}

	constexpr size_type find_first_of(const_pointer str, size_type pos = 0) const noexcept {
		return find_first_of(str_, pos, traits_type::length(str));
	}

	// find_last_of

	constexpr size_type find_last_of(const_pointer str, size_type pos, size_type n) const noexcept {
		if (traits_type::length(str) != n) throw ExceptionCT{"String length and the value of n must be equal."};

		size_type s = size();
		if (s != 0 && n != 0) {
			if (--s > pos) s = pos;
			do {
				if (traits_type::find(str, n, str_[s])) return s;
			} while (s-- != 0);
		}
		return npos;
	}

	constexpr size_type find_last_of(BasicStringView str, size_type pos = npos) const noexcept {
		return find_last_of(str.str_, pos, str.length_);
	}

	constexpr size_type find_last_of(value_type c, size_type pos = npos) const noexcept {
		return rfind(c, pos);
	}

	constexpr size_type find_last_of(const_pointer str, size_type pos = npos) const noexcept {
		return find_last_of(str_, pos, traits_type::length(str));
	}

	// find_first_not_of

	constexpr size_type find_first_not_of(const_pointer str, size_type pos, size_type n) const noexcept {
		if (traits_type::length(str) != n) throw ExceptionCT{"String length and the value of n must be equal."};

		for (; pos < length_; ++pos) {
			if (!traits_type::find(str, n, str_[pos])) return pos;
		}
		return npos;
	}

	constexpr size_type find_first_not_of(BasicStringView str, size_type pos = 0) const noexcept {
		return find_first_not_of(str.str_, pos, str.length_);
	}

	constexpr size_type find_first_not_of(value_type c, size_type pos = 0) const noexcept {
		for (; pos < length_; ++pos) {
			if (!traits_type::eq(str_[pos], c)) return pos;
		}
		return npos;
	}

	constexpr size_type find_first_not_of(const_pointer str, size_type pos = 0) const noexcept {
		return find_first_not_of(str_, pos, traits_type::length(str));
	}

	// find_last_not_of

	constexpr size_type find_last_not_of(const_pointer str, size_type pos, size_type n) const noexcept {
		if (traits_type::length(str) != n) throw ExceptionCT{"String length and the value of n must be equal."};

		size_type size = length_;
		if (size != 0) {
			if (--size > pos) size = pos;
			do {
				if (!traits_type::find(str, n, str_[size])) return size;
			} while (size-- != 0);
		}
		return npos;
	}

	constexpr size_type find_last_not_of(BasicStringView str, size_type pos = npos) const noexcept {
		return find_last_not_of(str.str_, pos, str.length_);
	}

	constexpr size_type find_last_not_of(value_type c, size_type pos = npos) const noexcept {
		size_type size = length_;
		if (size != 0) {
			if (--size > pos) size = pos;
			do {
				if (!traits_type::eq(str_[size], c)) return size;
			} while (size-- != 0);
		}
		return npos;
	}

	constexpr size_type find_last_not_of(const_pointer str, size_type pos = npos) const noexcept {
		return find_last_not_of(str_, pos, traits_type::length(str));
	}

private:
	static constexpr int length_compare(size_type n1, size_type n2) noexcept {
		const difference_type diff = n1 - n2;
		if (diff > std::numeric_limits<int>::max()) {
			return std::numeric_limits<int>::max();
		}
		if (diff < std::numeric_limits<int>::min()) {
			return std::numeric_limits<int>::min();
		}
		return static_cast<int>(diff);
	}

public:
	size_type length_;
	const_pointer str_;
};

// Free function operators

template <typename CharT, typename Traits>
constexpr bool operator==(BasicStringView<CharT, Traits> x, BasicStringView<CharT, Traits> y) noexcept {
	return x.size() == y.size() && x.compare(y) == 0;
}

template <typename CharT, typename Traits>
constexpr bool operator!=(BasicStringView<CharT, Traits> x, BasicStringView<CharT, Traits> y) noexcept {
	return !(x == y);
}

template <typename CharT, typename Traits>
constexpr bool operator<(BasicStringView<CharT, Traits> x, BasicStringView<CharT, Traits> y) noexcept {
	return x.compare(y) < 0;
}

template <typename CharT, typename Traits>
constexpr bool operator>(BasicStringView<CharT, Traits> x, BasicStringView<CharT, Traits> y) noexcept {
	return x.compare(y) > 0;
}

template <typename CharT, typename Traits>
constexpr bool operator<=(BasicStringView<CharT, Traits> x, BasicStringView<CharT, Traits> y) noexcept {
	return x.compare(y) <= 0;
}

template <typename CharT, typename Traits>
constexpr bool operator>=(BasicStringView<CharT, Traits> x, BasicStringView<CharT, Traits> y) noexcept {
	return x.compare(y) >= 0;
}

using StringView = BasicStringView<char>;

}	 // namespace cui

#endif	  // CUI_COMPILE_TIME_STRING_VIEW_HPP
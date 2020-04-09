#ifndef CUI_STATIC_VECTOR_HPP
#define CUI_STATIC_VECTOR_HPP

#include <array>

#include <aliases.hpp>

namespace cui {

template <typename T, u64 Size>
class StaticVector
{
public:
	using value_type = T;
	using pointer = value_type*;
	using const_pointer = const value_type*;
	using reference = value_type&;
	using const_reference = const value_type&;
	using iterator = value_type*;
	using const_iterator = const value_type*;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = const std::reverse_iterator<iterator>;
	using size_type = u64;
	using difference_type = i64;

	constexpr StaticVector() : size_{0}, data_{} {}

	template <typename... Args>
	constexpr StaticVector(Args... args) : size_{sizeof...(Args)}, data_{args...} {}

	[[nodiscard]] constexpr auto size() const noexcept -> size_type {
		return size_;
	}

	[[nodiscard]] constexpr auto max_size() const noexcept -> size_type {
		return Size;
	}

	[[nodiscard]] constexpr bool full() const noexcept {
		return size() == max_size();
	}

	[[nodiscard]] constexpr bool empty() const noexcept {
		return size() == 0;
	}

	[[nodiscard]] constexpr auto at(size_type pos) -> reference {
		if (pos >= size()) throw "Out of bounds";
		return data_[pos];
	}

	[[nodiscard]] constexpr auto at(size_type pos) const -> const_reference {
		if (pos >= size()) throw "Out of bounds";
		return data_[pos];
	}

	[[nodiscard]] constexpr auto operator[](size_type pos) -> reference {
		return data_[pos];
	}

	[[nodiscard]] constexpr auto operator[](size_type pos) const -> const_reference {
		return data_[pos];
	}

	[[nodiscard]] constexpr auto front() -> reference {
		if (empty()) throw "Out of bounds";
		return data_.front();
	}

	[[nodiscard]] constexpr auto front() const -> const_reference {
		if (empty()) throw "Out of bounds";
		return data_.front();
	}

	[[nodiscard]] constexpr auto back() -> reference {
		if (empty()) throw "Out of bounds";
		return data_[size_ - 1];
	}

	[[nodiscard]] constexpr auto back() const -> const_reference {
		if (empty()) throw "Out of bounds";
		return data_[size_ - 1];
	}

	[[nodiscard]] constexpr auto data() -> pointer {
		return data_.data();
	}

	[[nodiscard]] constexpr auto data() const -> const_pointer {
		return data_.data();
	}

	[[nodiscard]] constexpr auto begin() noexcept -> iterator {
		return data_.begin();
	}

	[[nodiscard]] constexpr auto begin() const noexcept -> const_iterator {
		return data_.begin();
	}

	[[nodiscard]] constexpr auto cbegin() const noexcept -> const_iterator {
		return data_.cbegin();
	}

	[[nodiscard]] constexpr auto end() noexcept -> iterator {
		return data_.begin() + size();
	}

	[[nodiscard]] constexpr auto end() const noexcept -> const_iterator {
		return data_.begin() + size();
	}

	[[nodiscard]] constexpr auto cend() const noexcept -> const_iterator {
		return data_.begin() + size();
	}

	[[nodiscard]] constexpr auto rbegin() noexcept -> reverse_iterator {
		return data_.rbegin();
	}

	[[nodiscard]] constexpr auto rbegin() const noexcept -> const_reverse_iterator {
		return data_.rbegin();
	}

	[[nodiscard]] constexpr auto crbegin() const noexcept -> const_reverse_iterator {
		return data_.crbegin();
	}

	[[nodiscard]] constexpr auto rend() noexcept -> reverse_iterator {
		return data_.rbegin() + size();
	}

	[[nodiscard]] constexpr auto rend() const noexcept -> const_reverse_iterator {
		return data_.rbegin() + size();
	}

	[[nodiscard]] constexpr auto crend() const noexcept -> const_reverse_iterator {
		return data_.crbegin() + size();
	}

	constexpr void assign(size_type count, const_reference val) {
		if (count >= max_size() - size()) throw "Out of bounds";
		size_ = count;
		for (size_type i = 0; i < size_; ++i) {
			data_[i] = val;
		}
	}

	template <typename InputIt>
	constexpr void assign(InputIt first, InputIt last) {
		const auto dist = std::distance<InputIt>(first, last);
		if (dist >= max_size() - size()) throw "Out of bounds";
		size_ = dist;
		auto it = data_.begin();
		while (first < last) {
			*(it++) = *(first++);
		}
	}

	constexpr void assign(std::initializer_list<value_type> ilist) {
		if (ilist.size() > max_size()) throw "Out of bounds";
		auto it = data_.begin();
		auto in_it = ilist.begin();
		while (in_it < ilist.end()) {
			*(it++) = *(in_it++);
		}
	}

	constexpr void clear() {
		size_ = 0;
	}

	constexpr auto insert(iterator pos, const_reference val) -> iterator {
		if (std::distance<const_iterator>(begin(), pos) >= max_size() - size() || size() >= max_size())
			throw "Out of bounds";
		for (auto it = pos; it < cend(); ++it) {
			*(it + 1) = *it;
		}
		*pos = val;
		return pos;
	}

	constexpr auto insert(iterator pos, value_type&& val) -> const_iterator {
		if (std::distance<const_iterator>(begin(), pos) >= max_size() - size() || size() >= max_size())
			throw "Out of bounds";
		for (auto it = pos; it < cend(); ++it) {
			*(it + 1) = *it;
		}
		*pos = std::move(val);
		return pos;
	}

	constexpr auto insert(iterator pos, size_type count, const_reference val) -> iterator {
		if (std::distance<const_iterator>(begin(), pos) + count >= max_size() - size() || size() - count >= max_size())
			throw "Out of bounds";
		for (auto it = pos; it < cend(); ++it) {
			*(it + count + 1) = *it;
		}
		for (size_type i = 0; i < count; ++i) {
			pos[i] = val;
		}
		return pos;
	}

	template <typename InputIt>
	constexpr auto insert(iterator pos, InputIt first, InputIt last) -> iterator {
		const auto count = std::distance(first, last);
		if (std::distance<const_iterator>(begin(), pos) + count >= max_size() - size() || size() - count >= max_size())
			throw "Out of bounds";
		for (auto it = pos; it < cend(); ++it) {
			*(it + count + 1) = *it;
		}
		auto it = pos;
		while (first < last) {
			*(it++) = *(first++);
		}
		return pos;
	}

	constexpr auto insert(iterator pos, std::initializer_list<value_type> ilist) -> iterator {
		const auto count = ilist.size();
		if (std::distance<const_iterator>(begin(), pos) + count >= max_size() - size() || size() - count >= max_size())
			throw "Out of bounds";
		for (auto it = pos; it < cend(); ++it) {
			*(it + count + 1) = *it;
		}
		auto it = pos;
		auto in_it = ilist.begin();
		while (in_it < ilist.end()) {
			*(it++) = *(in_it++);
		}
		return pos;
	}

	template <typename... Args>
	constexpr auto emplace(iterator pos, Args&&... args) -> iterator {
		insert(pos, value_type{std::forward<Args>(args)...});
		return pos;
	}

	constexpr auto erase(iterator pos) -> iterator {
		size_ = std::distance<const_iterator>(begin(), pos);
		return pos;
	}

	constexpr auto erase(iterator first, iterator last) -> iterator {
		const auto count = std::distance<const_iterator>(first, last);
		if (first >= last || first < begin() || last >= end()) throw "Invalid iterators";
		auto right_half_it = last;
		if (std::distance<const_iterator>(begin(), last) < size()) {
			auto it = first;
			while (right_half_it < end()) {
				*(it++) = *(right_half_it++);
			}
			size_ -= count;
		} else {
			size_ = std::distance(begin(), first);
		}
		return first;
	}

	constexpr void push_back(const_reference val) {
		if (full()) throw "StaticVector is full";
		data_[size_++] = val;
	}

	constexpr void push_back(value_type&& val) {
		if (full()) throw "StaticVector is full";
		data_[size_++] = std::move(val);
	}

	template <typename... Args>
	constexpr auto emplace_back(Args... args) -> reference {
		if (full()) throw "StaticVector is full";
		data_[size_++] = value_type{std::forward<Args>(args)...};
		return back();
	}

	constexpr void pop_back() {
		if (empty()) throw "StaticVector is empty";
		--size_;
	}

	constexpr void resize(size_type count) {
		if (count >= max_size()) throw "Resize parameter larger than max size";
		size_ = count;
	}

	constexpr void resize(size_type count, const_reference val) {
		if (count > max_size()) throw "Resize parameter larger than max size";
		if (size() < count) {
			for (size_type i = 0; i < count - size(); ++i) {
				data_[i] = val;
			}
		}
		size_ = count;
	}

	template <typename T2, size_type Size2>
	constexpr void swap(StaticVector<T2, Size2>& other) {
		size_type i = 0;
		while (i < size() && i < other.size()) {
			value_type temp = data_[i];
			data_[i] = other[i];
			other[i++] = temp;
		}
	}

private:
	size_type size_;
	std::array<T, Size> data_;
};

template <typename T, u64 Size>
constexpr bool operator==(const StaticVector<T, Size>& lhs, const StaticVector<T, Size>& rhs) {
	u64 i = 0;
	while (i < Size) {
		if (lhs[i] != rhs[i++]) return false;
	}
	return true;
}

template <typename T, u64 Size>
constexpr bool operator!=(const StaticVector<T, Size>& lhs, const StaticVector<T, Size>& rhs) {
	u64 i = 0;
	while (i < Size) {
		if (lhs[i] == rhs[i++]) return false;
	}
	return true;
}

template <typename T, u64 Size>
constexpr bool operator<(const StaticVector<T, Size>& lhs, const StaticVector<T, Size>& rhs) {
	u64 i = 0;
	while (i < Size) {
		if (lhs[i] >= rhs[i++]) return false;
	}
	return true;
}

template <typename T, u64 Size>
constexpr bool operator>(const StaticVector<T, Size>& lhs, const StaticVector<T, Size>& rhs) {
	u64 i = 0;
	while (i < Size) {
		if (lhs[i] <= rhs[i++]) return false;
	}
	return true;
}

template <typename T, u64 Size>
constexpr bool operator<=(const StaticVector<T, Size>& lhs, const StaticVector<T, Size>& rhs) {
	u64 i = 0;
	while (i < Size) {
		if (lhs[i] > rhs[i++]) return false;
	}
	return true;
}

template <typename T, u64 Size>
constexpr bool operator>=(const StaticVector<T, Size>& lhs, const StaticVector<T, Size>& rhs) {
	u64 i = 0;
	while (i < Size) {
		if (lhs[i] < rhs[i++]) return false;
	}
	return true;
}

}	 // namespace cui

#endif	  // CUI_STATIC_VECTOR_HPP
#ifndef CUI_COMPILE_TIME_STATIC_VECTOR_HPP
#define CUI_COMPILE_TIME_STATIC_VECTOR_HPP

#include <global/constants.hpp>
#include <compile_time/exception.hpp>
#include <array>

namespace cui {

template <typename T, std::size_t Size>
class static_vector
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
	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;

	constexpr static_vector() : size_{0}, data_{} {}
	template <typename... Args>
	constexpr static_vector(Args... args) : size_{sizeof...(Args)}, data_{args...} {}

	[[nodiscard]] constexpr size_type size() const noexcept {
		return size_;
	}

	[[nodiscard]] constexpr size_type max_size() const noexcept {
		return Size;
	}

	[[nodiscard]] constexpr bool full() const noexcept {
		return size() == max_size();
	}

	[[nodiscard]] constexpr bool empty() const noexcept {
		return size() == 0;
	}

	constexpr void assign(size_type count, const_reference val) {
		if (count >= max_size() - size()) throw ExceptionCT{"Out of range"};
		size_ = count;
		for (std::size_t i = 0; i < size_; ++i) {
			data_[i] = val;
		}
	}

	template <typename InputIt>
	constexpr void assign(InputIt first, InputIt last) {
		const auto dist = std::distance<InputIt>(first, last);
		if (dist >= max_size() - size()) throw ExceptionCT{"Out of range"};
		size_ = dist;
		auto it = data_.begin();
		while (first < last) {
			*(it++) = *(first++);
		}
	}

	constexpr void assign(std::initializer_list<value_type> ilist) {
		if (ilist.size() > max_size()) throw ExceptionCT{"Out of range"};
		auto it = data_.begin();
		auto in_it = ilist.begin();
		while (in_it < ilist.end()) {
			*(it++) = *(in_it++);
		}
	}

	constexpr reference at(size_type pos) {
		if (pos >= size()) throw ExceptionCT{"Out of range"};
		return data_[pos];
	}

	constexpr const_reference at(size_type pos) const {
		if (pos >= size()) throw ExceptionCT{"Out of range"};
		return data_[pos];
	}

	constexpr reference operator[](size_type pos) {
		return data_[pos];
	}

	constexpr const_reference operator[](size_type pos) const {
		return data_[pos];
	}

	constexpr reference front() {
		if (empty()) throw ExceptionCT{"Out of range"};
		return data_.front();
	}

	constexpr const_reference front() const {
		if (empty()) throw ExceptionCT{"Out of range"};
		return data_.front();
	}

	constexpr reference back() {
		if (empty()) throw ExceptionCT{"Out of range"};
		return data_[size_ - 1];
	}

	constexpr const_reference back() const {
		if (empty()) throw ExceptionCT{"Out of range"};
		return data_[size_ - 1];
	}

	constexpr pointer data() {
		return data_.data();
	}

	constexpr const_pointer data() const {
		return data_.data();
	}

	constexpr iterator begin() noexcept {
		return data_.begin();
	}

	constexpr const_iterator begin() const noexcept {
		return data_.begin();
	}

	constexpr const_iterator cbegin() const noexcept {
		return data_.cbegin();
	}

	constexpr iterator end() noexcept {
		return data_.begin() + size();
	}

	constexpr const_iterator end() const noexcept {
		return data_.begin() + size();
	}

	constexpr const_iterator cend() const noexcept {
		return data_.begin() + size();
	}

	constexpr reverse_iterator rbegin() noexcept {
		return data_.rbegin();
	}

	constexpr const_reverse_iterator rbegin() const noexcept {
		return data_.rbegin();
	}

	constexpr const_reverse_iterator crbegin() const noexcept {
		return data_.crbegin();
	}

	constexpr reverse_iterator rend() noexcept {
		return data_.rbegin() + size();
	}

	constexpr const_reverse_iterator rend() const noexcept {
		return data_.rbegin() + size();
	}

	constexpr const_reverse_iterator crend() const noexcept {
		return data_.crbegin() + size();
	}

	constexpr void clear() {
		size_ = 0;
	}

	constexpr iterator insert(iterator pos, const_reference val) {
		if (std::distance<const_iterator>(begin(), pos) >= max_size() - size()) throw ExceptionCT{"Out of range"};
		if (size() >= max_size()) throw ExceptionCT{"Out of range"};
		for (auto it = pos; it < cend(); ++it) {
			*(it + 1) = *it;
		}
		*pos = val;
		return pos;
	}

	constexpr const_iterator insert(iterator pos, value_type&& val) {
		if (std::distance<const_iterator>(begin(), pos) >= max_size() - size()) throw ExceptionCT{"Out of range"};
		if (size() >= max_size()) throw ExceptionCT{"Out of range"};
		for (auto it = pos; it < cend(); ++it) {
			*(it + 1) = *it;
		}
		*pos = std::move(val);
		return pos;
	}

	constexpr iterator insert(iterator pos, size_type count, const_reference val) {
		if (std::distance<const_iterator>(begin(), pos) + count >= max_size() - size())
			throw ExceptionCT{"Out of range"};
		if (size() - count >= max_size()) throw ExceptionCT{"Out of range"};
		for (auto it = pos; it < cend(); ++it) {
			*(it + count + 1) = *it;
		}
		for (std::size_t i = 0; i < count; ++i) {
			pos[i] = val;
		}
		return pos;
	}

	template <typename InputIt>
	constexpr iterator insert(iterator pos, InputIt first, InputIt last) {
		const auto count = std::distance(first, last);
		if (std::distance<const_iterator>(begin(), pos) + count >= max_size() - size())
			throw ExceptionCT{"Out of range"};
		if (size() - count >= max_size()) throw ExceptionCT{"Out of range"};
		for (auto it = pos; it < cend(); ++it) {
			*(it + count + 1) = *it;
		}
		auto it = pos;
		while (first < last) {
			*(it++) = *(first++);
		}
		return pos;
	}

	constexpr iterator insert(iterator pos, std::initializer_list<value_type> ilist) {
		const auto count = ilist.size();
		if (std::distance<const_iterator>(begin(), pos) + count >= max_size() - size())
			throw ExceptionCT{"Out of range"};
		if (size() - count >= max_size()) throw ExceptionCT{"Out of range"};
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
	constexpr iterator emplace(iterator pos, Args&&... args) {
		insert(pos, value_type{std::forward<Args>(args)...});
		return pos;
	}

	constexpr iterator erase(iterator pos) {
		size_ = std::distance<const_iterator>(begin(), pos);
		return pos;
	}

	constexpr iterator erase(iterator first, iterator last) {
		const auto count = std::distance<const_iterator>(first, last);
		if (first >= last) throw ExceptionCT{"Invalid iterators"};
		if (first < begin() || last >= end()) throw ExceptionCT{"Invalid iterators"};
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
		if (full()) throw ExceptionCT{"Vector is full"};
		data_[size_++] = val;
	}

	constexpr void push_back(value_type&& val) {
		if (full()) throw ExceptionCT{"Vector is full"};
		data_[size_++] = std::move(val);
	}

	template <typename... Args>
	constexpr reference emplace_back(Args... args) {
		if (full()) throw ExceptionCT{"Vector is full"};
		data_[size_++] = value_type{std::forward<Args>(args)...};
		return back();
	}

	constexpr void pop_back() {
		if (empty()) throw ExceptionCT{"Vector is empty"};
		--size_;
	}

	constexpr void resize(size_type count) {
		if (count >= max_size()) throw ExceptionCT{"Resize parameter larger than max size"};
		size_ = count;
	}

	constexpr void resize(size_type count, const_reference val) {
		if (count > max_size()) throw ExceptionCT{"Resize parameter larger than max size"};
		if (size() < count) {
			for (size_type i = 0; i < count - size(); ++i) {
				data_[i] = val;
			}
		}
		size_ = count;
	}

	template <typename T2, std::size_t Size2>
	constexpr void swap(static_vector<T2, Size2>& other) {
		size_type i = 0;
		while (i < size() && i < other.size()) {
			value_type temp = data_[i];
			data_[i] = other[i];
			other[i++] = temp;
		}
	}

private:
	std::size_t size_;
	std::array<T, Size> data_;
};

template <typename T, std::size_t Size>
constexpr bool operator==(const static_vector<T, Size>& lhs, const static_vector<T, Size>& rhs) {
	using vec_t = static_vector<T, Size>;
	typename vec_t::size_type i = 0;
	while (i < Size) {
		if (lhs[i] != rhs[i++]) return false;
	}
	return true;
}

template <typename T, std::size_t Size>
constexpr bool operator!=(const static_vector<T, Size>& lhs, const static_vector<T, Size>& rhs) {
	using vec_t = static_vector<T, Size>;
	typename vec_t::size_type i = 0;
	while (i < Size) {
		if (lhs[i] == rhs[i++]) return false;
	}
	return true;
}

template <typename T, std::size_t Size>
constexpr bool operator<(const static_vector<T, Size>& lhs, const static_vector<T, Size>& rhs) {
	using vec_t = static_vector<T, Size>;
	typename vec_t::size_type i = 0;
	while (i < Size) {
		if (lhs[i] >= rhs[i++]) return false;
	}
	return true;
}

template <typename T, std::size_t Size>
constexpr bool operator>(const static_vector<T, Size>& lhs, const static_vector<T, Size>& rhs) {
	using vec_t = static_vector<T, Size>;
	typename vec_t::size_type i = 0;
	while (i < Size) {
		if (lhs[i] <= rhs[i++]) return false;
	}
	return true;
}

template <typename T, std::size_t Size>
constexpr bool operator<=(const static_vector<T, Size>& lhs, const static_vector<T, Size>& rhs) {
	using vec_t = static_vector<T, Size>;
	typename vec_t::size_type i = 0;
	while (i < Size) {
		if (lhs[i] > rhs[i++]) return false;
	}
	return true;
}

template <typename T, std::size_t Size>
constexpr bool operator>=(const static_vector<T, Size>& lhs, const static_vector<T, Size>& rhs) {
	using vec_t = static_vector<T, Size>;
	typename vec_t::size_type i = 0;
	while (i < Size) {
		if (lhs[i] < rhs[i++]) return false;
	}
	return true;
}

}	 // namespace cui

#endif	  // CUI_COMPILE_TIME_STATIC_VECTOR_HPP
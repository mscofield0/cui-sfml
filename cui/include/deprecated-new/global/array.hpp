#ifndef CUI_ARRAY_HPP
#define CUI_ARRAY_HPP

#include <cstdint>
#include <cstddef>
#include <iterator>

#include <compile_time/exception.hpp>

namespace cui {

template <typename T, std::size_t Size>
class Array
{
public:
	using value_type = T;
	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;
	using reference = value_type&;
	using const_reference = const value_type&;
	using pointer = value_type*;
	using const_pointer = const value_type*;
	using iterator = value_type*;
	using const_iterator = const value_type*;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

	constexpr Array() : data_{0} {}
	constexpr Array(const T& val) : data_{val} {}

	template <typename... Args>
	constexpr Array(Args... args) : data_{args...} {}

	// Capacity

	constexpr size_type size() const noexcept {
		return Size;
	}

	constexpr bool empty() const noexcept {
		return size() == 0;
	}

	constexpr size_type max_size() const noexcept {
		return Size;
	}

	// Element access

	constexpr reference at(size_t index) {
		if (index >= size()) throw ExceptionCT{"Out of range"};
		return data_[index];
	}

	constexpr const_reference at(size_t index) const {
		return data_[index];
	}

	constexpr reference operator[](size_t index) noexcept {
		return data_[index];
	}

	constexpr const_reference operator[](size_t index) const noexcept {
		return data_[index];
	}

	constexpr reference front() noexcept {
		return data_[0];
	}

	constexpr const_reference front() const noexcept {
		return data_[0];
	}

	constexpr reference back() noexcept {
		return data_[size() - 1];
	}

	constexpr const_reference back() const noexcept {
		return data_[size() - 1];
	}

	constexpr pointer data() noexcept {
		return data_;
	}

	constexpr const_pointer data() const noexcept {
		return data_;
	}

	constexpr iterator begin() noexcept {
		return data_;
	}

	constexpr const_iterator begin() const noexcept {
		return data_;
	}

	constexpr const_iterator cbegin() const noexcept {
		return data_;
	}

	constexpr iterator end() noexcept {
		return data_ + size();
	}

	constexpr const_iterator end() const noexcept {
		return data_ + size();
	}

	constexpr const_iterator cend() const noexcept {
		return data_ + size();
	}

	constexpr void fill(const T& value) {
		for (auto it = begin(); it < end(); ++it) {
			*it = value;
		}
	}

	constexpr void swap(Array& other) noexcept {
		auto a = begin();
		auto b = other.begin();
		while (a != end()) {
			value_type temp = *a;
			*a = *b;
			*b = temp;
			++a;
			++b;
		}
	}
	
protected:
	T data_[Size];
};

}	 // namespace cui

#endif
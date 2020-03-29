#ifndef CUI_VECTOR_HPP
#define CUI_VECTOR_HPP

#include <vector>
#include <initializer_list>

#include <utils/forward.hpp>
#include <utils/move.hpp>

namespace cui {

template <typename T>
class Vector
{
	template <typename TT>
	friend bool operator==(const Vector<TT>& lhs, const Vector<TT>& rhs);

	template <typename TT>
	friend bool operator!=(const Vector<TT>& lhs, const Vector<TT>& rhs);

	template <typename TT>
	friend bool operator>(const Vector<TT>& lhs, const Vector<TT>& rhs);

	template <typename TT>
	friend bool operator>=(const Vector<TT>& lhs, const Vector<TT>& rhs);

	template <typename TT>
	friend bool operator<(const Vector<TT>& lhs, const Vector<TT>& rhs);

	template <typename TT>
	friend bool operator<=(const Vector<TT>& lhs, const Vector<TT>& rhs);

public:
	using vec_t = std::vector<T>;
	using value_type = typename vec_t::value_type;
	using size_type = typename vec_t::size_type;
	using difference_type = typename vec_t::difference_type;
	using reference = typename vec_t::reference;
	using const_reference = typename vec_t::const_reference;
	using pointer = typename vec_t::pointer;
	using const_pointer = typename vec_t::const_pointer;
	using iterator = typename vec_t::iterator;
	using const_iterator = typename vec_t::const_iterator;
	using reverse_iterator = typename vec_t::reverse_iterator;
	using const_reverse_iterator = typename vec_t::const_reverse_iterator;

	Vector() : vec_{} {}
	Vector(size_type count, const_reference value = value_type()) : vec_(count, value) {}
	template <typename InputIt>
	explicit Vector(InputIt first, InputIt last) : vec_(first, last) {}
	Vector(std::initializer_list<T> init) : vec_(init) {}
	template <typename... Args>
	Vector(Args&&... args) : vec_{cui::forward<Args>(cui::move(args))...} {}

	auto empty() const noexcept {
		return vec_.empty();
	}

	auto size() const noexcept {
		return vec_.size();
	}

	auto max_size() const noexcept {
		return vec_.max_size();
	}

	auto reserve(size_type new_cap) {
		return vec_.reserve(new_cap);
	}

	auto capacity() const noexcept {
		return vec_.capacity();
	}

	auto shrink_to_fit() noexcept {
		return vec_.shrink_to_fit();
	}

	reference at(size_type idx) {
		return vec_.at(idx);
	}

	const_reference at(size_type idx) const {
		return vec_.at(idx);
	}

	reference operator[](size_type idx) noexcept {
		return vec_[idx];
	}

	const_reference operator[](size_type idx) const noexcept {
		return vec_[idx];
	}

	reference front() {
		return vec_.front();
	}

	const_reference front() const {
		return vec_.front();
	}

	reference back() {
		return vec_.back();
	}

	const_reference back() const {
		return vec_.back();
	}

	pointer data() noexcept {
		return vec_.data();
	}

	const_pointer data() const noexcept {
		return vec_.data();
	}

	iterator begin() noexcept {
		return vec_.begin();
	}

	const_iterator begin() const noexcept {
		return vec_.begin();
	}

	const_iterator cbegin() const noexcept {
		return vec_.cbegin();
	}

	iterator end() noexcept {
		return vec_.end();
	}

	const_iterator end() const noexcept {
		return vec_.end();
	}

	const_iterator cend() const noexcept {
		return vec_.cend();
	}

	reverse_iterator rbegin() noexcept {
		return vec_.rbegin();
	}

	const_reverse_iterator rbegin() const noexcept {
		return vec_.rbegin();
	}

	const_reverse_iterator crbegin() const noexcept {
		return vec_.crbegin();
	}

	reverse_iterator rend() noexcept {
		return vec_.rend();
	}

	const_reverse_iterator rend() const noexcept {
		return vec_.rend();
	}

	const_reverse_iterator crend() const noexcept {
		return vec_.crbegin();
	}

	auto clear() noexcept {
		return vec_.clear();
	}

	auto insert(const_iterator pos, const_reference value) {
		return vec_.insert(pos, value);
	}

	auto insert(const_iterator pos, value_type&& value) {
		return vec_.insert(pos, cui::move(value));
	}

	auto insert(iterator pos, size_type count, const_reference value) {
		return vec_.insert(pos, count, value);
	}

	auto insert(const_iterator pos, size_type count, const_reference value) {
		return vec_.insert(pos, count, value);
	}

	template <typename InputIt>
	auto insert(iterator pos, InputIt first, InputIt last) {
		return vec_.insert(pos, first, last);
	}

	template <typename InputIt>
	auto insert(const_iterator pos, InputIt first, InputIt last) {
		return vec_.insert(pos, first, last);
	}

	auto insert(const_iterator pos, std::initializer_list<T> ilist) {
		return vec_.insert(pos, ilist);
	}

	template <typename... Args>
	auto emplace(const_iterator pos, Args&&... args) {
		return vec_.emplace(pos, cui::move(args)...);
	}

	auto erase(iterator pos) {
		return vec_.erase(pos);
	}

	auto erase(const_iterator pos) {
		return vec_.erase(pos);
	}

	auto erase(iterator first, iterator last) {
		return vec_.erase(first, last);
	}

	auto erase(const_iterator first, const_iterator last) {
		return vec_.erase(first, last);
	}

	auto push_back(const_reference value) {
		return vec_.push_back(value);
	}

	auto push_back(value_type&& value) {
		return vec_.push_back(cui::move(value));
	}

	template <typename... Args>
	auto emplace_back(Args&&... args) {
		return vec_.emplace_back(cui::move(args)...);
	}

	auto pop_back() {
		return vec_.pop_back();
	}

	auto resize(size_type count, value_type value) {
		return vec_.resize(count, value);
	}

	auto resize(size_type count) {
		return vec_.resize(count);
	}

	auto resize(size_type count, const value_type& value) {
		return vec_.resize(count, value);
	}

	auto swap(Vector& other) noexcept {
		return vec_.swap(other.vec_);
	}

private:
	std::vector<T> vec_;
};

template <typename T>
bool operator==(const Vector<T>& lhs, const Vector<T>& rhs) {
	return lhs.vec_ == rhs.vec_;
}

template <typename T>
bool operator!=(const Vector<T>& lhs, const Vector<T>& rhs) {
	return lhs.vec_ != rhs.vec_;
}

template <typename T>
bool operator<(const Vector<T>& lhs, const Vector<T>& rhs) {
	return lhs.vec_ == rhs.vec_;
}

template <typename T>
bool operator<=(const Vector<T>& lhs, const Vector<T>& rhs) {
	return lhs.vec_ == rhs.vec_;
}

template <typename T>
bool operator>(const Vector<T>& lhs, const Vector<T>& rhs) {
	return lhs.vec_ == rhs.vec_;
}

template <typename T>
bool operator>=(const Vector<T>& lhs, const Vector<T>& rhs) {
	return lhs.vec_ == rhs.vec_;
}

}	 // namespace cui

#endif	  // CUI_VECTOR_HPP
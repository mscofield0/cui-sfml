#ifndef CUI_LIST_HPP
#define CUI_LIST_HPP

#include <list>
#include <initializer_list>
#include <exception>

#include <utils/forward.hpp>
#include <utils/move.hpp>

namespace cui {

template <typename T>
class List
{
	template <typename TT>
	friend bool operator==(const List<TT>& lhs, const List<TT>& rhs);

	template <typename TT>
	friend bool operator!=(const List<TT>& lhs, const List<TT>& rhs);

	template <typename TT>
	friend bool operator>(const List<TT>& lhs, const List<TT>& rhs);

	template <typename TT>
	friend bool operator>=(const List<TT>& lhs, const List<TT>& rhs);

	template <typename TT>
	friend bool operator<(const List<TT>& lhs, const List<TT>& rhs);

	template <typename TT>
	friend bool operator<=(const List<TT>& lhs, const List<TT>& rhs);

public:
	using list_t = std::vector<T>;
	using value_type = typename list_t::value_type;
	using size_type = typename list_t::size_type;
	using difference_type = typename list_t::difference_type;
	using reference = typename list_t::reference;
	using const_reference = typename list_t::const_reference;
	using pointer = typename list_t::pointer;
	using const_pointer = typename list_t::const_pointer;
	using iterator = typename list_t::iterator;
	using const_iterator = typename list_t::const_iterator;
	using reverse_iterator = typename list_t::reverse_iterator;
	using const_reverse_iterator = typename list_t::const_reverse_iterator;

	List() : list_{} {}
	List(size_type count, const_reference value = value_type()) : list_(count, value) {}
	template <typename InputIt>
	explicit List(InputIt first, InputIt last) : list_(first, last) {}
	List(std::initializer_list<T> init) : list_(init) {}
	template <typename... Args>
	List(Args&&... args) : list_{cui::forward<Args>(args)...} {}

	auto empty() const noexcept {
		return list_.empty();
	}

	auto size() const noexcept {
		return list_.size();
	}

	auto max_size() const noexcept {
		return list_.max_size();
	}

	reference at(size_type idx) {
		if (idx >= size()) throw std::length_error("Index larger than size");
		auto it = list_.begin();
		for(size_type i = 0; i < idx; ++i) ++it;
		return *it;
	}

	const_reference at(size_type idx) const {
		if (idx >= size()) throw std::length_error("Index larger than size");
		auto it = list_.begin();
		for(size_type i = 0; i < idx; ++i) ++it;
		return *it;
	}

	reference operator[](size_type idx) noexcept {
		auto it = list_.begin();
		for(size_type i = 0; i < idx; ++i) ++it;
		return *it;
	}

	const_reference operator[](size_type idx) const noexcept {
		auto it = list_.begin();
		for(size_type i = 0; i < idx; ++i) ++it;
		return *it;
	}

	reference front() {
		return list_.front();
	}

	const_reference front() const {
		return list_.front();
	}

	reference back() {
		return list_.back();
	}

	const_reference back() const {
		return list_.back();
	}

	iterator begin() noexcept {
		return list_.begin();
	}

	const_iterator begin() const noexcept {
		return list_.begin();
	}

	const_iterator cbegin() const noexcept {
		return list_.cbegin();
	}

	iterator end() noexcept {
		return list_.end();
	}

	const_iterator end() const noexcept {
		return list_.end();
	}

	const_iterator cend() const noexcept {
		return list_.cend();
	}

	reverse_iterator rbegin() noexcept {
		return list_.rbegin();
	}

	const_reverse_iterator rbegin() const noexcept {
		return list_.rbegin();
	}

	const_reverse_iterator crbegin() const noexcept {
		return list_.crbegin();
	}

	reverse_iterator rend() noexcept {
		return list_.rend();
	}

	const_reverse_iterator rend() const noexcept {
		return list_.rend();
	}

	const_reverse_iterator crend() const noexcept {
		return list_.crbegin();
	}

	auto clear() noexcept {
		return list_.clear();
	}

	auto insert(const_iterator pos, const_reference value) {
		return list_.insert(pos, value);
	}

	auto insert(const_iterator pos, value_type&& value) {
		return list_.insert(pos, cui::move(value));
	}

	auto insert(iterator pos, size_type count, const_reference value) {
		return list_.insert(pos, count, value);
	}

	auto insert(const_iterator pos, size_type count, const_reference value) {
		return list_.insert(pos, count, value);
	}

	template <typename InputIt>
	auto insert(iterator pos, InputIt first, InputIt last) {
		return list_.insert(pos, first, last);
	}

	template <typename InputIt>
	auto insert(const_iterator pos, InputIt first, InputIt last) {
		return list_.insert(pos, first, last);
	}

	auto insert(const_iterator pos, std::initializer_list<T> ilist) {
		return list_.insert(pos, ilist);
	}

	template <typename... Args>
	auto emplace(const_iterator pos, Args&&... args) {
		return list_.emplace(pos, cui::move(args)...);
	}

	auto erase(iterator pos) {
		return list_.erase(pos);
	}

	auto erase(const_iterator pos) {
		return list_.erase(pos);
	}

	auto erase(iterator first, iterator last) {
		return list_.erase(first, last);
	}

	auto erase(const_iterator first, const_iterator last) {
		return list_.erase(first, last);
	}

	auto push_back(const_reference value) {
		return list_.push_back(value);
	}

	auto push_back(value_type&& value) {
		return list_.push_back(cui::move(value));
	}

	template <typename... Args>
	auto emplace_back(Args&&... args) {
		return list_.emplace_back(cui::move(args)...);
	}

	auto pop_back() {
		return list_.pop_back();
	}

	auto resize(size_type count, value_type value) {
		return list_.resize(count, value);
	}

	auto resize(size_type count) {
		return list_.resize(count);
	}

	auto resize(size_type count, const value_type& value) {
		return list_.resize(count, value);
	}

	auto swap(List& other) noexcept {
		return list_.swap(other.list_);
	}

	void merge(List&& other) {
		list_.merge(cui::move(other));
	}

	template <typename Compare>
	void merge(List&& other, Compare comp) {
		list_.merge(cui::move(other), comp);
	}

	void splice(const_iterator pos, List&& other) {
		list_.splice(pos, cui::move(other));
	}

	void splice(const_iterator pos, List&& other, const_iterator it) {
		list_.splice(pos, cui::move(other), it);
	}

	void splice(const_iterator pos, List&& other, const_iterator first, const_iterator last) {
		list_.splice(pos, cui::move(other), first, last);
	}

	void remove(const T& value) {
		list_.remove(value);
	}

	template <class UnaryPredicate>
	void remove_if(UnaryPredicate p) {
		list_.remove_if(cui::move(p));
	}

	void reverse() noexcept {
		list_.reverse();
	}

	void unique() {
		list_.unique();
	}

	template <class BinaryPredicate>
	void unique(BinaryPredicate p) {
		list_.unique(cui::move(p));
	}

private:
	std::list<T> list_;
};

template <typename T>
bool operator==(const List<T>& lhs, const List<T>& rhs) {
	return lhs.list_ == rhs.list_;
}

template <typename T>
bool operator!=(const List<T>& lhs, const List<T>& rhs) {
	return lhs.list_ != rhs.list_;
}

template <typename T>
bool operator<(const List<T>& lhs, const List<T>& rhs) {
	return lhs.list_ < rhs.list_;
}

template <typename T>
bool operator<=(const List<T>& lhs, const List<T>& rhs) {
	return lhs.list_ <= rhs.list_;
}

template <typename T>
bool operator>(const List<T>& lhs, const List<T>& rhs) {
	return lhs.list_ > rhs.list_;
}

template <typename T>
bool operator>=(const List<T>& lhs, const List<T>& rhs) {
	return lhs.list_ >= rhs.list_;
}

}	 // namespace cui

#endif	  // CUI_LIST_HPP
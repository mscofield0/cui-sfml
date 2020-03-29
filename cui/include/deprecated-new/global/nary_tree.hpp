#ifndef CUI_GLOBAL_NARY_TREE_HPP
#define CUI_GLOBAL_NARY_TREE_HPP

#include <vector>
#include <functional>

#include <compile_time/exception.hpp>

namespace cui {

template <typename T>
class NaryTree;

template <typename T>
class NodeIterator
{
public:
	using value_type = T;
	using size_type = std::size_t;
	using reference = value_type&;
	using const_reference = const value_type&;
	using iterator = NodeIterator;

	using tree_t = NaryTree<T>;
	using ref_tree = std::reference_wrapper<NaryTree<T>>;

	NodeIterator(tree_t& p_tree, size_type idx) : tree_{std::ref(p_tree)}, index_{idx} {}
	NodeIterator(const ref_tree& p_ref_wrap_tree, size_type idx) : tree_{p_ref_wrap_tree}, index_{idx} {}

	auto index() const noexcept {
		return index_;
	}

	auto& data() {
		return tree().nodes().at(index());
	}

	auto& data() const {
		return tree().nodes().at(index());
	}

	auto& children() const {
		return tree().children().at(index());
	}

	auto depth() const {
		return tree().depths().at(index());
	}

	auto& operator*() noexcept {
		return (*this);
	}

	auto& operator++() {
		++index_;
		return (*this);
	}

	auto& operator--() {
		--index_;
		return (*this);
	}

	auto operator++(int) {
		++index_;
		return iterator{tree_, index() - 1};
	}

	auto operator--(int) {
		--index_;
		return iterator{tree_, index() + 1};
	}

	auto& operator+=(size_type val) {
		index_ += val;
		return (*this);
	}

	auto& operator-=(size_type val) {
		index_ -= val;
		return (*this);
	}

	auto operator==(iterator rhs) const noexcept {
		return index() == rhs.index();
	}

	auto operator!=(iterator rhs) const noexcept {
		return index() != rhs.index();
	}

	void add_child(reference val) {
		tree().add_node(val, index());
	}

	void add_child(const_reference val) {
		tree().add_node(val, index());
	}

	void add_child(value_type&& val) {
		tree().add_node(std::move(val), index());
	}

private:
	auto& tree() noexcept {
		return tree_.get();
	}

	auto& tree() const noexcept {
		return tree_.get();
	}

	ref_tree tree_;
	size_type index_;
};

template <typename T>
class ConstNodeIterator
{
public:
	using value_type = T;
	using size_type = std::size_t;
	using reference = value_type&;
	using const_reference = const value_type&;
	using iterator = ConstNodeIterator;
	using const_iterator = const ConstNodeIterator;

	using tree_t = NaryTree<T>;
	using ref_tree = std::reference_wrapper<const NaryTree<T>>;

	ConstNodeIterator(const tree_t& p_tree, size_type idx) : tree_{std::cref(p_tree)}, index_{idx} {}
	ConstNodeIterator(const ref_tree& p_ref_wrap_tree, size_type idx) : tree_{p_ref_wrap_tree}, index_{idx} {}

	auto index() const noexcept {
		return index_;
	}

	auto& data() const {
		return tree().nodes().at(index());
	}

	auto& children() const {
		return tree().children().at(index());
	}

	auto depth() const {
		return tree().depths().at(index());
	}

	auto& operator*() const noexcept {
		return (*this);
	}

	auto& operator++() {
		++index_;
		return (*this);
	}

	auto& operator--() {
		--index_;
		return (*this);
	}

	auto operator++(int) {
		++index_;
		return iterator{tree_, index() - 1};
	}

	auto operator--(int) {
		--index_;
		return iterator{tree_, index() + 1};
	}

	auto& operator+=(size_type val) {
		index_ += val;
		return (*this);
	}

	auto& operator-=(size_type val) {
		index_ -= val;
		return (*this);
	}

	auto operator==(iterator rhs) const noexcept {
		return index() == rhs.index();
	}

	auto operator!=(iterator rhs) const noexcept {
		return index() != rhs.index();
	}

private:
	auto& tree() const noexcept {
		return tree_.get();
	}

	ref_tree tree_;
	size_type index_;
};

template <typename T>
class NaryTree
{
	friend class NodeIterator<T>;
	friend class ConstNodeIterator<T>;

public:
	using value_type = T;
	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;
	using reference = value_type&;
	using const_reference = const value_type&;
	using pointer = value_type*;
	using const_pointer = const value_type*;
	using iterator = NodeIterator<T>;
	using const_iterator = ConstNodeIterator<T>;

	using node_vec = std::vector<value_type>;
	using child_vec = std::vector<std::vector<size_type>>;
	using depth_vec = std::vector<size_type>;

	auto length() const noexcept {
		return nodes_.size();
	}

	auto begin() noexcept {
		return iterator(*this, 0);
	}

	auto begin() const noexcept {
		return const_iterator(*this, 0);
	}

	auto cbegin() const noexcept {
		return const_iterator(*this, 0);
	}

	auto end() noexcept {
		return iterator(*this, length());
	}

	auto end() const noexcept {
		return const_iterator(*this, length());
	}

	auto cend() const noexcept {
		return const_iterator(*this, length());
	}

	auto operator[](size_type idx) noexcept {
		return iterator(*this, idx);
	}

	auto operator[](size_type idx) const noexcept {
		return const_iterator(*this, idx);
	}

	auto at(size_type idx) noexcept {
		if (idx >= length()) throw ExceptionCT{"Out of range"};
		return iterator(*this, idx);
	}

	auto at(size_type idx) const noexcept {
		if (idx >= length()) throw ExceptionCT{"Out of range"};
		return const_iterator(*this, idx);
	}

	void add_node(const_reference val) {
		nodes_.push_back(val);
		children_.emplace_back();
		depths_.push_back(0);
	}

	void add_node(value_type&& val) {
		nodes_.push_back(std::move(val));
		children_.emplace_back();
		depths_.push_back(0);
	}

	void add_node(const_reference val, size_type idx) {
		children_[idx].push_back(length());
		nodes_.push_back(val);
		children_.emplace_back();
		depths_.push_back(depths_[idx] + 1);
	}

	void add_node(value_type&& val, size_type idx) {
		children_[idx].push_back(length());
		nodes_.push_back(std::move(val));
		children_.emplace_back();
		depths_.push_back(depths_[idx] + 1);
	}

	void remove_node(size_type idx) {
		children().erase(children().begin() + idx);
		for (auto it = children().begin(); it != children().end(); ++it) {
			for (auto it_ = it->begin(); it_ != it->end(); ++it_) {
				if (*it_ == idx) {
					it->erase(it_);
				}
			}
		}

		depths().erase(idx);
		nodes().erase(idx);
	}

	void pop_node() {
		children().pop_back();
		const auto idx = children().size();
		for (auto it = children().begin(); it != children().end(); ++it) {
			for (auto it_ = it->begin(); it_ != it->end(); ++it_) {
				if (*it_ == idx) {
					it->erase(it_);
				}
			}
		}

		depths().pop_back();
		nodes().pop_back();
	}

protected:
	auto& nodes() noexcept {
		return nodes_;
	}

	auto& nodes() const noexcept {
		return nodes_;
	}

	auto& children() noexcept {
		return children_;
	}

	auto& children() const noexcept {
		return children_;
	}

	auto& depths() noexcept {
		return depths_;
	}

	auto& depths() const noexcept {
		return depths_;
	}

	node_vec nodes_;
	child_vec children_;
	depth_vec depths_;
};

}	 // namespace cui

#endif	  // CUI_GLOBAL_NARY_TREE_HPP
#ifndef CUI_NARY_TREE_HPP
#define CUI_NARY_TREE_HPP

#include <vector>
#include <functional>

#include <containers/detail/nary_tree/node_iterator.hpp>
#include <containers/detail/nary_tree/const_node_iterator.hpp>
#include <aliases.hpp>

namespace cui {

template <typename T>
class NaryTree
{
	friend class NodeIterator<T>;
	friend class ConstNodeIterator<T>;

public:
	using value_type = T;
	using size_type = u64;
	using difference_type = i64;
	using reference = value_type&;
	using const_reference = const value_type&;
	using pointer = value_type*;
	using const_pointer = const value_type*;
	using iterator = NodeIterator<T>;
	using const_iterator = ConstNodeIterator<T>;

	using node_vec = std::vector<value_type>;
	using child_vec = std::vector<std::vector<size_type>>;
	using depth_vec = std::vector<size_type>;

	[[nodiscard]] auto length() const noexcept -> size_type {
		return nodes_.size();
	}

	[[nodiscard]] auto begin() noexcept -> iterator {
		return iterator(*this, 0);
	}

	[[nodiscard]] auto begin() const noexcept -> const_iterator {
		return const_iterator(*this, 0);
	}

	[[nodiscard]] auto cbegin() const noexcept -> const_iterator {
		return const_iterator(*this, 0);
	}

	[[nodiscard]] auto end() noexcept -> iterator {
		return iterator(*this, length());
	}

	[[nodiscard]] auto end() const noexcept -> const_iterator {
		return const_iterator(*this, length());
	}

	[[nodiscard]] auto cend() const noexcept -> const_iterator {
		return const_iterator(*this, length());
	}

	[[nodiscard]] auto operator[](size_type idx) noexcept -> iterator {
		return iterator(*this, idx);
	}

	[[nodiscard]] auto operator[](size_type idx) const noexcept -> const_iterator {
		return const_iterator(*this, idx);
	}

	auto at(size_type idx) noexcept -> iterator {
		if (idx >= length()) throw "Out of range";
		return iterator(*this, idx);
	}

	auto at(size_type idx) const noexcept -> const_iterator {
		if (idx >= length()) throw "Out of range";
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

	[[nodiscard]] auto nodes() const noexcept -> const node_vec& {
		return nodes_;
	}

	[[nodiscard]] auto children() const noexcept -> const child_vec& {
		return children_;
	}

	[[nodiscard]] auto depths() const noexcept -> const depth_vec& {
		return depths_;
	}

	[[nodiscard]] auto nodes() noexcept -> node_vec& {
		return nodes_;
	}

	[[nodiscard]] auto children() noexcept -> child_vec& {
		return children_;
	}

	[[nodiscard]] auto depths() noexcept -> depth_vec& {
		return depths_;
	}

protected:
	node_vec nodes_;
	child_vec children_;
	depth_vec depths_;
};

}	 // namespace cui

#endif	  // CUI_NARY_TREE_HPP
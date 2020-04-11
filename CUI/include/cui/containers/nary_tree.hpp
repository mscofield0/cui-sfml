#ifndef CUI_NARY_TREE_HPP
#define CUI_NARY_TREE_HPP

#include <vector>
#include <functional>

#include <containers/detail/nary_tree/node.hpp>
#include <aliases.hpp>

namespace cui {

template <typename T>
class NaryTree
{
public:
	using node_type = Node<T>;
	using data_type = node_type::data_type;
	using size_type = u64;
	using difference_type = i64;
	using iterator = std::vector<node_type>::iterator;
	using const_iterator = std::vector<node_type>::const_iterator;

	[[nodiscard]] auto length() const noexcept -> size_type {
		return vec_.size();
	}

	[[nodiscard]] auto begin() noexcept -> iterator {
		return vec_.begin();
	}

	[[nodiscard]] auto begin() const noexcept -> const_iterator {
		return vec_.cbegin();
	}

	[[nodiscard]] auto cbegin() const noexcept -> const_iterator {
		return vec_.cbegin();
	}

	[[nodiscard]] auto end() noexcept -> iterator {
		return vec_.end();
	}

	[[nodiscard]] auto end() const noexcept -> const_iterator {
		return vec_.cend();
	}

	[[nodiscard]] auto cend() const noexcept -> const_iterator {
		return vec_.cend();
	}

	[[nodiscard]] auto operator[](size_type idx) noexcept -> iterator {
		return vec_[idx];
	}

	[[nodiscard]] auto operator[](size_type idx) const noexcept -> const_iterator {
		return vec_[idx];
	}

	auto at(size_type idx) noexcept -> iterator {
		return vec_.at(idx);
	}

	auto at(size_type idx) const noexcept -> const_iterator {
		return vec_.at(idx);
	}

	void add_node(const_reference val) {
		vec_.push_back()
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
	std::vector<node_type> vec_;
};

}	 // namespace cui

#endif	  // CUI_NARY_TREE_HPP
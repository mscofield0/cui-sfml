#ifndef CUI_NARY_TREE_CONST_NODE_ITERATOR_HPP
#define CUI_NARY_TREE_CONST_NODE_ITERATOR_HPP

#include <vector>
#include <functional>

#include <containers/nary_tree.hpp>
#include <aliases.hpp>

namespace cui {

template <typename T>
class NaryTree;

template <typename T>
class ConstNodeIterator
{
public:
	using value_type = T;
	using size_type = u64;
	using reference = value_type&;
	using const_reference = const value_type&;
	using tree_t = NaryTree<T>;
	using vec_t = std::vector<size_type>;
	using iterator = ConstNodeIterator;

	using ref_tree = std::reference_wrapper<const NaryTree<T>>;

	ConstNodeIterator(const tree_t& p_tree, size_type idx) : tree_{std::cref(p_tree)}, index_{idx} {}
	ConstNodeIterator(const ref_tree& p_ref_wrap_tree, size_type idx) : tree_{p_ref_wrap_tree}, index_{idx} {}

	[[nodiscard]] auto index() const noexcept -> size_type {
		return index_;
	}

	[[nodiscard]] auto data() const -> const value_type& {
		return tree().nodes().at(index());
	}

	[[nodiscard]] auto children() const -> const vec_t& {
		return tree().children().at(index());
	}

	[[nodiscard]] auto depth() const -> size_type {
		return tree().depths().at(index());
	}

	[[nodiscard]] auto operator*() const noexcept -> const iterator& {
		return (*this);
	}

	auto operator++() -> const iterator& {
		++index_;
		return (*this);
	}

	auto operator--() -> const iterator& {
		--index_;
		return (*this);
	}

	auto operator++(int) -> const iterator {
		++index_;
		return iterator{tree_, index() - 1};
	}

	auto operator--(int) -> const iterator {
		--index_;
		return iterator{tree_, index() + 1};
	}

	auto operator+=(size_type val) -> const iterator& {
		index_ += val;
		return (*this);
	}

	auto operator-=(size_type val) -> const iterator& {
		index_ -= val;
		return (*this);
	}

	[[nodiscard]] bool operator==(iterator rhs) const noexcept {
		return index() == rhs.index();
	}

	[[nodiscard]] bool operator!=(iterator rhs) const noexcept {
		return index() != rhs.index();
	}

private:
	[[nodiscard]] auto tree() const noexcept -> const tree_t& {
		return tree_.get();
	}

	ref_tree tree_;
	size_type index_;
};

}	 // namespace cui

#endif	  // CUI_NARY_TREE_CONST_NODE_ITERATOR_HPP
#ifndef CUI_SFML_ITERATOR_PAIR_HPP
#define CUI_SFML_ITERATOR_PAIR_HPP

#include <vector>
#include <cui/containers/vector.hpp>
#include <cui/containers/detail/nary_tree/node.hpp>
#include <cui/visual/node.hpp>
#include <render_context/visual_element.hpp>

namespace cui {

class IteratorPair
{
public:
	using T1 = nary::Node<Node>;
	using T2 = VisualElement;
	using iterator1_t = typename std::vector<T1>::iterator;
	using const_iterator1_t = typename std::vector<T1>::const_iterator;
	using iterator2_t = typename Vector<T2>::iterator;
	using const_iterator2_t = typename Vector<T2>::const_iterator;
	using size_type = std::size_t;

	IteratorPair(const_iterator1_t it1, iterator2_t it2) : it1_(it1), it2_(it2) {}

	void operator=(IteratorPair& it) {
		T2 t = *it2_;
		*it2_ = *it.it2_;
		*it.it2_ = t;
	}

	auto operator*() const noexcept -> const T1& {
		return *it1_;
	}

	auto operator++() noexcept -> IteratorPair& {
		++it1_;
		++it2_;
		return (*this);
	}

	auto operator--() noexcept -> IteratorPair& {
		--it1_;
		--it2_;
		return (*this);
	}

	auto operator++(int) noexcept -> IteratorPair {
		++it1_;
		++it2_;
		return {it1_ - 1, it2_ - 1};
	}

	auto operator--(int) noexcept -> IteratorPair {
		--it1_;
		--it2_;
		return {it1_ + 1, it2_ + 1};
	}

	auto operator+=(size_type val) noexcept -> IteratorPair& {
		it1_ += val;
		it2_ += val;
		return (*this);
	}

	auto operator-=(size_type val) noexcept -> IteratorPair& {
		it1_ -= val;
		it2_ -= val;
		return (*this);
	}

	[[nodiscard]] bool operator==(const IteratorPair& it) const noexcept {
		return it1_ == it.it1_ && it2_ == it.it2_;
	}

	[[nodiscard]] bool operator!=(const IteratorPair& it) const noexcept {
		return !this->operator==(it);
	}

private:
	const_iterator1_t it1_;
	iterator2_t it2_;
};

}	 // namespace cui

#endif	  // CUI_SFML_ITERATOR_PAIR_HPP
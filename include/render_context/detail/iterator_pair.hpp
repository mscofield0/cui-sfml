#ifndef CUI_SFML_ITERATOR_PAIR_HPP
#define CUI_SFML_ITERATOR_PAIR_HPP

#include <vector>
#include <utility>
#include <functional>
#include <cui/containers/vector.hpp>
#include <cui/containers/detail/nary_tree/node.hpp>
#include <cui/visual/node.hpp>
#include <render_context/visual_element.hpp>

namespace cui {

class ProxyObject
{
public:
	using size_type = std::size_t;
	using T1 = size_type;
	using T2 = VisualElement;
	using ref1_t = std::reference_wrapper<T1>;
	using ref2_t = std::reference_wrapper<T2>;

	ProxyObject(T1& p_ref1, T2& p_ref2) : ref1_(std::ref(p_ref1)), ref2_(std::ref(p_ref2)) {}

	ProxyObject& operator=(const ProxyObject& obj) {
		ref1_ = obj.ref1_;
		ref2_ = obj.ref2_;
		return *this;
	}

	[[nodiscard]] bool operator==(const ProxyObject& obj) const noexcept {
		return ref1_ == obj.ref1_;
	}

	[[nodiscard]] bool operator!=(const ProxyObject& obj) const noexcept {
		return !this->operator==(obj);
	}

	[[nodiscard]] bool operator>(const ProxyObject& obj) const noexcept {
		return ref1_ > obj.ref1_;
	}

	[[nodiscard]] bool operator<(const ProxyObject& obj) const noexcept {
		return ref1_ < obj.ref1_;
	}

	[[nodiscard]] bool operator>=(const ProxyObject& obj) const noexcept {
		return ref1_ >= obj.ref1_;
	}

	[[nodiscard]] bool operator<=(const ProxyObject& obj) const noexcept {
		return ref1_ <= obj.ref1_;
	}

private:
	ref1_t ref1_;
	ref2_t ref2_;
};

class IteratorPair
{
public:
	using size_type = std::size_t;
	using value_type = ProxyObject;
	using difference_type = std::ptrdiff_t;
	using pointer = ProxyObject*;
	using reference = ProxyObject&;
	using iterator_category = std::random_access_iterator_tag;
	using T1 = size_type;
	using T2 = VisualElement;
	using iterator1_t = typename std::vector<T1>::iterator;
	using const_iterator1_t = typename std::vector<T1>::const_iterator;
	using iterator2_t = typename Vector<T2>::iterator;
	using const_iterator2_t = typename Vector<T2>::const_iterator;

	IteratorPair(iterator1_t it1, iterator2_t it2) : it1_(it1), it2_(it2), proxy_(*it1_, *it2_) {}

	auto operator*() noexcept -> ProxyObject& {
		proxy_ = ProxyObject(*it1_, *it2_);
		return proxy_;
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

	[[nodiscard]] auto operator-(const IteratorPair& it) const noexcept -> std::ptrdiff_t {
		return it1_ - it.it1_;
	}

	[[nodiscard]] auto operator-(int n) noexcept -> IteratorPair {
		return IteratorPair(it1_ - n, it2_ - n);
	}

	[[nodiscard]] auto operator+(int n) noexcept -> IteratorPair {
		return IteratorPair(it1_ + n, it2_ + n);
	}

	[[nodiscard]] bool operator==(const IteratorPair& it) const noexcept {
		return it1_ == it.it1_;
	}

	[[nodiscard]] bool operator!=(const IteratorPair& it) const noexcept {
		return !this->operator==(it);
	}

	[[nodiscard]] bool operator>(const IteratorPair& it) const noexcept {
		return it1_ > it.it1_;
	}

	[[nodiscard]] bool operator<(const IteratorPair& it) const noexcept {
		return it1_ < it.it1_;
	}

	[[nodiscard]] bool operator>=(const IteratorPair& it) const noexcept {
		return it1_ >= it.it1_;
	}

	[[nodiscard]] bool operator<=(const IteratorPair& it) const noexcept {
		return it1_ <= it.it1_;
	}

private:
	iterator1_t it1_;
	iterator2_t it2_;
	ProxyObject proxy_;
};

}	 // namespace cui

#endif	  // CUI_SFML_ITERATOR_PAIR_HPP
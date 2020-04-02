#ifndef CUI_STATIC_STACK_HPP
#define CUI_STATIC_STACK_HPP

#include <containers/static_vector.hpp>
#include <aliases.hpp>

namespace cui {

template <typename T, u64 MaxSize>
class StaticStack
{
public:
	using value_type = T;
	using size_type = u64;
	using reference = value_type&;
	using const_reference = const value_type&;

	constexpr StaticStack() : data_{} {}

	[[nodiscard]] constexpr auto size() const noexcept -> size_type {
		return data_.size();
	}

	[[nodiscard]] constexpr bool empty() const noexcept {
		return size() == 0;
	}

	[[nodiscard]] constexpr auto top() noexcept -> reference {
		return data_.back();
	}

	[[nodiscard]] constexpr auto top() const noexcept -> const_reference {
		return data_.back();
	}

	constexpr auto pop() -> value_type {
		const auto t = data_.back();
		data_.pop_back();
		return t;
	}

	constexpr void push(const_reference item) {
		data_.push_back(item);
	}

	constexpr void remove_top(size_type n) {
		if (n >= size()) throw "Cannot remove more elements than the size dictates";
		data_.erase(data_.end() - n - 1);
	}

	constexpr void clear() noexcept {
		data_.clear();
	}

private:
	StaticVector<T, MaxSize> data_;
};

}	 // namespace cui

#endif	  // CUI_STATIC_STACK_HPP
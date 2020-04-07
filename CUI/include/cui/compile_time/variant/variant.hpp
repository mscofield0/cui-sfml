#ifndef CUI_CT_VARIANT_HPP
#define CUI_CT_VARIANT_HPP

#include <aliases.hpp>

namespace cui::ct {

template <typename T1, typename T2>
class Variant
{
public:
	using type_1 = T1;
	using type_2 = T2;

	constexpr Variant(const type_1& a) noexcept : active_{true}, type_a_{a} {}
	constexpr Variant(const type_2& b) noexcept : active_{false}, type_b_{b} {}

	constexpr Variant& operator=(const Variant& x) noexcept {
		active_ = x.active_;
		if(active_) type_a_ = x.type_a_;
		else type_b_ = x.type_b_;
		return *this;
	}

	constexpr Variant& operator=(const type_1& a) noexcept {
		active_ = true;
		type_a_ = a;
		return *this;
	}

	constexpr Variant& operator=(const type_2& b) noexcept {
		active_ = false;
		type_b_ = b;
		return *this;
	}

	[[nodiscard]] constexpr bool is_type_a() const noexcept {
		return active_;
	}

	[[nodiscard]] constexpr bool is_type_b() const noexcept {
		return !active_;
	}

	[[nodiscard]] constexpr auto type_a() noexcept -> type_1& {
		return type_a_;
	}

	[[nodiscard]] constexpr auto type_a() const noexcept -> const type_1& {
		return type_a_;
	}

	[[nodiscard]] constexpr auto type_b() noexcept -> type_2& {
		return type_b_;
	}

	[[nodiscard]] constexpr auto type_b() const noexcept -> const type_2& {
		return type_b_;
	}

private:
	bool active_;

	union
	{
		T1 type_a_;
		T2 type_b_;
	};
};

}	 // namespace cui::ct

#endif	  // CUI_CT_VARIANT_HPP

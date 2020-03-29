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

	constexpr Variant(type_1 a) : active_{Types::Type1}, type_a_{a} {}
	constexpr Variant(type_2 b) : active_{Types::Type2}, type_b_{b} {}

	constexpr Variant& operator=(const type_1& a) {
		active_ = Types::Type1;
		type_a_ = a;
		return *this;
	}

	constexpr Variant& operator=(const type_2& b) {
		active_ = Types::Type2;
		type_b_ = b;
		return *this;
	}

	[[nodiscard]] constexpr bool is_type_a() const noexcept {
		return active_ == Types::Type1;
	}

	[[nodiscard]] constexpr bool is_type_b() const noexcept {
		return active_ == Types::Type2;
	}

	constexpr auto type_a() noexcept -> type_1& {
		return type_a_;
	}

	constexpr auto type_a() const noexcept -> const type_1& {
		return type_a_;
	}

	constexpr auto type_b() noexcept -> type_2& {
		return type_b_;
	}

	constexpr auto type_b() const noexcept -> const type_2& {
		return type_b_;
	}

private:
	enum class Types : byte
	{
		Type1,
		Type2
	} active_;

	union
	{
		T1 type_a_;
		T2 type_b_;
	};
};

}	 // namespace cui::ct

#endif	  // CUI_CT_VARIANT_HPP

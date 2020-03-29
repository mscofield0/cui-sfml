#ifndef CUI_COMPILE_TIME_VARIANT_HPP
#define CUI_COMPILE_TIME_VARIANT_HPP

namespace cui {

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

	constexpr type_1 type_a() const noexcept {
		return type_a_;
	}

	constexpr type_1& type_a() noexcept {
		return type_a_;
	}

	constexpr type_2 type_b() const noexcept {
		return type_b_;
	}

	constexpr type_2& type_b() noexcept {
		return type_b_;
	}

#ifndef CUI_ENABLE_CONSTEXPR_PARSING
	~Variant() {
		if (is_type_a()) type_a_.T1::~T1();
		else
			type_b_.T2::~T2();
	}
#endif

private:
	enum class Types
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

}	 // namespace cui

#endif	  // CUI_COMPILE_TIME_VARIANT_HPP

#ifndef CUI_VARIANT_HPP
#define CUI_VARIANT_HPP

#ifndef VARIANT_ACTIVE_MEMBER
#define VARIANT_ACTIVE_MEMBER(variant) variant.get_active_member<variant.active>()
#endif

namespace cui::ct {

template <typename T1, typename T2>
struct variant
{
	enum active_
	{
		T1_,
		T2_
	} active;

	union
	{
		T1 type1;
		T2 type2;
	};

	constexpr variant(T1 val) : active(T1_), type1(val) {}
	constexpr variant(T2 val) : active(T2_), type2(val) {}

	template <int N>
	[[nodiscard]] constexpr auto get_active_member() const noexcept {
		if constexpr (N == 0) return type1;
		if constexpr (N == 1) return type2;
	}
};

}	 // namespace cui::ct

#endif	  //CUI_VARIANT_HPP

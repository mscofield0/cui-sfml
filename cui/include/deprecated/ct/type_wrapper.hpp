#ifndef CUI_TYPE_WRAPPER_HPP
#define CUI_TYPE_WRAPPER_HPP

#include <type_traits>

namespace cui::ct {

template <typename T, typename DefaultConstructableValue>
struct type_wrapper
{
	T val;

	constexpr type_wrapper() : val{DefaultConstructableValue::value} {}
	constexpr type_wrapper(T val_) : val{val_} {}

	constexpr decltype(auto) operator=(T& rhs) {
		return val = rhs;
	}
	constexpr decltype(auto) operator=(type_wrapper& rhs) {
		return val = rhs.val;
	}
	template <typename T_>
	constexpr decltype(auto) operator=(T_& rhs) {
		return val = rhs;
	}
	constexpr decltype(auto) operator*(T& rhs) {
		return val * rhs;
	}
	constexpr decltype(auto) operator*(type_wrapper& rhs) {
		return val * rhs.val;
	}
	template <typename T_>
	constexpr decltype(auto) operator*(T_& rhs) {
		return val * rhs;
	}
	constexpr decltype(auto) operator/(T& rhs) {
		return val / rhs;
	}
	constexpr decltype(auto) operator/(type_wrapper& rhs) {
		return val / rhs.val;
	}
	template <typename T_>
	constexpr decltype(auto) operator/(T_& rhs) {
		return val / rhs;
	}
	constexpr decltype(auto) operator+(T& rhs) {
		return val + rhs;
	}
	constexpr decltype(auto) operator+(type_wrapper& rhs) {
		return val + rhs.val;
	}
	template <typename T_>
	constexpr decltype(auto) operator+(T_& rhs) {
		return val + rhs;
	}
	constexpr decltype(auto) operator-(T& rhs) {
		return val - rhs;
	}
	constexpr decltype(auto) operator-(type_wrapper& rhs) {
		return val - rhs.val;
	}
	template <typename T_>
	constexpr decltype(auto) operator-(T_& rhs) {
		return val - rhs;
	}
	constexpr decltype(auto) operator%(T& rhs) {
		return val % rhs;
	}
	constexpr decltype(auto) operator%(type_wrapper& rhs) {
		return val % rhs.val;
	}
	constexpr decltype(auto) operator^(T& rhs) {
		return val ^ rhs;
	}
	constexpr decltype(auto) operator^(type_wrapper& rhs) {
		return val ^ rhs.val;
	}
	template <typename T_>
	constexpr decltype(auto) operator^(T_& rhs) {
		return val ^ rhs;
	}
	constexpr decltype(auto) operator&(T& rhs) {
		return val & rhs;
	}
	constexpr decltype(auto) operator&(type_wrapper& rhs) {
		return val & rhs.val;
	}
	template <typename T_>
	constexpr decltype(auto) operator&(T_& rhs) {
		return val & rhs;
	}
	constexpr decltype(auto) operator|(T& rhs) {
		return val | rhs;
	}
	constexpr decltype(auto) operator|(type_wrapper& rhs) {
		return val | rhs.val;
	}
	template <typename T_>
	constexpr decltype(auto) operator|(T_& rhs) {
		return val | rhs;
	}
	constexpr decltype(auto) operator<(T& rhs) {
		return val < rhs;
	}
	constexpr decltype(auto) operator<(type_wrapper& rhs) {
		return val < rhs.val;
	}
	template <typename T_>
	constexpr decltype(auto) operator<(T_& rhs) {
		return val < rhs;
	}
	constexpr decltype(auto) operator>(T& rhs) {
		return val > rhs;
	}
	constexpr decltype(auto) operator>(type_wrapper& rhs) {
		return val > rhs.val;
	}
	template <typename T_>
	constexpr decltype(auto) operator>(T_& rhs) {
		return val > rhs;
	}
	constexpr decltype(auto) operator<<(T& rhs) {
		return val << rhs;
	}
	constexpr decltype(auto) operator<<(type_wrapper& rhs) {
		return val << rhs.val;
	}
	template <typename T_>
	constexpr decltype(auto) operator<<(T_& rhs) {
		return val << rhs;
	}
	constexpr decltype(auto) operator>>(T& rhs) {
		return val >> rhs;
	}
	constexpr decltype(auto) operator>>(type_wrapper& rhs) {
		return val >> rhs.val;
	}
	template <typename T_>
	constexpr decltype(auto) operator>>(T_& rhs) {
		return val >> rhs;
	}
	constexpr decltype(auto) operator==(T& rhs) {
		return val == rhs;
	}
	constexpr decltype(auto) operator==(type_wrapper& rhs) {
		return val == rhs.val;
	}
	template <typename T_>
	constexpr decltype(auto) operator==(T_& rhs) {
		return val == rhs;
	}
	constexpr decltype(auto) operator!=(T& rhs) {
		return val != rhs;
	}
	constexpr decltype(auto) operator!=(type_wrapper& rhs) {
		return val != rhs.val;
	}
	template <typename T_>
	constexpr decltype(auto) operator!=(T_& rhs) {
		return val != rhs;
	}
	constexpr decltype(auto) operator<=(T& rhs) {
		return val <= rhs;
	}
	constexpr decltype(auto) operator<=(type_wrapper& rhs) {
		return val <= rhs.val;
	}
	template <typename T_>
	constexpr decltype(auto) operator<=(T_& rhs) {
		return val <= rhs;
	}
	constexpr decltype(auto) operator>=(T& rhs) {
		return val >= rhs;
	}
	constexpr decltype(auto) operator>=(type_wrapper& rhs) {
		return val >= rhs.val;
	}
	template <typename T_>
	constexpr decltype(auto) operator>=(T_& rhs) {
		return val >= rhs;
	}
	constexpr decltype(auto) operator&&(T& rhs) {
		return val && rhs;
	}
	constexpr decltype(auto) operator&&(type_wrapper& rhs) {
		return val && rhs.val;
	}
	template <typename T_>
	constexpr decltype(auto) operator&&(T_& rhs) {
		return val && rhs;
	}
	constexpr decltype(auto) operator||(T& rhs) {
		return val || rhs;
	}
	constexpr decltype(auto) operator||(type_wrapper& rhs) {
		return val || rhs.val;
	}
	template <typename T_>
	constexpr decltype(auto) operator||(T_& rhs) {
		return val || rhs;
	}
	constexpr decltype(auto) operator+=(T& rhs) {
		return val += rhs;
	}
	constexpr decltype(auto) operator+=(type_wrapper& rhs) {
		return val += rhs.val;
	}
	template <typename T_>
	constexpr decltype(auto) operator+=(T_& rhs) {
		return val += rhs;
	}
	constexpr decltype(auto) operator-=(T& rhs) {
		return val -= rhs;
	}
	constexpr decltype(auto) operator-=(type_wrapper& rhs) {
		return val -= rhs.val;
	}
	template <typename T_>
	constexpr decltype(auto) operator-=(T_& rhs) {
		return val -= rhs;
	}
	constexpr decltype(auto) operator*=(T& rhs) {
		return val *= rhs;
	}
	constexpr decltype(auto) operator*=(type_wrapper& rhs) {
		return val *= rhs.val;
	}
	template <typename T_>
	constexpr decltype(auto) operator*=(T_& rhs) {
		return val *= rhs;
	}
	constexpr decltype(auto) operator/=(T& rhs) {
		return val /= rhs;
	}
	constexpr decltype(auto) operator/=(type_wrapper& rhs) {
		return val /= rhs.val;
	}
	template <typename T_>
	constexpr decltype(auto) operator/=(T_& rhs) {
		return val /= rhs;
	}
	constexpr decltype(auto) operator%=(T& rhs) {
		return val %= rhs;
	}
	constexpr decltype(auto) operator%=(type_wrapper& rhs) {
		return val %= rhs.val;
	}
	template <typename T_>
	constexpr decltype(auto) operator%=(T_& rhs) {
		return val %= rhs;
	}
	constexpr decltype(auto) operator&=(T& rhs) {
		return val &= rhs;
	}
	constexpr decltype(auto) operator&=(type_wrapper& rhs) {
		return val &= rhs.val;
	}
	template <typename T_>
	constexpr decltype(auto) operator&=(T_& rhs) {
		return val &= rhs;
	}
	constexpr decltype(auto) operator|=(T& rhs) {
		return val |= rhs;
	}
	constexpr decltype(auto) operator|=(type_wrapper& rhs) {
		return val |= rhs.val;
	}
	template <typename T_>
	constexpr decltype(auto) operator|=(T_& rhs) {
		return val |= rhs;
	}
	constexpr decltype(auto) operator^=(T& rhs) {
		return val ^= rhs;
	}
	constexpr decltype(auto) operator^=(type_wrapper& rhs) {
		return val ^= rhs.val;
	}
	template <typename T_>
	constexpr decltype(auto) operator^=(T_& rhs) {
		return val ^= rhs;
	}
	constexpr decltype(auto) operator*() {
		return *val;
	}
	constexpr decltype(auto) operator~() {
		return ~val;
	}
	constexpr decltype(auto) operator!() {
		return !val;
	}
	constexpr decltype(auto) operator++() {
		return ++val;
	}
	constexpr decltype(auto) operator--() {
		return --val;
	}
	constexpr decltype(auto) operator->() {
		return *val;
	}
};

}	 // namespace cui::ct

#endif	  // CUI_TYPE_WRAPPER_HPP

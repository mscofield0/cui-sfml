#ifndef CUI_STYLES_ATTRIBUTE_DATA_HPP
#define CUI_STYLES_ATTRIBUTE_DATA_HPP

#include <compile_time/string_view.hpp>
#include <styles/data.hpp>
#include <styles/detail/value_type.hpp>

namespace cui::styles {

class AttributeData
{
public:
	using type_t = StringView;
	using value_t = ValueData;

	constexpr AttributeData() : type_(), value_(), preprocessed_type_(ValueType::Color) {}
	constexpr AttributeData(const StringView p_type, const ValueData& p_data, const ValueType p_pptype)
		: type_(p_type), value_(p_data), preprocessed_type_(p_pptype) {}

	[[nodiscard]] constexpr auto type() noexcept -> type_t& {
		return type_;
	}

	[[nodiscard]] constexpr auto type() const noexcept -> type_t {
		return type_;
	}

	[[nodiscard]] constexpr auto value() noexcept -> value_t& {
		return value_;
	}

	[[nodiscard]] constexpr auto value() const noexcept -> const value_t& {
		return value_;
	}

	[[nodiscard]] constexpr auto preprocessed_type() const noexcept -> const ValueType {
		return preprocessed_type_;
	}

private:
	StringView type_;
	ValueData value_;
	ValueType preprocessed_type_;
};

}	 // namespace cui::styles

#endif	  // CUI_STYLES_ATTRIBUTE_DATA_HPP
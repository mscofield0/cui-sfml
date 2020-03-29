#ifndef CUI_STYLES_DATA_WITH_RULE_HPP
#define CUI_STYLES_DATA_WITH_RULE_HPP

#include <styles/data.hpp>
#include <styles/detail/value_type.hpp>

namespace cui::styles {

class DataWithRule
{
public:
	constexpr DataWithRule(const ValueData& p_data, const ValueType p_type) : data_(p_data), type_(p_type) {}

	constexpr auto data() noexcept -> ValueData& {
		return data_;
	}

	constexpr auto data() const noexcept -> const ValueData& {
		return data_;
	}

	constexpr auto preprocessed_type() noexcept -> ValueType& {
		return type_;
	}

	constexpr auto preprocessed_type() const noexcept -> const ValueType {
		return type_;
	}

private:
	ValueData data_;
	ValueType type_;
};

}	 // namespace cui::styles

#endif	  // CUI_STYLES_DATA_WITH_RULE_HPP
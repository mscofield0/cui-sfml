#ifndef CUI_STYLES_DETAIL_FUNCTION_ARGUMENT_LIST_HPP
#define CUI_STYLES_DETAIL_FUNCTION_ARGUMENT_LIST_HPP

#include <styles/data.hpp>
#include <styles/data_with_rule.hpp>
#include <styles/detail/parse_attribute_value.hpp>
#include <compile_time/string_view.hpp>
#include <compile_time/string_utils/trim_sv.hpp>
#include <compile_time/variant.hpp>
#include <compile_time/static_vector.hpp>

#ifdef CUI_ENABLE_CONSTEXPR_PARSING

#include <compile_time/format.hpp>

namespace cui::styles::detail__ {

constexpr auto parse_attribute_value(const StringView str) -> cui::Variant<styles::DataWithRule, ct::Format<512>>;

class FunctionArgumentList
{
public:
	using value_list_t = cui::static_vector<ValueData, 32>;
	using reference = value_list_t&;
	using const_reference = const value_list_t&;

	constexpr FunctionArgumentList() : list_{} {}

	[[nodiscard]] constexpr auto list() noexcept -> reference {
		return list_;
	}

	[[nodiscard]] constexpr auto list() const noexcept -> const_reference {
		return list_;
	}

	constexpr void add_argument(const ValueData& arg) {
		list().push_back(arg);
	}

	static constexpr auto parse(const StringView str) -> cui::Variant<FunctionArgumentList, ct::Format<512>> {
		FunctionArgumentList fal;
		if (str.empty()) return fal;

		size_t prev = 0;
		size_t pos = str.find(',');
		while (pos != StringView::npos) {
			if (prev == pos) {
				return ct::create_formatted_string_with_max_size<512>(
				  "ERROR: Function argument list is invalid\n\tList: {}",
				  str);
			}
			const auto trimmed = trim_sv(str.substr(prev, pos - prev));
			auto variant_val = detail__::parse_attribute_value(trimmed);
			if (variant_val.is_type_b()) {
				return variant_val.type_b();
			}
			fal.add_argument(variant_val.type_a().data());
			prev = pos + 1;
			pos = str.find(',', prev);
		}

		if (prev == pos) {
			return ct::create_formatted_string_with_max_size<512>(
			  "ERROR: Function argument list is invalid\n\tList: {}",
			  str);
		}
		const auto trimmed = trim_sv(str.substr(prev, pos - prev));
		auto variant_val = detail__::parse_attribute_value(trimmed);
		if (variant_val.is_type_b()) {
			return variant_val.type_b();
		}
		fal.add_argument(variant_val.type_a().data());
		return fal;
	}

private:
	value_list_t list_;
};

}	 // namespace cui::styles::detail__

#else	 // Disable constexpr FunctionArgumentList

#include <global/format.hpp>

namespace cui::styles::detail__ {

auto parse_attribute_value(const StringView str) -> cui::Variant<styles::DataWithRule, Format>;

class FunctionArgumentList
{
public:
	using value_list_t = cui::static_vector<ValueData, 32>;
	using reference = value_list_t&;
	using const_reference = const value_list_t&;

	FunctionArgumentList() : list_{} {}

	[[nodiscard]] auto list() noexcept -> reference {
		return list_;
	}

	[[nodiscard]] auto list() const noexcept -> const_reference {
		return list_;
	}

	void add_argument(const ValueData& arg) {
		list().push_back(arg);
	}

	static auto parse(const StringView str) -> cui::Variant<FunctionArgumentList, Format> {
		FunctionArgumentList fal;
		if (str.empty()) return fal;

		size_t prev = 0;
		size_t pos = str.find(',');
		while (pos != StringView::npos) {
			if (prev == pos) {
				return create_formatted_string("ERROR: Function argument list is invalid\n\tList: {}", str);
			}
			const auto trimmed = trim_sv(str.substr(prev, pos - prev));
			auto variant_val = detail__::parse_attribute_value(trimmed);
			if (variant_val.is_type_b()) {
				return variant_val.type_b();
			}
			fal.add_argument(variant_val.type_a().data());
			prev = pos + 1;
			pos = str.find(',', prev);
		}

		if (prev == pos) {
			return create_formatted_string("ERROR: Function argument list is invalid\n\tList: {}", str);
		}
		const auto trimmed = trim_sv(str.substr(prev, pos - prev));
		auto variant_val = detail__::parse_attribute_value(trimmed);
		if (variant_val.is_type_b()) {
			return variant_val.type_b();
		}
		fal.add_argument(variant_val.type_a().data());
		return fal;
	}

private:
	value_list_t list_;
};

}	 // namespace cui::styles::detail__

#endif

#endif	  // CUI_STYLES_DETAIL_FUNCTION_ARGUMENT_LIST_HPP
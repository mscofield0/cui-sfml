#ifndef CUI_STYLES_STYLE_HPP
#define CUI_STYLES_STYLE_HPP

#include <compile_time/string_view.hpp>
#include <styles/attribute_data.hpp>
#include <styles/definition.hpp>
#include <styles/detail/validate_attribute_type.hpp>
#include <styles/detail/is_correct_value_for_attr_type.hpp>
#include <styles/detail/parse_attribute_value.hpp>

// Enable constexpr Style
#ifdef CUI_ENABLE_CONSTEXPR_PARSING

#include <compile_time/static_vector.hpp>
#include <compile_time/format.hpp>

namespace cui::styles {

class Style
{
public:
	using event_vec_t = cui::static_vector<StringView, 128>;
	using attrdata_vec_t = cui::static_vector<AttributeData, 128>;

	constexpr Style() : name_(), events_{}, attributes_{} {}

	[[nodiscard]] constexpr auto name() noexcept -> StringView& {
		return name_;
	}

	[[nodiscard]] constexpr auto name() const noexcept -> StringView {
		return name_;
	}

	[[nodiscard]] constexpr auto events() noexcept -> event_vec_t& {
		return events_;
	}

	[[nodiscard]] constexpr auto events() const noexcept -> const event_vec_t& {
		return events_;
	}

	[[nodiscard]] constexpr auto attributes() noexcept -> attrdata_vec_t& {
		return attributes_;
	}

	[[nodiscard]] constexpr auto attributes() const noexcept -> const attrdata_vec_t& {
		return attributes_;
	}

	[[nodiscard]] static constexpr auto create(const styles::Definition& def) -> cui::Variant<Style, ct::Format<512>> {
		Style style;
		style.name() = def.identifier();
		for (const auto str : def.events()) style.events().push_back(str);

		for (const auto& attribute : def.attributes()) {
			if (!detail__::validate_attribute_type(attribute.type())) {
				return ct::create_formatted_string_with_max_size<512>("ERROR: Attribute type is invalid.\nProvided: {}",
																	  attribute.type());
			}

			const auto attr_value_variant = detail__::parse_attribute_value(attribute.value());
			if (attr_value_variant.is_type_b()) {
				return attr_value_variant.type_b();
			}

			if (!detail__::is_correct_value_for_attr_type(attribute.type(), attr_value_variant.type_a().data())) {
				return ct::create_formatted_string_with_max_size<512>("ERROR: Received wrong value for attribute.");
			}
			style.attributes().emplace_back(attribute.type(),
											attr_value_variant.type_a().data(),
											attr_value_variant.type_a().preprocessed_type());
		}

		return style;
	}

private:
	StringView name_;
	event_vec_t events_;
	attrdata_vec_t attributes_;
};

}	 // namespace cui::styles

#else	 // Disable constexpr Style

#include <vector>

namespace cui::styles {

class Style
{
public:
	using event_vec_t = std::vector<StringView>;
	using attrdata_vec_t = std::vector<AttributeData>;

	[[nodiscard]] auto name() noexcept -> StringView& {
		return name_;
	}

	[[nodiscard]] auto name() const noexcept -> StringView {
		return name_;
	}

	[[nodiscard]] auto events() noexcept -> event_vec_t& {
		return events_;
	}

	[[nodiscard]] auto events() const noexcept -> const event_vec_t& {
		return events_;
	}

	[[nodiscard]] auto attributes() noexcept -> attrdata_vec_t& {
		return attributes_;
	}

	[[nodiscard]] auto attributes() const noexcept -> const attrdata_vec_t& {
		return attributes_;
	}

	[[nodiscard]] static auto create(const styles::Definition& def) -> cui::Variant<Style, Format> {
		Style style;
		style.name() = def.identifier();
		for (const auto str : def.events()) style.events().push_back(str);

		for (const auto& attribute : def.attributes()) {
			if (!detail__::validate_attribute_type(attribute.type())) {
				return create_formatted_string("ERROR: Attribute type is invalid.\nProvided: {}", attribute.type());
			}

			const auto attr_value_variant = detail__::parse_attribute_value(attribute.value());
			if (attr_value_variant.is_type_b()) {
				return attr_value_variant.type_b();
			}

			if (!detail__::is_correct_value_for_attr_type(attribute.type(), attr_value_variant.type_a().data())) {
				return create_formatted_string("ERROR: Received wrong value for attribute.");
			}

			style.attributes().emplace_back(attribute.type(),
											attr_value_variant.type_a().data(),
											attr_value_variant.type_a().preprocessed_type());
		}

		return style;
	}

private:
	StringView name_;
	event_vec_t events_;
	attrdata_vec_t attributes_;
};

}	 // namespace cui::styles

#endif	  // constexpr Style

#endif	  // CUI_STYLES_STYLE_HPP
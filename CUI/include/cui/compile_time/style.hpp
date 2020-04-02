#ifndef CUI_CT_STYLE_HPP
#define CUI_CT_STYLE_HPP

#include <compile_time/string/string_view.hpp>
#include <compile_time/styles/attribute_data.hpp>
#include <compile_time/styles/definition.hpp>
#include <compile_time/styles/detail/values/validate_attribute_type.hpp>
#include <compile_time/styles/detail/values/is_correct_value_for_attr_type.hpp>
#include <compile_time/styles/detail/helper_macros.hpp>
#include <compile_time/styles/parse_attribute_value.hpp>

#include <containers/static_vector.hpp>
#include <compile_time/format/fmt_string_view.hpp>
#include <compile_time/format/format.hpp>
#include <compile_time/format/fmt.hpp>

namespace cui::ct {

class Style
{
public:
	using events_t = StaticVector<StringView, 64>;
	using attributes_t = StaticVector<styles::AttributeData, 64>;

	constexpr Style() : name_(), events_{}, attributes_{} {}

	[[nodiscard]] constexpr auto name() noexcept -> StringView& {
		return name_;
	}

	[[nodiscard]] constexpr auto name() const noexcept -> StringView {
		return name_;
	}

	[[nodiscard]] constexpr auto events() noexcept -> events_t& {
		return events_;
	}

	[[nodiscard]] constexpr auto events() const noexcept -> const events_t& {
		return events_;
	}

	[[nodiscard]] constexpr auto attributes() noexcept -> attributes_t& {
		return attributes_;
	}

	[[nodiscard]] constexpr auto attributes() const noexcept -> const attributes_t& {
		return attributes_;
	}

	[[nodiscard]] static constexpr auto create(const styles::Definition& def) -> Variant<Style, Format<256>> {
		Style style;
		style.name() = def.identifier();
		for (const auto str : def.events()) style.events().push_back(str);

		for (const auto& attribute : def.attributes()) {
			if (!styles::detail::validate_attribute_type(attribute.type())) {
				RETURN_ERROR("ERROR: Attribute type is invalid.\nProvided: {}", attribute.type());
			}

			const auto attr_value_variant = styles::detail::parse_attribute_value(attribute.value());
			if (attr_value_variant.is_type_b()) {
				return attr_value_variant.type_b();
			}

			if (!styles::detail::is_correct_value_for_attr_type(attribute.type(), attr_value_variant.type_a().data())) {
				RETURN_ERROR("ERROR: Received wrong value for attribute.");
			}
			style.attributes().emplace_back(attribute.type(),
											attr_value_variant.type_a().data(),
											attr_value_variant.type_a().preprocessed_type());
		}

		return style;
	}

private:
	StringView name_;
	events_t events_;
	attributes_t attributes_;
};

}	 // namespace cui::ct

#endif	  // CUI_STYLES_STYLE_HPP
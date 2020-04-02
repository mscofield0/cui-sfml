#ifndef CUI_SCHEMATIC_HPP
#define CUI_SCHEMATIC_HPP

#include <visual/attributes.hpp>
#include <visual/rules.hpp>
#include <compile_time/styles/attribute_data.hpp>
#include <compile_time/styles/detail/styles/get_attribute_index.hpp>
#include <aliases.hpp>

namespace cui {

class Schematic : public Attributes, public Rules
{
public:
	Schematic() noexcept = default;

	constexpr void assign(const ct::styles::AttributeData& attr_data) noexcept {
		const auto i = ct::styles::detail::get_attribute_index(attr_data.type());

		switch (static_cast<ct::styles::detail::AttributeIndexes>(i)) {
			case ct::styles::detail::AttributeIndexes::Color: {
				this->text_color_ = attr_data.value();
				break;
			}
		}
	}
};

}	 // namespace cui

#endif	  // CUI_SCHEMATIC_HPP
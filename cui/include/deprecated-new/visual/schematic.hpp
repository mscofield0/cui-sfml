#ifndef CUI_VISUAL_SCHEMATIC_HPP
#define CUI_VISUAL_SCHEMATIC_HPP

#include <visual/attributes.hpp>
#include <visual/rules.hpp>
#include <styles/attribute_data.hpp>
#include <styles/detail/get_attribute_index.hpp>

namespace cui {

class Schematic : public Attributes, public Rules
{
public:
	Schematic() noexcept = default;

	constexpr void assign(const styles::AttributeData& attr_data) noexcept {
		uint32_t i = styles::get_attribute_index(attr_data.type());

		switch (static_cast<styles::AttributeIndexes>(i)) {
			case styles::AttributeIndexes::Color: {
				this->text_color_ = attr_data.value();
				break;
			}
		}
	}
};

}	 // namespace cui

#endif	  // CUI_VISUAL_SCHEMATIC_HPP
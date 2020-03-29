#ifndef CUI_STYLES_DETAIL_DT_TO_VAT_HPP
#define CUI_STYLES_DETAIL_DT_TO_VAT_HPP

#include <styles/valid_attribute_types.hpp>
#include <styles/constants.hpp>

namespace cui::styles::detail__ {

constexpr auto dt_to_vat(styles::DataTypes data_type) -> styles::ValidAttributeType {
	switch (data_type) {
		case styles::DataTypes::Color: {
			return styles::ValidAttributeType::RGBA;
		}
		case styles::DataTypes::Float: {
			return styles::ValidAttributeType::Float;
		}
		case styles::DataTypes::Int: {
			return styles::ValidAttributeType::Int;
		}
		case styles::DataTypes::Instruction: {
			return styles::ValidAttributeType::Instruction;
		}
		case styles::DataTypes::Vector2: {
			return styles::ValidAttributeType::Vec2;
		}
		case styles::DataTypes::Vector3: {
			return styles::ValidAttributeType::Vec3;
		}
		case styles::DataTypes::Vector4: {
			return styles::ValidAttributeType::Vec4;
		}
		default: {
			return styles::ValidAttributeType::Invalid;
		}
	}

	// Impossible
	return styles::ValidAttributeType::Invalid;
}

}	 // namespace cui::styles::detail__

#endif	  // CUI_STYLES_DETAIL_DT_TO_VAT_HPP
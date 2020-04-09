#ifndef CUI_SFML_INTERMEDIARY_PROXY_HPP
#define CUI_SFML_INTERMEDIARY_PROXY_HPP


#include <SFML/Graphics/Color.hpp>
#include <cui/data_types/color.hpp>
#include <exceptions/conversion_error.hpp>
#include <aliases.hpp>

namespace cui::intermediary {

enum class ObjectTypes : byte{
	SF_Color,
	Color
};

class Proxy
{
public:
	Proxy(const sf::Color& val) : active_(ObjectTypes::SF_Color), sf_color(&val) {}
	Proxy(const data_types::Color& val) : active_(ObjectTypes::Color), color(&val) {}

	[[nodiscard]] auto get_sf_color() const -> const sf::Color* {
		if(active_ != ObjectTypes::SF_Color) throw cui::ConversionError{"Proxy active is not set to ObjectTypes::SF_Color"};

		return sf_color;
	}
	
	[[nodiscard]] auto get_color() const -> const data_types::Color* {
		if(active_ != ObjectTypes::Color) throw cui::ConversionError{"Proxy active is not set to ObjectTypes::Color"};

		return color;
	}

private:
	ObjectTypes active_;

	union {
		const sf::Color* sf_color;
		const data_types::Color* color;
	};
};

}	 // namespace cui::intermediary

#endif	  // CUI_SFML_INTERMEDIARY_PROXY_HPP
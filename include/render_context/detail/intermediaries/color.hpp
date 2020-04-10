#ifndef CUI_SFML_INTERMEDIARY_COLOR_HPP
#define CUI_SFML_INTERMEDIARY_COLOR_HPP

#include <SFML/Graphics/Color.hpp>
#include <cui/data_types/color.hpp>
#include <render_context/detail/intermediaries/proxy.hpp>

namespace cui::intermediary {

class Color
{
public:
	Color(const sf::Color& val) : proxy_(val) {}
	Color(const data_types::Color& val) : proxy_(val) {}

	[[nodiscard]] operator sf::Color() const {
		const auto* val = proxy_.get_color();
		return sf::Color(val->red(), val->green(), val->blue(), val->alpha());
	}

	[[nodiscard]] operator data_types::Color() const {
		const auto* val = proxy_.get_sf_color();
		return data_types::Color(val->r, val->g, val->b, val->a);
	}

private:
	Proxy proxy_;
};

}	 // namespace cui::intermediary

#endif	  // CUI_SFML_INTERMEDIARY_COLOR_HPP
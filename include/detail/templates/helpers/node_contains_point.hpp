#ifndef CUI_SFML_TEMPLATE_HELPER_NODE_CONTAINS_POINT_HPP
#define CUI_SFML_TEMPLATE_HELPER_NODE_CONTAINS_POINT_HPP

#include <detail/templates/requirements.hpp>

namespace cui::templates {

bool NodeContainsPoint(Window& window, std::size_t caller_index, const sf::Vector2f& point) {
	return window.cache()[caller_index + 1].getGlobalBounds().contains(point);
}

}	 // namespace cui::templates

#endif	  // CUI_SFML_TEMPLATE_HELPER_GET_MOUSE_POSITION_HPP
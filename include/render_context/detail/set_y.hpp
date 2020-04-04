#ifndef CUI_SFML_DETAIL_SET_Y_HPP
#define CUI_SFML_DETAIL_SET_Y_HPP

#include <cui/visual/node.hpp>
#include <render_context/visual_element.hpp>
#include <aliases.hpp>

namespace cui {

void set_y(const Node& node, VisualElement& ve) noexcept {
	const auto x = ve.getPosition().x;
	const auto y = node.active_schematic().get().y().integer_value();
	ve.setPosition(x, y);
}

}	 // namespace cui

#endif	  // CUI_SFML_DETAIL_SET_Y_HPP
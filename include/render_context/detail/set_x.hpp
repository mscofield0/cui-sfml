#ifndef CUI_SFML_DETAIL_SET_X_HPP
#define CUI_SFML_DETAIL_SET_X_HPP

#include <cui/visual/node.hpp>
#include <render_context/visual_element.hpp>
#include <aliases.hpp>

namespace cui {

void set_x(const Node& node, VisualElement& ve) noexcept {
	const auto x = node.active_schematic().get().x().integer_value();
	const auto y = ve.getPosition().y;
	ve.setPosition(x, y);
}

}	 // namespace cui

#endif	  // CUI_SFML_DETAIL_SET_X_HPP
#ifndef CUI_SFML_TEMPLATE_HELPER_GET_MOUSE_POSITION_HPP
#define CUI_SFML_TEMPLATE_HELPER_GET_MOUSE_POSITION_HPP

#include <detail/templates/requirements.hpp>

namespace cui::templates {

sf::Vector2f GetMousePosition(const event_data_t& event_data) {
	int x, y;
	std::visit(
	  [&x, &y](auto& arg) {
		  using T = std::decay_t<decltype(arg)>;
		  if constexpr (cui::is_any_v<T, sf::Event::MouseButtonEvent, sf::Event::MouseMoveEvent>) {
			  x = arg.x;
			  y = arg.y;
		  } else {
			  throw std::logic_error("Accessed invalid type inside CUI_BUTTON template");
		  }
	  },
	  event_data.get());
	return sf::Vector2f(x, y);
}

}	 // namespace cui::templates

#endif	  // CUI_SFML_TEMPLATE_HELPER_GET_MOUSE_POSITION_HPP
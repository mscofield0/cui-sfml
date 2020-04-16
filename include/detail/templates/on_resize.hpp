#ifndef CUI_SFML_TEMPLATE_ON_RESIZE_HPP
#define CUI_SFML_TEMPLATE_ON_RESIZE_HPP

#include <detail/templates/requirements.hpp>

namespace cui::templates {

void OnResize(Window& window, event_data_t& event_data) {
	const auto [w, h] = std::get<sf::Event::SizeEvent>(event_data.get());
	window.resize(w, h);
	window.schedule_to_update_cache();
}

}	 // namespace cui::templates

#endif	  // CUI_SFML_TEMPLATE_SWITCH_SCHEMATIC_HPP
#ifndef CUI_SFML_TEMPLATE_BUTTON_HPP
#define CUI_SFML_TEMPLATE_BUTTON_HPP

#define CUI_BUTTON_ON_CLICK(window, event_data) \
	{ \
		std::unique_lock lock(window.window_mutex_); \
		const auto [_, x, y] = std::get<sf::Event::MouseButtonEvent>(event_data.get()); \
		const auto point = sf::Vector2f(x, y); \
		auto* node = event_data.caller(); \
		if (!window.cache()[event_data.caller_index()].getGlobalBounds().contains(point)) return; \
		auto& e_schemes = node->data().event_schematics(); \
		for (auto it = e_schemes.begin(); it != e_schemes.end(); ++it) { \
			if (it.key() == event_name) { \
				node->data().active_schematic() = it.value(); \
			} \
		} \
	}

#endif	  // CUI_SFML_TEMPLATE_BUTTON_HPP
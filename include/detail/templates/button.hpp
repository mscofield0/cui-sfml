#ifndef CUI_SFML_TEMPLATE_BUTTON_HPP
#define CUI_SFML_TEMPLATE_BUTTON_HPP

#include <detail/templates/requirements.hpp>

#define CUI_BUTTON(window, event_data) \
	{ \
		int x, y; \
		std::visit( \
		  [&x, &y](auto& arg) { \
			  using T = std::decay_t<decltype(arg)>; \
			  if constexpr (cui::is_any_v<T, sf::Event::MouseButtonEvent, sf::Event::MouseMoveEvent>) { \
				  x = arg.x; \
				  y = arg.y; \
			  } else { \
				  throw std::logic_error("Accessed invalid type inside CUI_BUTTON template"); \
			  } \
		  }, \
		  event_data.get()); \
		const auto point = sf::Vector2f(x, y); \
		{ \
			std::unique_lock wlock(window.window_mutex_); \
			if (!window.cache()[event_data.caller_index()].getGlobalBounds().contains(point)) { \
				return; \
			} \
		} \
	}

#endif	  // CUI_SFML_TEMPLATE_BUTTON_HPP
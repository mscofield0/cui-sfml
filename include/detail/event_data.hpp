#ifndef CUI_SFML_EVENT_DATA_HPP
#define CUI_SFML_EVENT_DATA_HPP

#include <variant>

#include <SFML/Window/Event.hpp>

namespace cui {

using EventData = std::variant<sf::Event::SizeEvent,
							   sf::Event::KeyEvent,
							   sf::Event::TextEvent,
							   sf::Event::MouseMoveEvent,
							   sf::Event::MouseButtonEvent,
							   sf::Event::MouseWheelEvent,
							   sf::Event::MouseWheelScrollEvent,
							   sf::Event::JoystickConnectEvent,
							   sf::Event::JoystickMoveEvent,
							   sf::Event::JoystickButtonEvent,
							   sf::Event::TouchEvent,
							   sf::Event::SensorEvent>;

}	 // namespace cui

#endif	  // CUI_SFML_EVENT_DATA_HPP
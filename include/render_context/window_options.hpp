#ifndef CUI_SFML_WINDOW_OPTIONS_HPP
#define CUI_SFML_WINDOW_OPTIONS_HPP

#include <string>

#include <SFML/Graphics.hpp>

#include <aliases.hpp>

namespace cui {

class WindowOptions
{
public:
	u32 width, height;
	std::string title;
	u32 style;
	sf::ContextSettings ctx_settings;
	u32 framerate;
};

}	 // namespace cui

#endif	  // CUI_SFML_WINDOW_OPTIONS_HPP
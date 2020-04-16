#ifndef CUI_SFML_TEMPLATE_REQUIREMENTS_HPP
#define CUI_SFML_TEMPLATE_REQUIREMENTS_HPP

#include <exception>

#include <detail/event_data.hpp>
#include <detail/utils/any_is_same.hpp>
#include <window.hpp>

#include <SFML/Window/Event.hpp>

namespace cui {

using event_data_t = typename Window::event_data_t;

}

#endif	  // CUI_SFML_TEMPLATE_REQUIREMENTS_HPP
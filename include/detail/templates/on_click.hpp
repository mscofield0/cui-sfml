#ifndef CUI_SFML_TEMPLATE_ON_CLICK_HPP
#define CUI_SFML_TEMPLATE_ON_CLICK_HPP

#include <detail/templates/helpers/get_mouse_position.hpp>
#include <detail/templates/helpers/node_contains_point.hpp>
#include <detail/templates/requirements.hpp>

namespace cui::templates {

using on_click_invoke_fn_t = std::function<void(Window&, event_data_t&)>;
using on_click_with_point_invoke_fn_t = std::function<void(Window&, event_data_t&, const sf::Vector2f&)>;

void OnClick(Window& window, event_data_t& event_data, on_click_invoke_fn_t&& fn_on_click) {
	if (!NodeContainsPoint(window, event_data.caller_index(), GetMousePosition(window, event_data))) {
		return;
	}

	fn_on_click(window, event_data);
}

void OnClick(Window& window, event_data_t& event_data, on_click_with_point_invoke_fn_t&& fn_on_click) {
	const auto point = GetMousePosition(window, event_data);
	if (!NodeContainsPoint(window, event_data.caller_index(), point)) {
		return;
	}

	fn_on_click(window, event_data, point);
}

}	 // namespace cui::templates

#endif	  // CUI_SFML_TEMPLATE_ON_CLICK_HPP
#ifndef CUI_SFML_TEMPLATE_ON_HOVER_HPP
#define CUI_SFML_TEMPLATE_ON_HOVER_HPP

#include <detail/templates/helpers/get_mouse_position.hpp>
#include <detail/templates/helpers/node_contains_point.hpp>
#include <detail/templates/requirements.hpp>

namespace cui::templates {

using on_hover_invoke_fn_t = std::function<void(Window&, event_data_t&)>;
using on_hover_with_id_invoke_fn_t = std::function<void(Window&, event_data_t&, const std::string&)>;

void OnHover(Window& window,
			 event_data_t& event_data,
			 const std::string& event_cache_id,
			 on_hover_invoke_fn_t&& fn_no_hover,
			 on_hover_invoke_fn_t&& fn_hover) {
	if (!NodeContainsPoint(window, event_data.caller_index(), GetMousePosition(window, event_data))) {
		std::unique_lock dlock(window.scene_mutex);

		auto& cache_item = window.event_cache[event_cache_id];

		if (!cache_item.has_value()) cache_item = false;
		auto& hovered_before = std::any_cast<bool&>(cache_item);
		if (!hovered_before) return;

		cache_item = false;

		fn_no_hover(window, event_data);

		return;
	}

	std::unique_lock dlock(window.scene_mutex);
	auto& cache_item = window.event_cache[event_cache_id];

	if (!cache_item.has_value()) cache_item = false;
	auto& hovered_before = std::any_cast<bool&>(cache_item);
	if (hovered_before) return;

	cache_item = true;

	fn_hover(window, event_data);
}

void OnHover(Window& window,
			 event_data_t& event_data,
			 const std::string& event_cache_id,
			 on_hover_with_id_invoke_fn_t&& fn_no_hover,
			 on_hover_with_id_invoke_fn_t&& fn_hover) {
	if (!NodeContainsPoint(window, event_data.caller_index(), GetMousePosition(window, event_data))) {
		std::unique_lock dlock(window.event_cache_mutex);

		auto& cache_item = window.event_cache[event_cache_id];

		if (!cache_item.has_value()) cache_item = false;
		auto& hovered_before = std::any_cast<bool&>(cache_item);
		if (!hovered_before) return;

		cache_item = false;

		fn_no_hover(window, event_data, event_cache_id);

		return;
	}

	std::unique_lock dlock(window.event_cache_mutex);
	auto& cache_item = window.event_cache[event_cache_id];

	if (!cache_item.has_value()) cache_item = false;
	auto& hovered_before = std::any_cast<bool&>(cache_item);
	if (hovered_before) return;

	cache_item = true;

	fn_hover(window, event_data, event_cache_id);
}

}	 // namespace cui::templates

#endif	  // CUI_SFML_TEMPLATE_ON_HOVER_HPP
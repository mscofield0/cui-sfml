#ifndef CUI_SFML_TEMPLATE_ON_HOVER_HPP
#define CUI_SFML_TEMPLATE_ON_HOVER_HPP

#include <detail/templates/helpers/get_mouse_position.hpp>
#include <detail/templates/helpers/node_contains_point.hpp>
#include <detail/templates/requirements.hpp>

namespace cui::templates {

using on_hover_invoke_fn_t = std::function<void(Window&)>;
using on_hover_with_event_data_invoke_fn_t = std::function<void(Window&, event_data_t&)>;

void OnHover(Window& window, event_data_t& event_data, on_hover_invoke_fn_t&& fn_no_hover, on_hover_invoke_fn_t&& fn_hover) {
	auto& prev_node_index_any = window.event_cache["previous_hover_node"];
	if (!prev_node_index_any.has_value()) {
		prev_node_index_any = event_data.caller_index();

		fn_hover(window);
		return;
	}

	auto& prev_node_index = std::any_cast<std::size_t&>(prev_node_index_any);

	if (prev_node_index == event_data.caller_index()) return;

	fn_no_hover(window);

	prev_node_index = event_data.caller_index();

	fn_hover(window);
}

void OnHover(Window& window, event_data_t& event_data, on_hover_with_event_data_invoke_fn_t&& fn_no_hover, on_hover_with_event_data_invoke_fn_t&& fn_hover) {
	auto& prev_node_index_any = window.event_cache["previous_hover_node"];
	if (!prev_node_index_any.has_value()) {
		prev_node_index_any = event_data.caller_index();

		fn_hover(window, event_data);
		return;
	}

	auto& prev_node_index = std::any_cast<std::size_t&>(prev_node_index_any);

	if (prev_node_index == event_data.caller_index()) return;

	auto& graph = window.active_scene().graph();
	auto& prev_node = (prev_node_index == SceneGraph::root_index) ? graph.root() : graph[prev_node_index].data();
	auto temp_event_data = event_data_t(event_data.get(), &prev_node, prev_node_index, event_data.event_name());

	fn_no_hover(window, temp_event_data);

	prev_node_index = event_data.caller_index();

	fn_hover(window, event_data);
}

}	 // namespace cui::templates

#endif	  // CUI_SFML_TEMPLATE_ON_HOVER_HPP
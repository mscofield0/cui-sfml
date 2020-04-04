#ifndef CUI_SFML_EVENT_MANAGER_HPP
#define CUI_SFML_EVENT_MANAGER_HPP

#include <functional>

#include <cui/visual/scene_graph.hpp>
#include <render_context/render_context.hpp>

namespace cui {

class EventManager
{
public:
	using graph_ref = std::reference_wrapper<SceneGraph>;
	using ctx_ref = const RenderContext&;
	EventManager(SceneGraph& p_graph, ctx_ref p_ctx) : graph_(std::ref(p_graph)), ctx_(p_ctx) {}

	EventManager(const EventManager&) = delete;
	EventManager(EventManager&&) = delete;
	EventManager& operator=(const EventManager&) = delete;
	EventManager& operator=(EventManager&&) = delete;

	bool poll_event();

private:
	graph_ref graph_;
	ctx_ref ctx_;
};

bool EventManager::poll_event() {
	sf::Event evt;
	const bool empty = ctx_.window()->pollEvent(evt);
	// process_event(evt)
	return empty;
}

}	 // namespace cui

#endif	  // CUI_SFML_EVENT_MANAGER_HPP
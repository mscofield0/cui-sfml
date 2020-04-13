#ifndef CUI_SFML_SCENE_STATE_HPP
#define CUI_SFML_SCENE_STATE_HPP

#include <functional>
#include <string>
#include <string_view>

#include <visual/scene_graph.hpp>
#include <tsl/hopscotch_map.h>
#include <aliases.hpp>

namespace cui {

template <typename EventFunctionType>
class SceneState
{
public:
	using event_t = EventFunctionType;
	using event_map_t = tsl::hopscotch_map<std::string, event_t>;

	SceneState(const SceneGraph& p_graph) : graph_(p_graph) {}

	SceneState(SceneGraph&& p_graph) : graph_(std::move(p_graph)) {}

	void register_event(const std::string& name, event_t&& p_func) {
		registered_events_[name] = std::move(p_func);
	}

	void register_event(std::string&& name, event_t&& p_func) {
		registered_events_[std::move(name)] = std::move(p_func);
	}

	auto get_event(const std::string& name) const -> const event_t& {
		return registered_events_.at(name);
	}

	auto get_event(std::string&& name) const -> const event_t& {
		return registered_events_.at(std::move(name));
	}

	[[nodiscard]] auto graph() noexcept -> SceneGraph& {
		return graph_;
	}

	[[nodiscard]] auto graph() const noexcept -> const SceneGraph& {
		return graph_;
	}

	[[nodiscard]] auto registered_events() noexcept -> event_map_t& {
		return registered_events_;
	}

	[[nodiscard]] auto registered_events() const noexcept -> const event_map_t& {
		return registered_events_;
	}

private:
	SceneGraph graph_;
	event_map_t registered_events_;
};

}	 // namespace cui

#endif	  // CUI_SFML_SCENE_STATE_HPP
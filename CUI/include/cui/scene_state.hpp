#ifndef CUI_SFML_SCENE_STATE_HPP
#define CUI_SFML_SCENE_STATE_HPP

#include <functional>
#include <string>
#include <string_view>
#include <utility>

#include <visual/scene_graph.hpp>
#include <tsl/hopscotch_map.h>
#include <aliases.hpp>

namespace cui {

template <typename EventFunctionType, typename EventType>
class SceneState
{
public:
	using graph_t = SceneGraph;
	using event_t = EventFunctionType;
	using event_map_t = tsl::hopscotch_map<std::string, event_t>;
	using event_marker_map_t = tsl::hopscotch_map<EventType, std::pair<event_map_t, event_map_t>>;

	SceneState(const graph_t& p_graph) : graph_(p_graph) {}

	SceneState(graph_t&& p_graph) : graph_(std::move(p_graph)) {}

	void register_event(const EventType& type, const std::string& name, event_t&& event) {
		auto& reged_events = marked_sections_[type].first;
		reged_events[name] = std::move(event);
	}

	void register_event(const EventType& type, std::string&& name, event_t&& event) {
		auto& reged_events = marked_sections_[type].first;
		reged_events[std::move(name)] = std::move(event);
	}

	void register_global_event(const EventType& type, const std::string& name, event_t&& event) {
		auto& reged_events = marked_sections_[type].second;
		reged_events[name] = std::move(event);
	}

	void register_global_event(const EventType& type, std::string&& name, event_t&& event) {
		auto& reged_events = marked_sections_[type].second;
		reged_events[std::move(name)] = std::move(event);
	}

	void unregister_event(const EventType& type, const std::string& name) {
		marked_sections_[type].first.erase(name);
	}

	void unregister_event(const EventType& type, std::string&& name) {
		marked_sections_[type].first.erase(std::move(name));
	}

	void unregister_global_event(const EventType& type, const std::string& name) {
		marked_sections_[type].second.erase(name);
	}

	void unregister_global_event(const EventType& type, std::string&& name) {
		marked_sections_[type].second.erase(std::move(name));
	}

	[[nodiscard]] auto get_event(const EventType& type, const std::string& name) const -> const event_t& {
		return marked_sections_.at(type).first.at(name);
	}

	[[nodiscard]] auto get_event(const EventType& type, std::string&& name) const -> const event_t& {
		return marked_sections_.at(type).first.at(std::move(name));
	}

	[[nodiscard]] auto get_global_event(const EventType& type, const std::string& name) const -> const event_t& {
		return marked_sections_.at(type).second.at(name);
	}

	[[nodiscard]] auto get_global_event(const EventType& type, std::string&& name) const -> const event_t& {
		return marked_sections_.at(type).second.at(std::move(name));
	}

	[[nodiscard]] auto graph() noexcept -> graph_t& {
		return graph_;
	}

	[[nodiscard]] auto graph() const noexcept -> const graph_t& {
		return graph_;
	}

	[[nodiscard]] auto marked_sections() const noexcept -> const event_marker_map_t& {
		return marked_sections_;
	}

	[[nodiscard]] auto registered_events(const EventType& event_type) const noexcept -> const event_map_t& {
		return marked_sections_.at(event_type).first;
	}

	[[nodiscard]] auto registered_global_events(const EventType& event_type) const noexcept -> const event_map_t& {
		return marked_sections_.at(event_type).second;
	}

private:
	graph_t graph_;
	event_marker_map_t marked_sections_;
};

}	 // namespace cui

#endif	  // CUI_SFML_SCENE_STATE_HPP
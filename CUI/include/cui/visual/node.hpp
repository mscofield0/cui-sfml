#ifndef CUI_NODE_HPP
#define CUI_NODE_HPP

#include <functional>
#include <string>

#include <compile_time/string/string_view.hpp>
#include <tsl/hopscotch_map.h>
#include <tsl/hopscotch_set.h>
#include <visual/schematic.hpp>

namespace cui {

class Node
{
public:
	using schematic_map_t = tsl::hopscotch_map<std::string, Schematic>;
	using schematic_ref = std::reference_wrapper<Schematic>;

	Node()
		: default_schematic_(), event_schematics_(), active_(std::ref(default_schematic_)), name_("root"), text_("") {}

	Node(const std::string& p_name, const std::string& p_text)
		: default_schematic_(), event_schematics_(), active_(std::ref(default_schematic_)), name_(p_name),
		  text_(p_text) {}

	Node(const std::string& p_name, const ct::StringView p_text)
		: default_schematic_(), event_schematics_(), active_(std::ref(default_schematic_)), name_(p_name),
		  text_(p_text.begin(), p_text.end()) {}

	Node(const ct::StringView p_name, const std::string& p_text)
		: default_schematic_(), event_schematics_(), active_(std::ref(default_schematic_)),
		  name_(p_name.begin(), p_name.end()), text_(p_text) {}

	Node(const ct::StringView p_name, const ct::StringView p_text)
		: default_schematic_(), event_schematics_(), active_(std::ref(default_schematic_)),
		  name_(p_name.begin(), p_name.end()), text_(p_text.begin(), p_text.end()) {}

	Node(const Node& rhs)
		: default_schematic_(rhs.default_schematic_), event_schematics_(rhs.event_schematics_),
		  active_(std::ref(default_schematic_)), name_(rhs.name_), text_(rhs.text_),
		  attached_events_(rhs.attached_events_) {}

	Node(Node&& rhs)
		: default_schematic_(std::move(rhs.default_schematic_)), event_schematics_(std::move(rhs.event_schematics_)),
		  active_(std::ref(default_schematic_)), name_(std::move(rhs.name_)), text_(std::move(rhs.text_)),
		  attached_events_(std::move(rhs.attached_events_)) {}

	Node& operator=(const Node& rhs) {
		default_schematic_ = rhs.default_schematic_;
		event_schematics_ = rhs.event_schematics_;
		active_ = default_schematic_;
		name_ = rhs.name_;
		text_ = rhs.text_;
		attached_events_ = rhs.attached_events_;
		return *this;
	}

	Node& operator=(Node&& rhs) {
		default_schematic_ = std::move(rhs.default_schematic_);
		event_schematics_ = std::move(rhs.event_schematics_);
		active_ = default_schematic_;
		name_ = std::move(rhs.name_);
		text_ = std::move(rhs.text_);
		attached_events_ = std::move(rhs.attached_events_);
		return *this;
	}

	[[nodiscard]] auto default_schematic() noexcept -> Schematic& {
		return default_schematic_;
	}

	[[nodiscard]] auto default_schematic() const noexcept -> const Schematic& {
		return default_schematic_;
	}

	[[nodiscard]] auto event_schematics() noexcept -> schematic_map_t& {
		return event_schematics_;
	}

	[[nodiscard]] auto event_schematics() const noexcept -> const schematic_map_t& {
		return event_schematics_;
	}

	[[nodiscard]] auto active_schematic() noexcept -> schematic_ref& {
		return active_;
	}

	[[nodiscard]] auto active_schematic() const noexcept -> const schematic_ref {
		return active_;
	}

	[[nodiscard]] auto name() noexcept -> std::string& {
		return name_;
	}

	[[nodiscard]] auto name() const noexcept -> const std::string& {
		return name_;
	}

	[[nodiscard]] auto text() noexcept -> std::string& {
		return text_;
	}

	[[nodiscard]] auto text() const noexcept -> const std::string& {
		return text_;
	}

	[[nodiscard]] auto attached_events() noexcept -> tsl::hopscotch_set<std::string>& {
		return attached_events_;
	}

	[[nodiscard]] auto attached_events() const noexcept -> const tsl::hopscotch_set<std::string>& {
		return attached_events_;
	}

	void attach_event(const std::string& name) {
		attached_events_.emplace(name);
	}

	void detach_event(const std::string& name) {
		attached_events_.emplace(name);
	}

private:
	Schematic default_schematic_;
	schematic_map_t event_schematics_;
	schematic_ref active_;
	std::string name_;
	std::string text_;
	tsl::hopscotch_set<std::string> attached_events_;
};

}	 // namespace cui

#endif	  // CUI_VISUAL_NODE_HPP
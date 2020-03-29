#ifndef CUI_VISUAL_NODE_HPP
#define CUI_VISUAL_NODE_HPP

#include <visual/schematic.hpp>
#include <ext/tsl/hopscotch_map.h>

#include <string>
#include <functional>

namespace cui {

class Node
{
public:
	using schematic_map_t = tsl::hopscotch_map<std::string, Schematic>;
	using schematic_ref = std::reference_wrapper<Schematic>;

	Node() noexcept : default_schematic_(), event_schematics_(), active_(std::ref(default_schematic_)) {}

	auto default_schematic() noexcept -> Schematic& {
		return default_schematic_;
	}

	auto default_schematic() const noexcept -> const Schematic& {
		return default_schematic_;
	}

	auto event_schematics() noexcept -> schematic_map_t& {
		return event_schematics_;
	}

	auto event_schematics() const noexcept -> const schematic_map_t& {
		return event_schematics_;
	}

	auto active_schematic() noexcept -> schematic_ref& {
		return active_;
	}

	auto active_schematic() const noexcept -> const schematic_ref {
		return active_;
	}

private:
	Schematic default_schematic_;
	schematic_map_t event_schematics_;
	schematic_ref active_;
};

}	 // namespace cui

#endif	  // CUI_VISUAL_NODE_HPP
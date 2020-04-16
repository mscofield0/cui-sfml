#ifndef CUI_SFML_TEMPLATE_SWITCH_SCHEMATIC_HPP
#define CUI_SFML_TEMPLATE_SWITCH_SCHEMATIC_HPP

#include <detail/templates/requirements.hpp>

namespace cui::templates {

void SwitchToEventSchematic(Window& window, event_data_t& event_data) {
	bool scene_changed = false;
	auto& e_schemes = event_data.caller()->event_schematics();
	for (auto it = e_schemes.begin(); it != e_schemes.end(); ++it) {
		if (it.key() == event_data.event_name()) {
			event_data.caller()->active_schematic() = it.value();
			scene_changed = true;
		}
	}
	if (scene_changed) window.schedule_to_update_cache();
}

void SwitchToDefaultSchematic(Window& window, event_data_t& event_data) {
	event_data.caller()->active_schematic() = event_data.caller()->default_schematic();
	window.schedule_to_update_cache();
}

}	 // namespace cui::templates

#endif	  // CUI_SFML_TEMPLATE_SWITCH_SCHEMATIC_HPP
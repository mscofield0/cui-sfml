#ifndef CUI_SFML_EVENT_FUNCTION_HPP
#define CUI_SFML_EVENT_FUNCTION_HPP

#include <SFML/Window/Event.hpp>

namespace cui {

class EventFunction 
{
public:
	using event_t = std::function<void()>;
	using event_marker_t = sf::Event::EventType;

	EventFunction(event_t&& p_event, const event_marker_t p_marker) : event_(p_event), marker_(p_marker) {}

	[[nodiscard]] auto event() noexcept -> event_t& {
		return event_;
	}
	
	[[nodiscard]] auto event() const noexcept -> const event_t& {
		return event_;
	}
	
	[[nodiscard]] auto marker() noexcept -> event_marker_t& {
		return marker_;
	}
	
	[[nodiscard]] auto marker() const noexcept -> const event_marker_t& {
		return marker_;
	}

private:
	event_t event_;
	event_marker_t marker_;
};

}	 // namespace cui

#endif	  // CUI_SFML_EVENT_FUNCTION_HPP
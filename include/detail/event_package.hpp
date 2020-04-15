#ifndef CUI_SFML_EVENT_PACKAGE_HPP
#define CUI_SFML_EVENT_PACKAGE_HPP

#include <functional>

#include <SFML/Window/Event.hpp>

#include <detail/event_data.hpp>

namespace cui {

template <typename TEventData>
class EventPackage
{
public:
	using event_data_t = TEventData;
	using event_t = std::function<void(event_data_t)>;

	EventPackage() = default;
	EventPackage(const event_t& p_event) : event_(p_event) {}
	EventPackage(const event_t& p_event, const event_data_t& p_data) : event_(p_event), data_(p_data) {}

	void invoke() const noexcept {
		event_(data_);
	}

	[[nodiscard]] auto event() noexcept -> event_t& {
		return event_;
	}

	[[nodiscard]] auto event() const noexcept -> const event_t& {
		return event_;
	}

	[[nodiscard]] auto data() noexcept -> event_data_t& {
		return data_;
	}

	[[nodiscard]] auto data() const noexcept -> const event_data_t& {
		return data_;
	}

private:
	event_t event_;
	event_data_t data_;
};

}	 // namespace cui

#endif	  // CUI_SFML_EVENT_PACKAGE_HPP
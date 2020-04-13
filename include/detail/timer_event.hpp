#ifndef CUI_SFML_TIMER_EVENT_HPP
#define CUI_SFML_TIMER_EVENT_HPP

#include <functional>
#include <chrono>

using namespace std::literals::chrono_literals;

namespace cui {

class TimerEvent
{
public:
	using steady_clock_t = std::chrono::steady_clock;
	using standard_duration_t = steady_clock_t::duration;
	template <typename Period>
	using duration_t = std::chrono::duration<steady_clock_t::duration::rep, Period>;
	using event_t = std::function<void()>;

	template <typename Period>
	TimerEvent(const event_t& event, const duration_t<Period> wait_duration)
		: func_(event), duration_(std::chrono::duration_cast<standard_duration_t>(wait_duration)) {}

	void operator()() const {
		func_();
	}

	[[nodiscard]] auto duration() const noexcept -> standard_duration_t {
		return duration_;
	}

	[[nodiscard]] bool operator>(const TimerEvent& rhs) const noexcept {
		return duration_ > rhs.duration_;
	}

	[[nodiscard]] bool operator<(const TimerEvent& rhs) const noexcept {
		return duration_ < rhs.duration_;
	}

	[[nodiscard]] bool operator>=(const TimerEvent& rhs) const noexcept {
		return duration_ >= rhs.duration_;
	}

	[[nodiscard]] bool operator<=(const TimerEvent& rhs) const noexcept {
		return duration_ <= rhs.duration_;
	}

	[[nodiscard]] bool operator==(const TimerEvent& rhs) const noexcept {
		return duration_ == rhs.duration_;
	}

	[[nodiscard]] bool operator!=(const TimerEvent& rhs) const noexcept {
		return duration_ != rhs.duration_;
	}

private:
	event_t func_;
	standard_duration_t duration_;
};

}	 // namespace cui

#endif	  // CUI_SFML_TIMER_EVENT_HPP
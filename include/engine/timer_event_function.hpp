#ifndef CUI_SFML_TIMER_EVENT_FUNCTION_HPP
#define CUI_SFML_TIMER_EVENT_FUNCTION_HPP

#include <functional>
#include <chrono>

using namespace std::literals::chrono_literals;

namespace cui {

class TimerEventFunction
{
public:
	using duration_t = std::chrono::steady_clock::duration;

	TimerEventFunction(std::function<void()>&& p_func, duration_t nsec) : func_(std::move(p_func)), duration_(nsec) {}

	TimerEventFunction(std::function<void()>&& p_func, std::chrono::microseconds usec)
		: func_(std::move(p_func)), duration_(std::chrono::duration_cast<duration_t>(usec)) {}

	TimerEventFunction(std::function<void()>&& p_func, std::chrono::milliseconds msec)
		: func_(std::move(p_func)), duration_(std::chrono::duration_cast<duration_t>(msec)) {}

	TimerEventFunction(std::function<void()>&& p_func, std::chrono::seconds sec)
		: func_(std::move(p_func)), duration_(std::chrono::duration_cast<duration_t>(sec)) {}

	TimerEventFunction(std::function<void()>&& p_func, std::chrono::minutes min)
		: func_(std::move(p_func)), duration_(std::chrono::duration_cast<duration_t>(min)) {}

	TimerEventFunction(std::function<void()>&& p_func, std::chrono::hours hr)
		: func_(std::move(p_func)), duration_(std::chrono::duration_cast<duration_t>(hr)) {}

	void operator()() const {
		func_();
	}

	void execute() const {
		func_();
	}

	[[nodiscard]] auto duration() const noexcept -> duration_t {
		return duration_;
	}

private:
	const std::function<void()> func_;
	duration_t duration_;
};

}	 // namespace cui

#endif	  // CUI_SFML_TIMER_EVENT_FUNCTION_HPP
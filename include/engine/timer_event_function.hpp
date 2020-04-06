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

	TimerEventFunction(const std::function<void()>& p_func, const duration_t nsec) : func_(p_func), duration_(nsec) {}

	TimerEventFunction(const std::function<void()>& p_func, const std::chrono::microseconds usec)
		: func_(p_func), duration_(std::chrono::duration_cast<duration_t>(usec)) {}

	TimerEventFunction(const std::function<void()>& p_func, const std::chrono::milliseconds msec)
		: func_(p_func), duration_(std::chrono::duration_cast<duration_t>(msec)) {}

	TimerEventFunction(const std::function<void()>& p_func, const std::chrono::seconds sec)
		: func_(p_func), duration_(std::chrono::duration_cast<duration_t>(sec)) {}

	TimerEventFunction(const std::function<void()>& p_func, const std::chrono::minutes min)
		: func_(p_func), duration_(std::chrono::duration_cast<duration_t>(min)) {}

	TimerEventFunction(const std::function<void()>& p_func, const std::chrono::hours hr)
		: func_(p_func), duration_(std::chrono::duration_cast<duration_t>(hr)) {}

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
	const duration_t duration_;
};

}	 // namespace cui

#endif	  // CUI_SFML_TIMER_EVENT_FUNCTION_HPP
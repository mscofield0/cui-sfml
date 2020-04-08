#ifndef CUI_SFML_TIMER_EVENT_MANAGER_HPP
#define CUI_SFML_TIMER_EVENT_MANAGER_HPP

#include <queue>
#include <mutex>
#include <algorithm>
#include <condition_variable>

#include <engine/timer_event_function.hpp>

namespace cui {

class TimerEventManager
{
public:
	using steady_clock_t = std::chrono::steady_clock;
	using time_point_t = steady_clock_t::time_point;
	using duration_t = steady_clock_t::duration;

	void dispatch_timer_event(TimerEventFunction&& timer_evt) {
		std::unique_lock<std::mutex> lock(queue_mutex_);
		const bool was_empty = queue_.empty();
		queue_.push(std::move(timer_evt));
		if (!was_empty) {
			was_awakened = true;
		}
		cv_.notify_one();
	}

	void execute_next_in_line() {
		std::unique_lock<std::mutex> lock(queue_mutex_);
		queue_.top().execute();
		queue_.pop();
	}

	[[nodiscard]] auto get_time_until_next() noexcept -> duration_t {
		std::unique_lock<std::mutex> lock(queue_mutex_);
		return queue_.top().duration();
	}

	void wait_until_push() noexcept {
		std::unique_lock<std::mutex> lock(queue_mutex_);
		cv_.wait(lock, [this] { return this->queue_.empty(); });
	}

	[[nodiscard]] bool wait_for(duration_t& previous) {
		std::unique_lock<std::mutex> lock(queue_mutex_);
		time_point_t before = steady_clock_t::now();
		cv_.wait_for(lock, queue_.top().duration() - previous);
		previous = steady_clock_t::now() - before;
		if (was_awakened) {
			was_awakened = false;
			return true;
		}
		return false;
	}

private:
	std::priority_queue<TimerEventFunction, std::vector<TimerEventFunction>, std::greater<TimerEventFunction>> queue_;
	std::mutex queue_mutex_;
	std::condition_variable cv_;
	bool was_awakened;
};

}	 // namespace cui

#endif	  // CUI_SFML_TIMER_EVENT_MANAGER_HPP
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

	void register_timer_event(TimerEventFunction&& timer_evt) {
		std::unique_lock<std::mutex> lock(queue_mutex_);
		const bool was_empty = queue_.empty();
		queue_.push_back(std::move(timer_evt));
		std::sort(queue_.begin(), queue_.end(), [](const auto& lhs, const auto& rhs) {
			return rhs.duration() > lhs.duration();
		});
		if (was_empty) {
			cv_.notify_one();
		}
	}

	void execute_next_in_line() {
		std::unique_lock<std::mutex> lock(queue_mutex_);
		queue_.back().execute();
		queue_.pop_back();
	}

	[[nodiscard]] auto get_time_until_next() noexcept -> duration_t {
		std::unique_lock<std::mutex> lock(queue_mutex_);
		return queue_.top().duration();
	}

	void wait_until_push() noexcept {
		std::unique_lock<std::mutex> lock(queue_mutex_);
		cv_.wait(lock, [this] { return this->queue_.empty(); });
	}

private:
	std::priority_queue<TimerEventFunction, std::vector<TimerEventFunction>, std::greater<TimerEventFunction>> queue_;
	std::mutex queue_mutex_;
	std::condition_variable cv_;
};

}	 // namespace cui

#endif	  // CUI_SFML_TIMER_EVENT_MANAGER_HPP
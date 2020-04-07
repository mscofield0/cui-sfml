#ifndef CUI_SFML_EVENT_MANAGER_HPP
#define CUI_SFML_EVENT_MANAGER_HPP

#include <functional>
#include <vector>
#include <chrono>
#include <thread>
#include <mutex>
#include <shared_mutex>

#include <tsl/hopscotch_map.h>

#include <render_context/render_context.hpp>
#include <engine/event_function.hpp>
#include <engine/timer_event_manager.hpp>
#include <cui/scene_state.hpp>

namespace cui {

class EventManager
{
public:
	using scene_ref = std::reference_wrapper<SceneState>;
	using ctx_ref = const RenderContext&;
	using steady_clock_t = std::chrono::steady_clock;
	using time_point_t = std::chrono::time_point<steady_clock_t>;
	using duration_t = steady_clock_t::duration;

	EventManager(SceneState& p_graph, ctx_ref p_ctx, bool& p_running);

	EventManager(const EventManager&) = delete;
	EventManager(EventManager&&) = delete;
	EventManager& operator=(const EventManager&) = delete;
	EventManager& operator=(EventManager&&) = delete;

	void handle_events();

	void register_event(const std::string& name, std::function<void()>&& p_func);
	void register_event(std::string&& name, std::function<void()>&& p_func);

	void dispatch_event(const std::string& evt_name);

	void dispatch_as_timer_event(const std::string& evt_name, const duration_t nsec);
	void dispatch_as_timer_event(const std::string& evt_name, const std::chrono::microseconds usec);
	void dispatch_as_timer_event(const std::string& evt_name, const std::chrono::milliseconds msec);
	void dispatch_as_timer_event(const std::string& evt_name, const std::chrono::seconds sec);
	void dispatch_as_timer_event(const std::string& evt_name, const std::chrono::minutes min);
	void dispatch_as_timer_event(const std::string& evt_name, const std::chrono::hours hr);

	void process_event(sf::Event& evt);

private:
	scene_ref scene_;
	ctx_ref ctx_;
	bool& running_;
	std::vector<std::thread> pool_;
	std::thread timer_thread_;
	std::vector<EventFunction> queue_;
	TimerEventManager timer_evt_manager_;
	std::condition_variable cv_;
	std::mutex queue_mutex_;
};

EventManager::EventManager(SceneState& p_graph, ctx_ref p_ctx, bool& p_running)
	: scene_(p_graph), ctx_(p_ctx), running_(p_running) {
	const auto num_of_event_threads = std::thread::hardware_concurrency() - 1;
	pool_.reserve(num_of_event_threads);
	for (u64 i = 0; i < num_of_event_threads; ++i) {
		pool_.emplace_back([this] {
			while (true) {
				EventFunction evt;
				{
					std::unique_lock<std::mutex> lock(queue_mutex_);
					cv_.wait(lock, [this] { return !this->queue_.empty(); });
					evt = std::move(this->queue_.front());
					this->queue_.pop_back();
					cv_.notify_all();
				}
				evt();
			}
		});
	}

	timer_thread_ = std::thread([this] {
		duration_t prev = duration_t::zero();
		while (true) {
			timer_evt_manager_.wait_until_push();
			if (timer_evt_manager_.wait_for(prev)) continue;
			timer_evt_manager_.execute_next_in_line();
		}
	});
}

void EventManager::handle_events() {
	sf::Event evt;
	while (ctx_.window()->pollEvent(evt)) {
		process_event(evt);
	}
}

void EventManager::register_event(const std::string& name, std::function<void()>&& p_func) {
	std::unique_lock<std::mutex> lock(queue_mutex_);
	scene_.get().registered_events()[name] = std::move(p_func);
}

void EventManager::register_event(std::string&& name, std::function<void()>&& p_func) {
	std::unique_lock<std::mutex> lock(queue_mutex_);
	scene_.get().registered_events()[std::move(name)] = std::move(p_func);
}

void EventManager::dispatch_event(const std::string& evt_name) {
	std::unique_lock<std::mutex> lock(queue_mutex_);
	this->queue_.emplace_back(scene_.get().registered_events().at(evt_name));
}

void EventManager::dispatch_as_timer_event(const std::string& evt_name, const duration_t nsec) {
	timer_evt_manager_.dispatch_timer_event(TimerEventFunction{scene_.get().registered_events().at(evt_name), nsec});
}

void EventManager::dispatch_as_timer_event(const std::string& evt_name, std::chrono::microseconds usec) {
	const auto dur = std::chrono::duration_cast<duration_t>(usec);
	timer_evt_manager_.dispatch_timer_event(TimerEventFunction{scene_.get().registered_events().at(evt_name), dur});
}

void EventManager::dispatch_as_timer_event(const std::string& evt_name, std::chrono::milliseconds msec) {
	const auto dur = std::chrono::duration_cast<duration_t>(msec);
	timer_evt_manager_.dispatch_timer_event(TimerEventFunction{scene_.get().registered_events().at(evt_name), dur});
}

void EventManager::dispatch_as_timer_event(const std::string& evt_name, std::chrono::seconds sec) {
	const auto dur = std::chrono::duration_cast<duration_t>(sec);
	timer_evt_manager_.dispatch_timer_event(TimerEventFunction{scene_.get().registered_events().at(evt_name), dur});
}

void EventManager::dispatch_as_timer_event(const std::string& evt_name, std::chrono::minutes min) {
	const auto dur = std::chrono::duration_cast<duration_t>(min);
	timer_evt_manager_.dispatch_timer_event(TimerEventFunction{scene_.get().registered_events().at(evt_name), dur});
}

void EventManager::dispatch_as_timer_event(const std::string& evt_name, std::chrono::hours hr) {
	const auto dur = std::chrono::duration_cast<duration_t>(hr);
	timer_evt_manager_.dispatch_timer_event(TimerEventFunction{scene_.get().registered_events().at(evt_name), dur});
}

void EventManager::process_event(sf::Event& evt) {
	using EventType = sf::Event::EventType;

	switch (evt.type) {
		case EventType::Closed: {
			// running_ = false;
			break;
		}
		default: {
		}
	}
}

}	 // namespace cui

#endif	  // CUI_SFML_EVENT_MANAGER_HPP
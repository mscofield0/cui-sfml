#ifndef CUI_SFML_EVENT_MANAGER_HPP
#define CUI_SFML_EVENT_MANAGER_HPP

#include <iostream>
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
	using ctx_ref = RenderContext&;
	using steady_clock_t = std::chrono::steady_clock;
	using time_point_t = std::chrono::time_point<steady_clock_t>;
	using duration_t = steady_clock_t::duration;

	EventManager(SceneState& p_graph, ctx_ref p_ctx, bool& p_running);

	void init();

	EventManager(const EventManager&) = delete;
	EventManager(EventManager&&) = delete;
	EventManager& operator=(const EventManager&) = delete;
	EventManager& operator=(EventManager&&) = delete;

	void handle_events();

	void change_scene(SceneState& p_graph) {
		scene_ = std::ref(p_graph);
	}

	void register_event(const std::string& name, EventFunction&& p_func);
	void register_event(std::string&& name, EventFunction&& p_func);

	void dispatch_event(const std::string& evt_name);

	void dispatch_as_timer_event(const std::string& evt_name, duration_t nsec);
	void dispatch_as_timer_event(const std::string& evt_name, std::chrono::microseconds usec);
	void dispatch_as_timer_event(const std::string& evt_name, std::chrono::milliseconds msec);
	void dispatch_as_timer_event(const std::string& evt_name, std::chrono::seconds sec);
	void dispatch_as_timer_event(const std::string& evt_name, std::chrono::minutes min);
	void dispatch_as_timer_event(const std::string& evt_name, std::chrono::hours hr);

	void process_event(sf::Event& evt);

public:
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

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////

EventManager::EventManager(SceneState& p_graph, ctx_ref p_ctx, bool& p_running)
	: scene_(p_graph), ctx_(p_ctx), running_(p_running), timer_evt_manager_(p_running) {}

void EventManager::handle_events() {
	sf::Event evt;
	while (ctx_.window()->pollEvent(evt)) {
		process_event(evt);
	}
}

void EventManager::init() {
	const auto num_of_event_threads = std::thread::hardware_concurrency() - 1;
	pool_.reserve(num_of_event_threads);
	for (u64 i = 0; i < num_of_event_threads; ++i) {
		pool_.emplace_back([this] {
			while (true) {
				EventFunction evt;
				{
					std::unique_lock<std::mutex> lock(queue_mutex_);
					cv_.wait(lock, [this] { return !this->running_ || !this->queue_.empty(); });
					if (!running_ && queue_.empty()) return;
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
			if (timer_evt_manager_.wait_until_push()) return;
			if (timer_evt_manager_.wait_for(prev)) continue;
			timer_evt_manager_.execute_next_in_line();
		}
	});
}

void EventManager::register_event(const std::string& name, EventFunction&& p_func) {
	std::unique_lock<std::mutex> lock(queue_mutex_);
	scene_.get().register_event(name, std::move(p_func));
}

void EventManager::register_event(std::string&& name, EventFunction&& p_func) {
	std::unique_lock<std::mutex> lock(queue_mutex_);
	scene_.get().register_event(std::move(name), std::move(p_func));
}

void EventManager::dispatch_event(const std::string& evt_name) {
	std::unique_lock<std::mutex> lock(queue_mutex_);
	this->queue_.emplace_back(scene_.get().get_event(evt_name));
	cv_.notify_one();
}

void EventManager::dispatch_as_timer_event(const std::string& evt_name, const duration_t nsec) {
	timer_evt_manager_.dispatch_timer_event(TimerEventFunction{scene_.get().get_event(evt_name), nsec});
}

void EventManager::dispatch_as_timer_event(const std::string& evt_name, std::chrono::microseconds usec) {
	const auto dur = std::chrono::duration_cast<duration_t>(usec);
	timer_evt_manager_.dispatch_timer_event(TimerEventFunction{scene_.get().get_event(evt_name), dur});
}

void EventManager::dispatch_as_timer_event(const std::string& evt_name, std::chrono::milliseconds msec) {
	const auto dur = std::chrono::duration_cast<duration_t>(msec);
	timer_evt_manager_.dispatch_timer_event(TimerEventFunction{scene_.get().get_event(evt_name), dur});
}

void EventManager::dispatch_as_timer_event(const std::string& evt_name, std::chrono::seconds sec) {
	const auto dur = std::chrono::duration_cast<duration_t>(sec);
	timer_evt_manager_.dispatch_timer_event(TimerEventFunction{scene_.get().get_event(evt_name), dur});
}

void EventManager::dispatch_as_timer_event(const std::string& evt_name, std::chrono::minutes min) {
	const auto dur = std::chrono::duration_cast<duration_t>(min);
	timer_evt_manager_.dispatch_timer_event(TimerEventFunction{scene_.get().get_event(evt_name), dur});
}

void EventManager::dispatch_as_timer_event(const std::string& evt_name, std::chrono::hours hr) {
	const auto dur = std::chrono::duration_cast<duration_t>(hr);
	timer_evt_manager_.dispatch_timer_event(TimerEventFunction{scene_.get().get_event(evt_name), dur});
}

void EventManager::process_event(sf::Event& evt) {
	using EventType = sf::Event::EventType;

	switch (evt.type) {
		case EventType::Closed: {
			running_ = false;
			cv_.notify_all();
			timer_evt_manager_.notify();
			for (auto& thr : pool_) {
				thr.join();
			}
			timer_thread_.join();
			ctx_.window()->close();
			break;
		}
		default: {
		}
	}
}

}	 // namespace cui

#endif	  // CUI_SFML_EVENT_MANAGER_HPP
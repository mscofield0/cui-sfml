#ifndef CUI_SFML_EVENT_MANAGER_HPP
#define CUI_SFML_EVENT_MANAGER_HPP

#include <functional>
#include <vector>
#include <chrono>
#include <thread>
#include <mutex>
#include <shared_mutex>

#include <cui/visual/scene_graph.hpp>
#include <render_context/render_context.hpp>
#include <engine/event_function.hpp>
#include <engine/timer_event_manager.hpp>

namespace cui {

class EventManager
{
public:
	using graph_ref = std::reference_wrapper<SceneGraph>;
	using ctx_ref = const RenderContext&;
	using steady_clock_t = std::chrono::steady_clock;
	using time_point_t = std::chrono::time_point<steady_clock_t>;
	using duration_t = steady_clock_t::duration;

	EventManager(SceneGraph& p_graph, ctx_ref p_ctx) : graph_(std::ref(p_graph)), ctx_(p_ctx) {
		for (auto& thr : pool_) {
			thr = std::thread([this] {
				while (true) {
					EventFunction evt;
					{
						std::unique_lock<std::mutex> lock(queue_mutex_);
						cv_.wait(lock, [this] { return !this->queue_.empty(); });
						evt = std::move(this->queue_.front());
						this->queue_.pop_back();
						cv_.notify_one();
					}
					evt();
				}
			});
		}

		timer_thread_ = std::thread([this] {
			duration_t prev = duration_t::zero();
			while (true) {
				timer_evt_manager_.wait_until_push();
				const auto time_until_next = timer_evt_manager_.get_time_until_next();
				std::this_thread::sleep_for(time_until_next - prev);
				timer_evt_manager_.execute_next_in_line();
				prev = time_until_next;
			}
		});
	}

	EventManager(const EventManager&) = delete;
	EventManager(EventManager&&) = delete;
	EventManager& operator=(const EventManager&) = delete;
	EventManager& operator=(EventManager&&) = delete;

	bool poll_event();

	void register_event(std::function<void()>&& p_func);

	void process_event();

private:
	graph_ref graph_;
	ctx_ref ctx_;
	std::vector<std::thread> pool_;
	std::thread timer_thread_;
	std::vector<EventFunction> queue_;
	TimerEventManager timer_evt_manager_;
	std::condition_variable cv_;
	std::mutex queue_mutex_;
};

bool EventManager::poll_event() {
	sf::Event evt;
	const bool empty = ctx_.window()->pollEvent(evt);
	// process_event(evt)
	return empty;
}

void EventManager::register_event(std::function<void()>&& p_func) {
	this->queue_.emplace_back(std::move(p_func));
}

}	 // namespace cui

#endif	  // CUI_SFML_EVENT_MANAGER_HPP
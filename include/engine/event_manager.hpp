#ifndef CUI_SFML_EVENT_MANAGER_HPP
#define CUI_SFML_EVENT_MANAGER_HPP

#include <functional>
#include <vector>
#include <thread>
#include <mutex>
#include <shared_mutex>

#include <cui/visual/scene_graph.hpp>
#include <render_context/render_context.hpp>
#include <engine/event_function.hpp>

namespace cui {

class EventManager
{
public:
	using graph_ref = std::reference_wrapper<SceneGraph>;
	using ctx_ref = const RenderContext&;
	EventManager(SceneGraph& p_graph, ctx_ref p_ctx) : graph_(std::ref(p_graph)), ctx_(p_ctx) {
		pool_.reserve(std::thread::hardware_concurrency() - 1);

		for (auto& thread : pool_) {
			thread = std::thread([this] {
				while (true) {
					std::unique_lock<std::mutex> lock(this->data_mutex_);
					cv_.wait(lock, [this] { return !this->queue_.empty(); });
					this->queue_.front().execute();
					this->queue_.pop_back();
					cv_.notify_one();
				}
			});
		}
	}

	EventManager(const EventManager&) = delete;
	EventManager(EventManager&&) = delete;
	EventManager& operator=(const EventManager&) = delete;
	EventManager& operator=(EventManager&&) = delete;

	bool poll_event();

	template <typename Func>
	void register_event(bool will_modify, Func&& p_func);

	void process_event();

private:
	graph_ref graph_;
	ctx_ref ctx_;
	std::vector<std::thread> pool_;
	std::vector<EventFunction> queue_;
	std::condition_variable cv_;
	std::mutex data_mutex_;
	std::shared_mutex data_mutex_shared_;
};

bool EventManager::poll_event() {
	sf::Event evt;
	const bool empty = ctx_.window()->pollEvent(evt);
	// process_event(evt)
	return empty;
}

template <typename Func>
void EventManager::register_event(bool will_modify, Func&& p_func) {
	this->queue_.emplace_back(will_modify, std::move(p_func));
}

}	 // namespace cui

#endif	  // CUI_SFML_EVENT_MANAGER_HPP
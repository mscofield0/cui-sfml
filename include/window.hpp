#ifndef CUI_WINDOW_HPP
#define CUI_WINDOW_HPP

#include <functional>
#include <utility>
#include <vector>
#include <queue>
#include <mutex>
#include <shared_mutex>
#include <condition_variable>
#include <thread>
#include <string_view>

#include <cui/compile_time/style.hpp>
#include <cui/compile_time/scene.hpp>
#include <cui/scene_state.hpp>
#include <cui/containers/tracked_list.hpp>
#include <detail/timer_event.hpp>
#include <render_cache.hpp>
#include <window_options.hpp>
#include <aliases.hpp>

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics.hpp>

namespace cui {

class Window
{
	// impl type checking for RenderContext and EventManager
public:
	using steady_clock_t = std::chrono::steady_clock;
	using standard_duration_t = steady_clock_t::duration;
	using time_point_t = std::chrono::time_point<steady_clock_t>;
	template <typename Period>
	using duration_t = std::chrono::duration<steady_clock_t::duration::rep, Period>;
	using event_t = std::function<void()>;
	using timer_event_t = TimerEvent;
	using window_t = sf::RenderWindow;
	using window_ptr_t = std::unique_ptr<window_t>;
	using event_queue_t = std::vector<event_t>;
	using timer_queue_t = std::priority_queue<timer_event_t, std::vector<timer_event_t>, std::greater<timer_event_t>>;
	using cache_t = RenderCache;
	using scene_t = SceneState<event_t>;

	template <template <typename, u64> typename Container, u64 Size, typename... Scenes>
	Window(const Container<ct::Style, Size>& p_styles, const Scenes&... p_scenes)
		: scenes_{SceneGraph{p_scenes, p_styles}...}, cache_(RenderCache::populate(this->active_scene().graph())) {}

	template <template <typename, u64> typename Container, u64 Size, typename... Scenes>
	Window(Container<ct::Style, Size>&& p_styles, Scenes&&... p_scenes)
		: scenes_{SceneGraph{std::move(p_scenes), p_styles}...},
		  cache_(RenderCache::populate(this->active_scene().graph())) {}

	template <template <typename> typename Container, typename... Scenes>
	Window(const Container<ct::Style>& p_styles, const Scenes&... p_scenes)
		: scenes_{SceneGraph{p_scenes, p_styles}...}, cache_(RenderCache::populate(this->active_scene().graph())) {}

	template <template <typename> typename Container, typename... Scenes>
	Window(Container<ct::Style>&& p_styles, Scenes&&... p_scenes)
		: scenes_{SceneGraph{std::move(p_scenes), p_styles}...},
		  cache_(RenderCache::populate(this->active_scene().graph())) {}

	void init(const WindowOptions& options);

	void handle_events();

	void register_event(const std::string& name, event_t&& p_func);
	void register_event(std::string&& name, event_t&& p_func);

	void attach_event_to_node(std::string_view search_name, const std::string& event_name);

	void attach_event_to_node(std::string_view search_name, std::string&& event_name);

	void dispatch_event(const std::string& name);

	void process_event(const sf::Event& event);

	template <typename Period>
	void timer_dispatch_event(const std::string& evt_name, duration_t<Period> duration);

	void timer_execute_next_in_line();

	[[nodiscard]] bool timer_wait_until_push() noexcept;

	[[nodiscard]] bool timer_wait_for(standard_duration_t& previous);

	void update_cache();

	void render() noexcept;

	[[nodiscard]] auto active_scene() noexcept -> scene_t& {
		std::shared_lock lock(scene_mutex_);
		return scenes_.current_item();
	}

	[[nodiscard]] auto scenes() noexcept -> TrackedList<scene_t>& {
		std::shared_lock lock(scene_mutex_);
		return scenes_;
	}

	[[nodiscard]] bool is_running() noexcept {
		std::shared_lock lock(window_mutex_);
		return window_->isOpen();
	}

	[[nodiscard]] auto window() noexcept -> window_ptr_t& {
		std::shared_lock lock(window_mutex_);
		return window_;
	}

public:
	std::vector<std::thread> event_pool_;
	std::thread timer_thread_;
	event_queue_t event_queue_;
	timer_queue_t timer_queue_;
	std::mutex event_mutex_;
	std::mutex timer_mutex_;
	std::shared_mutex scene_mutex_;
	std::shared_mutex window_mutex_;
	std::condition_variable event_cv_;
	std::condition_variable timer_cv_;
	bool timer_wait_awakened;
	TrackedList<scene_t> scenes_;
	RenderCache cache_;
	std::unique_ptr<sf::RenderWindow> window_;
};

void Window::init(const WindowOptions& options) {
	const auto& [w, h, title, style, ctx_settings, framerate] = options;
	auto& graph = this->active_scene().graph();

	graph.root().default_schematic().width() = static_cast<int>(w);
	graph.root().default_schematic().height() = static_cast<int>(h);

	auto& e_schemes = graph.root().event_schematics();
	for (auto it = e_schemes.begin(); it != e_schemes.end(); ++it) {
		it.value().width() = static_cast<int>(w);
		it.value().height() = static_cast<int>(h);
	}
	cache_ = RenderCache::populate(this->active_scene().graph());

	window_ = std::make_unique<sf::RenderWindow>(sf::VideoMode(w, h), title, style, ctx_settings);
	window_->setFramerateLimit(framerate);

	const auto num_of_event_threads = std::thread::hardware_concurrency() - 1;
	event_pool_.reserve(num_of_event_threads);
	for (u64 i = 0; i < num_of_event_threads; ++i) {
		event_pool_.emplace_back([this] {
			while (true) {
				event_t evt;
				{
					std::unique_lock<std::mutex> lock(event_mutex_);
					bool running;
					event_cv_.wait(lock, [this, &running] {
						running = this->is_running();
						return !running || !this->event_queue_.empty();
					});
					if (!running && event_queue_.empty()) return;
					evt = std::move(this->event_queue_.back());
					this->event_queue_.pop_back();
					event_cv_.notify_all();
				}
				evt();
			}
		});
	}

	timer_thread_ = std::thread([this] {
		auto prev = standard_duration_t::zero();
		while (true) {
			if (this->timer_wait_until_push()) return;
			if (this->timer_wait_for(prev)) continue;
			this->timer_execute_next_in_line();
		}
	});
}

/// \brief Handles incoming events
/// \details Lets the window poll for events and then passes each enqueued event to
/// \sa Window::process_event(const sf::Event& event)
void Window::handle_events() {
	sf::Event event;
	while (window_->pollEvent(event)) {
		process_event(event);
	}
}

/// \brief Registers an event
/// \details Stores the event name and function inside the \sa SceneState event map
/// \param name The name for the event, eg. on_btn_click, on_packet_receive, ...
/// \param event The function that executes during dispatch
void Window::register_event(const std::string& name, event_t&& event) {
	std::unique_lock lock(scene_mutex_);
	this->active_scene().register_event(name, std::move(event));
}

/// \brief Registers an event
/// \details Stores the event name and function inside the \sa SceneState event map
/// \param name The name for the event, eg. on_btn_click, on_packet_receive, ...
/// \param event The function that executes during dispatch
void Window::register_event(std::string&& name, event_t&& event) {
	std::unique_lock lock(scene_mutex_);
	this->active_scene().register_event(name, std::move(event));
}

void Window::attach_event_to_node(std::string_view search_name, const std::string& event_name) {
	/*
	std::unique_lock lock(scene_mutex_);
	auto& graph = this->active_scene().graph();
	
	auto it = std::find(graph.begin(), graph.end(), [search_name](const auto& node_name) {
		return node_name.name() == search_name;
	});
	// it->attach_event(event_name);
	*/
}

/// \brief Dispatches an event
/// \details Pushes the event onto the event queue which then gets processed
/// by an available thread
/// \param name The name for the event, eg. on_btn_click, on_packet_receive, ...
void Window::dispatch_event(const std::string& name) {
	std::unique_lock<std::mutex> lock(event_mutex_);
	event_queue_.push_back(this->active_scene().get_event(name));
	event_cv_.notify_one();
}

/// \brief Dispatches a timer event
/// \details Pushes the event onto the timer event queue which then waits to execute
/// until its wait duration passes
/// \tparam Period A template parameter to accept any \sa std::ratio for the duration
/// \param name The name for the event, eg. on_btn_click, on_packet_receive, ...
/// \param duration The duration for the timer event to wait until executed
template <typename Period>
void Window::timer_dispatch_event(const std::string& name, duration_t<Period> duration) {
	std::unique_lock lock(timer_mutex_);
	const bool was_empty = timer_queue_.empty();
	const auto s_duration = std::chrono::duration_cast<standard_duration_t>(duration);
	timer_queue_.emplace(this->active_scene().get_event(name), s_duration);
	if (!was_empty) timer_wait_awakened = true;
	timer_cv_.notify_one();
}

/// \brief Execute the next timer event in waiting queue
/// \details Starts executing the event whose wait time is up
void Window::timer_execute_next_in_line() {
	std::unique_lock lock(timer_mutex_);
	timer_queue_.top()();
	timer_queue_.pop();
}

/// \brief Waits the thread until the queue has elements or the window has stopped running
/// \details Locks the timer mutex and performs an indefinite wait on enqueued timer events
/// \returns A boolean indicating whether the window has stopped running
bool Window::timer_wait_until_push() noexcept {
	std::unique_lock lock(timer_mutex_);
	bool running;
	timer_cv_.wait(lock, [this, &running] {
		running = this->is_running();
		return !running || !timer_queue_.empty();
	});
	return !running;
}

/// \brief Waits for the wait duration of the shortest timer event
/// \details The wait may be interrupted from the outside
/// \param previous The previous duration waited on
/// \returns A boolean indicating whether or not the wait has been interrupted
bool Window::timer_wait_for(standard_duration_t& previous) {
	std::unique_lock lock(timer_mutex_);
	time_point_t before = steady_clock_t::now();
	timer_cv_.wait_for(lock, timer_queue_.top().duration() - previous);
	previous = steady_clock_t::now() - before;
	if (timer_wait_awakened) {
		timer_wait_awakened = false;
		return true;
	}
	return false;
}

/// \brief Updates the internal \sa RenderCache
/// \details Locks the internal scene mutex with a \sa std::shared_lock
void Window::update_cache() {
	std::shared_lock lock(scene_mutex_);
	cache_.update_cache(this->active_scene().graph());
}

/// \brief Renders the current scene
/// \details Iterates through the cache and draws visible elements then displays them
void Window::render() noexcept {
	std::unique_lock lock(window_mutex_);
	window_->clear();
	for (const auto& ve : cache_) {
		window_->setView(ve);
		if (!ve.visible()) continue;
		window_->draw(ve);
		if (!ve.text().getString().isEmpty()) window_->draw(ve.text());
	}
	window_->display();
}

void Window::process_event(const sf::Event& evt) {
	using EventType = sf::Event::EventType;

	switch (evt.type) {
		case EventType::Closed: {
			{
				std::unique_lock lock(window_mutex_);
				window_->close();
			}
			event_cv_.notify_all();
			timer_cv_.notify_one();
			for (auto& thr : event_pool_) {
				thr.join();
			}
			timer_thread_.join();
			break;
		}
		default: {
		}
	}
}

}	 // namespace cui

#endif	  // CUI_VISUAL_WINDOW_HPP
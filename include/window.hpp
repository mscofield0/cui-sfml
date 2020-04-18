#ifndef CUI_WINDOW_HPP
#define CUI_WINDOW_HPP

#include <any>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <shared_mutex>
#include <string_view>
#include <thread>
#include <utility>
#include <vector>

#include <aliases.hpp>
#include <cui/compile_time/scene.hpp>
#include <cui/compile_time/style.hpp>
#include <cui/containers/tracked_list.hpp>
#include <cui/scene_state.hpp>
#include <detail/event_data.hpp>
#include <detail/event_package.hpp>
#include <detail/node_cache.hpp>
#include <detail/timer_event.hpp>
#include <render_cache.hpp>
#include <window_options.hpp>

#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>

namespace cui {

class Window
{
	// impl type checking for RenderContext and EventManager
public:
	using steady_clock_t = std::chrono::steady_clock;
	using standard_duration_t = steady_clock_t::duration;
	using time_point_t = std::chrono::time_point<steady_clock_t>;
	template <typename Period>
	using duration_t = std::chrono::duration<standard_duration_t::rep, Period>;
	using scene_graph_t = SceneGraph;
	using node_t = typename scene_graph_t::data_type;
	using event_data_t = EventData<node_t>;
	using event_t = std::function<void(event_data_t)>;
	using event_package_t = EventPackage<event_data_t>;
	using marker_t = sf::Event::EventType;
	using timer_event_t = TimerEvent;
	using window_t = sf::RenderWindow;
	using window_ptr_t = std::unique_ptr<window_t>;
	using event_queue_t = std::vector<event_package_t>;
	using timer_queue_t = std::priority_queue<timer_event_t, std::vector<timer_event_t>, std::greater<timer_event_t>>;
	using cache_t = RenderCache;
	using scene_t = SceneState<event_t, marker_t>;
	using event_cache_t = tsl::hopscotch_map<std::string, std::any>;

	template <template <typename, u64> typename Container, u64 Size, typename... Scenes>
	Window(const Container<ct::Style, Size>& p_styles, const Scenes&... p_scenes) : scenes_{scene_graph_t{p_scenes, p_styles}...} {}

	template <template <typename, u64> typename Container, u64 Size, typename... Scenes>
	Window(Container<ct::Style, Size>&& p_styles, Scenes&&... p_scenes) : scenes_{scene_graph_t{std::move(p_scenes), p_styles}...} {}

	template <template <typename> typename Container, typename... Scenes>
	Window(const Container<ct::Style>& p_styles, const Scenes&... p_scenes) : scenes_{scene_graph_t{p_scenes, p_styles}...} {}

	template <template <typename> typename Container, typename... Scenes>
	Window(Container<ct::Style>&& p_styles, Scenes&&... p_scenes) : scenes_{scene_graph_t{std::move(p_scenes), p_styles}...} {}

	void init(const WindowOptions& options);

	void handle_events();

	void register_event(marker_t marker, const std::string& name, event_t&& event);
	void register_event(marker_t marker, std::string&& name, event_t&& event);
	void register_global_event(marker_t marker, const std::string& name, event_t&& event);
	void register_global_event(marker_t marker, std::string&& name, event_t&& event);
	void unregister_event(marker_t marker, const std::string& name);
	void unregister_event(marker_t marker, std::string&& name);
	void unregister_global_event(marker_t marker, const std::string& name);
	void unregister_global_event(marker_t marker, std::string&& name);

	void attach_event_to_node(const std::string& search_name, const std::string& event_name);
	void attach_event_to_node(const std::string& search_name, std::string&& event_name);
	void detach_event_from_node(const std::string& search_name, const std::string& event_name);
	void detach_event_from_node(const std::string& search_name, std::string&& event_name);

	void dispatch_event(marker_t marker, const std::string& name, const event_data_t& event_data);
	void dispatch_event(marker_t marker, const std::string& name);
	void process_event(const sf::Event& event);

	template <typename Period>
	void timer_dispatch_event(marker_t marker, const std::string& evt_name, duration_t<Period> duration);
	void timer_execute_next_in_line();
	[[nodiscard]] bool timer_wait_until_push() noexcept;
	[[nodiscard]] bool timer_wait_for(standard_duration_t& previous);

	void schedule_to_update_cache();
	void update_cache();
	void render() noexcept;
	void resize(int w, int h);

	[[nodiscard]] auto active_scene() noexcept -> scene_t& {
		return scenes_.current_item();
	}

	[[nodiscard]] auto active_scene() const noexcept -> const scene_t& {
		return scenes_.current_item();
	}

	[[nodiscard]] auto scenes() noexcept -> TrackedList<scene_t>& {
		return scenes_;
	}

	[[nodiscard]] auto scenes() const noexcept -> const TrackedList<scene_t>& {
		return scenes_;
	}

	[[nodiscard]] auto cache() noexcept -> cache_t& {
		return cache_;
	}

	[[nodiscard]] auto cache() const noexcept -> const cache_t& {
		return cache_;
	}

	[[nodiscard]] bool is_running() noexcept {
		std::shared_lock lock(window_mutex);
		return running_;
	}

	[[nodiscard]] auto window() noexcept -> window_ptr_t& {
		return window_;
	}

	[[nodiscard]] auto window() const noexcept -> const window_ptr_t& {
		return window_;
	}

	void close() {
		std::unique_lock lock(window_mutex);
		running_ = false;
		timer_cv_.notify_one();
	}

	~Window() {
		window_->close();
		event_cv_.notify_all();
		timer_cv_.notify_one();
		for (auto& thr : event_pool_) {
			thr.join();
		}
		timer_thread_.join();
	}

public:
	std::mutex event_mutex;
	std::mutex timer_mutex;
	std::shared_mutex scene_mutex;
	std::shared_mutex window_mutex;
	std::shared_mutex event_cache_mutex;
	event_cache_t event_cache;

private:
	std::vector<std::thread> event_pool_;
	std::thread timer_thread_;
	event_queue_t event_queue_;
	timer_queue_t timer_queue_;
	std::condition_variable event_cv_;
	std::condition_variable timer_cv_;
	bool timer_wait_awakened_;
	bool update_cache_flag_;
	bool running_;
	TrackedList<scene_t> scenes_;
	RenderCache cache_;
	std::unique_ptr<sf::RenderWindow> window_;
};

/// \brief Initializes the window
/// \details Creates the \sa sf::RenderWindow with \sa WindowOptions and initializes the
/// the threads
/// \param options The options with which to construct the \sa sf::RenderWindow
void Window::init(const WindowOptions& options) {
	const auto& [w, h, title, style, ctx_settings, framerate] = options;
	auto& graph = this->active_scene().graph();
	this->resize(w, h);
	std::unique_lock lock(scene_mutex);

	window_ = std::make_unique<sf::RenderWindow>(sf::VideoMode(w, h), title, style, ctx_settings);

	cache_.cache_resource(graph.root());
	for (auto& node : graph) {
		cache_.cache_resource(node.data());
	}

	cache_.reserve(graph.length() + 1);
	cache_.emplace_back();
	println("Before updating cache | Size:", cache_.size());
	cache_.update_cache(graph);
	println("After updating cache | Size:", cache_.size());

	window_->setFramerateLimit(framerate);
	running_ = true;
	update_cache_flag_ = false;

	const auto num_of_event_threads = std::thread::hardware_concurrency() - 1;
	event_pool_.reserve(num_of_event_threads);
	for (u64 i = 0; i < num_of_event_threads; ++i) {
		event_pool_.emplace_back([this] {
			while (true) {
				event_package_t event_package;
				{
					std::unique_lock lock(event_mutex);
					event_cv_.wait(lock, [this] { return !this->is_running() || !this->event_queue_.empty(); });
					if (!this->is_running() && event_queue_.empty()) return;
					event_package = std::move(this->event_queue_.back());
					this->event_queue_.pop_back();
					event_cv_.notify_all();
				}
				event_package.invoke();
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

/// \brief Resizes the window
/// \details Updates the root node in all of its schematics
/// \param w The width to resize to
/// \param h The height to resize to
void Window::resize(const int w, const int h) {
	std::unique_lock lock(scene_mutex);
	auto& graph = this->active_scene().graph();

	graph.root().default_schematic().width() = static_cast<int>(w);
	graph.root().default_schematic().height() = static_cast<int>(h);

	auto& e_schemes = graph.root().event_schematics();
	for (auto it = e_schemes.begin(); it != e_schemes.end(); ++it) {
		it.value().width() = static_cast<int>(w);
		it.value().height() = static_cast<int>(h);
	}
}

/// \brief Handles incoming events
/// \details Lets the window poll for events and then passes each enqueued event to
/// \sa Window::process_event(const sf::Event& event)
void Window::handle_events() {
	sf::Event event;
	while (window_->pollEvent(event)) {
		this->process_event(event);
	}
}

/// \brief Registers an event available to nodes
/// \details Stores the event name and function inside the \sa SceneState event map
/// \param name The name for the event, eg. on_btn_click, on_packet_receive, ...
/// \param marker Marks the event to be looked up on a specific \sa sf::Event
/// \param event The function that executes during dispatch
void Window::register_event(const marker_t marker, const std::string& name, event_t&& event) {
	std::unique_lock lock(scene_mutex);
	this->active_scene().register_event(marker, name, std::move(event));
}

/// \brief Registers an event available to nodes
/// \details Stores the event name and function inside the \sa SceneState event map
/// \param name The name for the event, eg. on_btn_click, on_packet_receive, ...
/// \param marker Marks the event to be looked up on a specific \sa sf::Event
/// \param event The function that executes during dispatch
void Window::register_event(const marker_t marker, std::string&& name, event_t&& event) {
	std::unique_lock lock(scene_mutex);
	this->active_scene().register_event(marker, name, std::move(event));
}

/// \brief Registers a global event
/// \details Stores the event name and function inside the \sa SceneState event map
/// \param name The name for the event, eg. on_btn_click, on_packet_receive, ...
/// \param marker Marks the event to be looked up on a specific \sa sf::Event
/// \param event The function that executes during dispatch
void Window::register_global_event(marker_t marker, const std::string& name, event_t&& event) {
	std::unique_lock lock(scene_mutex);
	this->active_scene().register_global_event(marker, name, std::move(event));
}

/// \brief Registers a global event
/// \details Stores the event name and function inside the \sa SceneState event map
/// \param name The name for the event, eg. on_btn_click, on_packet_receive, ...
/// \param marker Marks the event to be looked up on a specific \sa sf::Event
/// \param event The function that executes during dispatch
void Window::register_global_event(marker_t marker, std::string&& name, event_t&& event) {
	std::unique_lock lock(scene_mutex);
	this->active_scene().register_global_event(marker, std::move(name), std::move(event));
}

/// \brief Unregisters an event
/// \details Erases the event from the \sa SceneState event map
/// \param name The name for the event, eg. on_btn_click, on_packet_receive, ...
/// \param marker Marks the event to be looked up on a specific \sa sf::Event
void Window::unregister_event(const marker_t marker, const std::string& name) {
	std::unique_lock lock(scene_mutex);
	this->active_scene().unregister_event(marker, name);
}

/// \brief Unregisters an event
/// \details Erases the event from the \sa SceneState event map
/// \param name The name for the event, eg. on_btn_click, on_packet_receive, ...
/// \param marker Marks the event to be looked up on a specific \sa sf::Event
void Window::unregister_event(const marker_t marker, std::string&& name) {
	std::unique_lock lock(scene_mutex);
	this->active_scene().unregister_event(marker, std::move(name));
}

/// \brief Unregisters a global event
/// \details Erases the event from the \sa SceneState event map
/// \param name The name for the event, eg. on_btn_click, on_packet_receive, ...
/// \param marker Marks the event to be looked up on a specific \sa sf::Event
void Window::unregister_global_event(const marker_t marker, const std::string& name) {
	std::unique_lock lock(scene_mutex);
	this->active_scene().unregister_global_event(marker, name);
}

/// \brief Unregisters a global event
/// \details Erases the event from the \sa SceneState event map
/// \param name The name for the event, eg. on_btn_click, on_packet_receive, ...
/// \param marker Marks the event to be looked up on a specific \sa sf::Event
void Window::unregister_global_event(const marker_t marker, std::string&& name) {
	std::unique_lock lock(scene_mutex);
	this->active_scene().unregister_global_event(marker, std::move(name));
}

/// \brief Attaches a registered event to a node
/// \details Searches for the node by name. If no node is found, an exception is thrown
/// \param search_name The name of the node to search for
/// \param event_name The name of the event that's being attached
void Window::attach_event_to_node(const std::string& search_name, const std::string& event_name) {
	std::unique_lock lock(scene_mutex);
	auto& graph = this->active_scene().graph();

	if (graph.root().name() == search_name) {
		graph.root().attach_event(event_name);
		return;
	}

	auto it = std::find_if(graph.begin(), graph.end(), [search_name](const auto& node) { return node.data().name() == search_name; });
	if (it == graph.end()) throw std::logic_error("No node found by that name");
	it->data().attach_event(event_name);
}

/// \brief Attaches a registered event to a node
/// \details Searches for the node by name. If no node is found, an exception is thrown
/// \param search_name The name of the node to search for
/// \param event_name The name of the event that's being attached
void Window::attach_event_to_node(const std::string& search_name, std::string&& event_name) {
	std::unique_lock lock(scene_mutex);
	auto& graph = this->active_scene().graph();

	if (graph.root().name() == search_name) {
		graph.root().attach_event(event_name);
		return;
	}

	auto it = std::find_if(graph.begin(), graph.end(), [search_name](const auto& node) { return node.data().name() == search_name; });
	if (it == graph.end()) throw std::logic_error("No node found by that name");
	it->data().attach_event(std::move(event_name));
}

/// \brief Detaches a registered event from a node
/// \details Searches for the node by name. If no node is found, an exception is thrown
/// \param search_name The name of the node to search for
/// \param event_name The name of the event that is being detached
void Window::detach_event_from_node(const std::string& search_name, const std::string& event_name) {
	std::unique_lock lock(scene_mutex);
	auto& graph = this->active_scene().graph();

	if (graph.root().name() == search_name) {
		graph.root().detach_event(event_name);
		return;
	}

	auto it = std::find_if(graph.begin(), graph.end(), [search_name](const auto& node) { return node.data().name() == search_name; });
	if (it == graph.end()) throw std::logic_error("No node found by that name");
	it->data().detach_event(event_name);
}

/// \brief Detaches a registered event from a node
/// \details Searches for the node by name. If no node is found, an exception is thrown
/// \param search_name The name of the node to search for
/// \param event_name The name of the event that is being detached
void Window::detach_event_from_node(const std::string& search_name, std::string&& event_name) {
	std::unique_lock lock(scene_mutex);
	auto& graph = this->active_scene().graph();

	if (graph.root().name() == search_name) {
		graph.root().detach_event(std::move(event_name));
		return;
	}

	auto it = std::find_if(graph.begin(), graph.end(), [search_name](const auto& node) { return node.data().name() == search_name; });
	if (it == graph.end()) throw std::logic_error("No node found by that name");
	it->data().detach_event(std::move(event_name));
}

/// \brief Dispatches an event with event data
/// \details Pushes the event onto the event queue which then gets processed
/// by an available thread
/// \param marker Marks the event to be looked up on a specific \sa sf::Event
/// \param name The name for the event, eg. on_btn_click, on_packet_receive, ...
/// \param event_data The event data (eg. received from sf::Event::Resized)
void Window::dispatch_event(const marker_t marker, const std::string& name, const event_data_t& event_data) {
	std::unique_lock<std::mutex> lock(event_mutex);
	event_queue_.emplace_back(event_data.has_caller() ? this->active_scene().get_event(marker, name) : this->active_scene().get_global_event(marker, name), event_data);
	event_cv_.notify_one();
}

/// \brief Dispatches a timer event
/// \details Pushes the event onto the timer event queue which then waits to execute
/// until its wait duration passes
/// \tparam Period A template parameter to accept any \sa std::ratio for the duration
/// \param marker Marks the event to be looked up on a specific \sa sf::Event
/// \param name The name for the event, eg. on_btn_click, on_packet_receive, ...
/// \param duration The duration for the timer event to wait until executed
template <typename Period>
void Window::timer_dispatch_event(const marker_t marker, const std::string& name, duration_t<Period> duration) {
	std::unique_lock lock(timer_mutex);
	const bool was_empty = timer_queue_.empty();
	const auto s_duration = std::chrono::duration_cast<standard_duration_t>(duration);
	timer_queue_.emplace(this->active_scene().get_event(marker, name), s_duration);
	if (!was_empty) timer_wait_awakened_ = true;
	timer_cv_.notify_one();
}

/// \brief Execute the next timer event in waiting queue
/// \details Starts executing the event whose wait time is up
void Window::timer_execute_next_in_line() {
	std::unique_lock lock(timer_mutex);
	timer_queue_.top()();
	timer_queue_.pop();
}

/// \brief Waits the thread until the queue has elements or the window has stopped running
/// \details Locks the timer mutex and performs an indefinite wait on enqueued timer events
/// \returns A boolean indicating whether the window has stopped running
bool Window::timer_wait_until_push() noexcept {
	std::unique_lock lock(timer_mutex);
	timer_cv_.wait(lock, [this] { return !this->is_running() || !timer_queue_.empty(); });
	return !this->is_running();
}

/// \brief Waits for the wait duration of the shortest timer event
/// \details The wait may be interrupted from the outside
/// \param previous The previous duration waited on
/// \returns A boolean indicating whether or not the wait has been interrupted
bool Window::timer_wait_for(standard_duration_t& previous) {
	std::unique_lock lock(timer_mutex);
	time_point_t before = steady_clock_t::now();
	timer_cv_.wait_for(lock, timer_queue_.top().duration() - previous);
	previous = steady_clock_t::now() - before;
	if (timer_wait_awakened_) {
		timer_wait_awakened_ = false;
		return true;
	}
	return false;
}

/// \brief Schedule to update the \sa RenderCache
/// \details Sets the update cache flag to true
void Window::schedule_to_update_cache() {
	std::unique_lock lock(window_mutex);
	update_cache_flag_ = true;
}

/// \brief Updates the internal \sa RenderCache
/// \details Locks the internal scene mutex with a \sa std::shared_lock
void Window::update_cache() {
	std::shared_lock lock(scene_mutex);
	cache_.update_cache(this->active_scene().graph());
}

/// \brief Renders the current scene
/// \details Iterates through the cache and draws visible elements then displays them
void Window::render() noexcept {
	std::unique_lock lock(window_mutex);
	if (update_cache_flag_) {
		this->update_cache();
		update_cache_flag_ = false;
	}
	window_->clear();
	for (const auto& ve : cache_) {
		window_->setView(ve);
		if (!ve.visible()) continue;
		window_->draw(ve);
		if (!ve.text().getString().isEmpty()) window_->draw(ve.text());
	}
	window_->display();
}

/// \brief Processes the polled-for event and dispatches registered events
/// \details Dispatches the events with the corresponding event marker
/// \param event The polled-for event
void Window::process_event(const sf::Event& event) {
	using EventType = sf::Event::EventType;
	const auto& type = event.type;

	std::shared_lock slock(scene_mutex);
	auto& scene = this->active_scene();

	const auto it = scene.marked_sections().find(type);

	if (it == scene.marked_sections().end()) return;
	slock.unlock();

	event_data_t event_data;

	switch (type) {
		case EventType::Resized: {
			event_data.get() = event.size;
			break;
		}
		case EventType::TextEntered: {
			event_data.get() = event.text;
			break;
		}
		case EventType::KeyPressed: {
			event_data.get() = event.key;
			break;
		}
		case EventType::KeyReleased: {
			event_data.get() = event.key;
			break;
		}
		case EventType::MouseWheelScrolled: {
			event_data.get() = event.mouseWheelScroll;
			break;
		}
		case EventType::MouseButtonPressed: {
			event_data.get() = event.mouseButton;
			break;
		}
		case EventType::MouseButtonReleased: {
			event_data.get() = event.mouseButton;
			break;
		}
		case EventType::JoystickButtonPressed: {
			event_data.get() = event.joystickButton;
			break;
		}
		case EventType::JoystickButtonReleased: {
			event_data.get() = event.joystickButton;
			break;
		}
		case EventType::JoystickMoved: {
			event_data.get() = event.joystickMove;
			break;
		}
		case EventType::JoystickConnected: {
			event_data.get() = event.joystickConnect;
			break;
		}
		case EventType::JoystickDisconnected: {
			event_data.get() = event.joystickConnect;
			break;
		}
		case EventType::TouchBegan: {
			event_data.get() = event.touch;
			break;
		}
		case EventType::TouchMoved: {
			event_data.get() = event.touch;
			break;
		}
		case EventType::TouchEnded: {
			event_data.get() = event.touch;
			break;
		}
		case EventType::SensorChanged: {
			event_data.get() = event.sensor;
			break;
		}
		case EventType::MouseMoved: {
			event_data.get() = event.mouseMove;
			break;
		}
		default: {
		}
	}

	std::unique_lock lock(scene_mutex);

	auto& graph = scene.graph();
	const auto& node_events = it->second.first;

	for (const auto& kvp : it->second.second) {
		this->dispatch_event(type, kvp.first, event_data_t(event_data.get(), kvp.first));
	}

	for (const auto& kvp : node_events) {
		const auto& event_name = kvp.first;

		for (std::size_t i = graph.length() - 1;; ++i) {
			auto& node = graph[i];

			if (node.data().attached_events().contains(event_name)) {
				this->dispatch_event(type, event_name, event_data_t(event_data.get(), &(node.data()), i + 1, event_name));
			}

			if (i == 0) break;
		}

		if (graph.root().attached_events().contains(event_name)) {
			this->dispatch_event(type, event_name, event_data_t(event_data.get(), &(graph.root()), 0, event_name));
		}
	}
}

}	 // namespace cui

#endif	  // CUI_VISUAL_WINDOW_HPP
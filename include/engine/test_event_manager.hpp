#ifndef CUI_SFML_TEST_EVENT_MANAGER_HPP
#define CUI_SFML_TEST_EVENT_MANAGER_HPP

#include <functional>
#include <vector>
#include <chrono>
#include <thread>
#include <mutex>
#include <shared_mutex>

#include <tsl/hopscotch_map.h>

#include <render_context/test_render_context.hpp>
#include <engine/event_function.hpp>
#include <engine/timer_event_manager.hpp>
#include <cui/scene_state.hpp>

namespace cui {

class TestEventManager
{
public:
	using scene_ref = std::reference_wrapper<SceneState>;
	using ctx_ref = const TestRenderContext&;
	using steady_clock_t = std::chrono::steady_clock;
	using time_point_t = std::chrono::time_point<steady_clock_t>;
	using duration_t = steady_clock_t::duration;

	TestEventManager(SceneState& p_graph, ctx_ref p_ctx, bool& p_running);

	void init() {
	}

    void handle_events() {

    }

private:
	scene_ref scene_;
	ctx_ref ctx_;
	bool& running_;
};

TestEventManager::TestEventManager(SceneState& p_graph, ctx_ref p_ctx, bool& p_running)
	: scene_(p_graph), ctx_(p_ctx), running_(p_running) {}

}	 // namespace cui

#endif	  // CUI_SFML_TEST_EVENT_MANAGER_HPP
#ifndef CUI_WINDOW_HPP
#define CUI_WINDOW_HPP

#include <array>

#include <containers/tracked_vector.hpp>
#include <compile_time/style.hpp>
#include <compile_time/scene.hpp>
#include <scene_state.hpp>
#include <aliases.hpp>

namespace cui {

static SceneGraph default_ref = SceneGraph{};

template <typename RenderContext, typename EventManager, typename WindowOptions>
class Window
{
	// impl type checking for RenderContext and EventManager
public:
	template <template <typename, u64> typename Container, u64 Size, typename... Scenes>
	Window(const Container<ct::Style, Size>& p_styles, const Scenes&... p_scenes)
		: running_(true), scenes_{SceneGraph{p_scenes, p_styles}...}, ctx_(scenes_.current_item().graph()),
		  event_manager_(scenes_.current_item(), ctx_, running_) {}

	template <template <typename> typename Container, u64 Size, typename... Scenes>
	Window(const Container<ct::Style>& p_styles, const Scenes&... p_scenes)
		: running_(true), scenes_{SceneGraph{p_scenes, p_styles}...}, ctx_(scenes_.current_item().graph()),
		  event_manager_(scenes_.current_item(), ctx_, running_) {}

	[[nodiscard]] auto current_scene() noexcept -> SceneState& {
		return scenes_.current_item();
	}

	[[nodiscard]] auto current_scene() const noexcept -> const SceneState& {
		return scenes_.current_item();
	}

	[[nodiscard]] auto scenes() noexcept -> TrackedVector<SceneState>& {
		return scenes_;
	}

	[[nodiscard]] auto scenes() const noexcept -> const TrackedVector<SceneState>& {
		return scenes_;
	}

	[[nodiscard]] bool is_running() const noexcept {
		return running_;
	}

	void init(const WindowOptions& opt) {
		ctx_.init(opt);
		event_manager_.init();
	}

	void render() const noexcept {
		ctx_.render();
	}

	// Event manager interface
	void handle_events() {
		event_manager_.handle_events();
	}

public:
	bool running_;
	TrackedVector<SceneState> scenes_;
	RenderContext ctx_;
	EventManager event_manager_;
};

}	 // namespace cui

#endif	  // CUI_VISUAL_WINDOW_HPP
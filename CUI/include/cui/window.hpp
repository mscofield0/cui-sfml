#ifndef CUI_WINDOW_HPP
#define CUI_WINDOW_HPP

#include <array>

#include <containers/tracked_vector.hpp>
#include <visual/scene_graph.hpp>
#include <compile_time/style.hpp>
#include <compile_time/scene.hpp>
#include <aliases.hpp>

namespace cui {

static SceneGraph default_ref = SceneGraph{};

template <typename RenderContext, typename EventManager>
class Window
{
	// impl type checking for RenderContext and EventManager
public:
	template <template <typename, u64> typename Container, u64 Size, typename... Scenes>
	Window(const Container<ct::Style, Size>& p_styles, const Scenes&... p_scenes)
		: scenes_{SceneGraph{p_scenes, p_styles}...}, ctx_(scenes_.current_item()),
		  event_manager_(scenes_.current_item()) {}

	template <template <typename> typename Container, u64 Size, typename... Scenes>
	Window(const Container<ct::Style>& p_styles, const Scenes&... p_scenes)
		: scenes_{SceneGraph{p_scenes, p_styles}...}, ctx_(scenes_.current_item()),
		  event_manager_(scenes_.current_item()) {}

	[[nodiscard]] auto current_scene() noexcept -> SceneGraph& {
		return scenes_.current_item();
	}

	[[nodiscard]] auto current_scene() const noexcept -> const SceneGraph& {
		return scenes_.current_item();
	}

	[[nodiscard]] auto scenes() noexcept -> TrackedVector<SceneGraph>& {
		return scenes_;
	}

	[[nodiscard]] auto scenes() const noexcept -> const TrackedVector<SceneGraph>& {
		return scenes_;
	}

	// Event manager interface
	bool poll_event() {
		return event_manager_.poll_event();
	}

private:
	TrackedVector<SceneGraph> scenes_;
	RenderContext ctx_;
	EventManager event_manager_;
};

}	 // namespace cui

#endif	  // CUI_VISUAL_WINDOW_HPP
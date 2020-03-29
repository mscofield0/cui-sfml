#ifndef CUI_VISUAL_WINDOW_HPP
#define CUI_VISUAL_WINDOW_HPP

#include <global/tracked_vector.hpp>
#include <global/reassign_type.hpp>
#include <visual/scene_graph.hpp>

#include <compile_time/static_vector.hpp>
#include <styles/style.hpp>
#include <styles/style_collection.hpp>
#include <scenes/scene.hpp>
#include <scenes/scene_collection.hpp>

#include <array>

namespace cui {

#ifndef CUI_ENABLE_CONSTEXPR_PARSING
namespace {

static SceneGraph default_ref = SceneGraph{};

}
#endif

template <typename RenderContext, typename EventManager>
class Window
{
public:
#ifdef CUI_ENABLE_CONSTEXPR_PARSING
	template <template <typename, size_t> typename Container, size_t Size, typename... Scenes>
	Window(const Container<styles::Style, Size>& p_styles, const Scenes&... p_scenes)
		: scenes_{SceneGraph{p_scenes, p_styles}...}, ctx_(scenes_.current_item()),
		  event_manager_(scenes_.current_item()) {}

#else
	template <typename ContainerA, typename ContainerB>
	Window(const ContainerA& p_scenes, const ContainerB& p_styles)
		: scenes_(), ctx_(default_ref), event_manager_(default_ref) {
		for (const auto& t_scene : p_scenes) {
			scenes_.emplace_back(t_scene, p_styles);
		}
		ctx_.assign(scenes_.current_item());
		event_manager_.assign(scenes_.current_item());
	}
#endif

	auto current_scene() noexcept -> SceneGraph& {
		return scenes_.current_item();
	}

	auto current_scene() const noexcept -> const SceneGraph& {
		return scenes_.current_item();
	}

private:
	TrackedVector<SceneGraph> scenes_;
	RenderContext ctx_;
	EventManager event_manager_;
};

}	 // namespace cui

#endif	  // CUI_VISUAL_WINDOW_HPP
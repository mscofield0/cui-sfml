#ifndef CUI_VISUAL_RENDER_CONTEXT_HPP
#define CUI_VISUAL_RENDER_CONTEXT_HPP

#include <visual/scene_graph.hpp>
#include <functional>

namespace cui {

class BaseRenderContext
{
public:
	using scene_ref = std::reference_wrapper<SceneGraph>;
	BaseRenderContext(SceneGraph& p_scene) : scene_(std::ref(p_scene)) {}

	void assign(SceneGraph& p_scene) noexcept {
		scene_ = std::ref(p_scene);
	}

	auto scene() const noexcept -> scene_ref {
		return scene_;
	}

private:
	scene_ref scene_;
};

}	 // namespace cui

#endif	  // CUI_VISUAL_RENDER_CONTEXT_HPP
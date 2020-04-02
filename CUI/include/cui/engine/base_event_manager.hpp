#ifndef CUI_BASE_EVENT_MANAGER_HPP
#define CUI_BASE_EVENT_MANAGER_HPP

#include <functional>

#include <visual/scene_graph.hpp>

namespace cui::engine {

class BaseEventManager
{
public:
	using scene_ref = std::reference_wrapper<SceneGraph>;
	BaseEventManager(SceneGraph& p_scene) : scene_(std::ref(p_scene)) {}

	void assign(SceneGraph& p_scene) noexcept {
		scene_ = std::ref(p_scene);
	}

	auto scene() const noexcept -> scene_ref {
		return scene_;
	}

	// bool poll_scene()

	// a

private:
	scene_ref scene_;
};

}	 // namespace cui::engine

#endif	  // CUI_BASE_EVENT_MANAGER_HPP
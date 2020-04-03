#ifndef CUI_SFML_RENDER_CONTEXT_HPP
#define CUI_SFML_RENDER_CONTEXT_HPP

#include <functional>

#include <cui/visual/scene_graph.hpp>
#include <render_context/visual_element.hpp>
#include <render_context/render_cache.hpp>

#include <render_context/detail/handle_x.hpp>

namespace cui {

class RenderContext
{
public:
	using graph_ref = std::reference_wrapper<const SceneGraph>;
	using node_iterator_t = typename SceneGraph::const_iterator;
	RenderContext(const SceneGraph& p_graph) : graph_(std::cref(p_graph)), cache_(graph_) {}

	static RenderCache populate_cache(const SceneGraph& graph);

	[[nodiscard]] auto graph() noexcept -> graph_ref& {
		return graph_;
	}

	[[nodiscard]] auto graph() const noexcept -> graph_ref {
		return graph_;
	}

	[[nodiscard]] auto cache() noexcept -> RenderCache& {
		return cache_;
	}

	[[nodiscard]] auto cache() const noexcept -> const RenderCache& {
		return cache_;
	}

private:
	graph_ref graph_;
	RenderCache cache_;
};

static RenderCache populate_cache(const SceneGraph& graph) {
	RenderCache cache;

	for(const auto& node_it : graph) {
		cache.emplace_back();
		update_visual_element()
	}
}

}	 // namespace cui

#endif	  // CUI_SFML_RENDER_CONTEXT_HPP
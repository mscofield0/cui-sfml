#ifndef CUI_SFML_RENDER_CONTEXT_HPP
#define CUI_SFML_RENDER_CONTEXT_HPP

#include <functional>

#include <cui/visual/scene_graph.hpp>
#include <render_context/visual_element.hpp>
#include <render_context/render_cache.hpp>

namespace cui {

class RenderCache;

class RenderContext
{
public:
	using graph_ref = std::reference_wrapper<const SceneGraph>;
	using node_iterator_t = typename SceneGraph::const_iterator;
	RenderContext(const SceneGraph& p_graph) : graph_(std::cref(p_graph)), cache_(graph_) {}

	void update_visual_element(VisualElement& ve, const SceneGraph& graph, const node_iterator_t& p_node);

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

void RenderContext::update_visual_element(VisualElement& ve, const RenderContext::node_iterator_t& p_node) {
	handle_x(*this, ve, p_node);
}

}	 // namespace cui

#endif	  // CUI_SFML_RENDER_CONTEXT_HPP
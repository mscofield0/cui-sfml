#ifndef CUI_SFML_RENDER_CACHE_HPP
#define CUI_SFML_RENDER_CACHE_HPP

#include <cui/containers/vector.hpp>
#include <cui/visual/node.hpp>
#include <cui/visual/scene_graph.hpp>
#include <render_context/visual_element.hpp>
#include <aliases.hpp>

namespace cui {

class RenderCache : public Vector<VisualElement>
{
public:
	static RenderCache populate(const SceneGraph& graph);
	void update_ve(const u64 index, const Node& node);
};

RenderCache RenderCache::populate(const SceneGraph& graph) {
	RenderCache cache;

	for(const auto& node_it : graph) {
		cache.emplace_back();
		cache.update_ve(node_it.index(), node_it.data());
	}
}

void RenderCache::update_ve(const u64 index, const Node& node) {
	handle_x(graph_.get(), cache_, p_node.data(), p_node.index(), ve);
}

}	 // namespace cui

#endif	  // CUI_SFML_RENDER_CACHE_HPP
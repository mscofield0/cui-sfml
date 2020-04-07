#ifndef CUI_SFML_RENDER_CACHE_HPP
#define CUI_SFML_RENDER_CACHE_HPP

#include <cui/containers/vector.hpp>
#include <cui/visual/node.hpp>
#include <cui/visual/scene_graph.hpp>
#include <render_context/visual_element.hpp>
#include <render_context/detail/set_x.hpp>
#include <render_context/detail/set_y.hpp>
#include <aliases.hpp>

namespace cui {

class RenderCache : public Vector<VisualElement>
{
public:
	static RenderCache populate(const SceneGraph& graph);
	void update_ve(const SceneGraph& graph, const Node& node, u64 index);
	void update_root(const SceneGraph& graph);
	void update_cache(const SceneGraph& graph);

	static void handle_x(const Schematic& scheme, VisualElement& ve);
	static void handle_y(const Schematic& scheme, VisualElement& ve);

	void handle_rule_x(const SceneGraph& graph, const Schematic& scheme, VisualElement& ve, u64 index);
	void handle_rule_y(const SceneGraph& graph, const Schematic& scheme, VisualElement& ve, u64 index);
};

RenderCache RenderCache::populate(const SceneGraph& graph) {
	RenderCache cache;
	cache.reserve(graph.length() + 1);
	cache.emplace_back();
	cache.update_cache(graph);

	return cache;
}

void RenderCache::update_cache(const SceneGraph& graph) {
	update_root(graph);

	for (const auto& node_it : graph) {
		if (node_it.index() > size()) this->emplace_back();
		update_ve(graph, node_it.data(), node_it.index());
	}
}

void RenderCache::update_root(const SceneGraph& graph) {
	const auto& root = graph.root();

	update_ve(graph, root, SceneGraph::root_index);
}

void RenderCache::update_ve(const SceneGraph& graph, const Node& node, const u64 index) {
	auto& ve = this->operator[](index + 1);
	const auto& scheme = node.active_schematic().get();

	if (scheme.x_rule()) {
		handle_rule_x(graph, scheme, ve, index);
	} else {
		handle_x(scheme, ve);
	}
}

void RenderCache::handle_x(const Schematic& scheme, VisualElement& ve) {
	const auto& val = scheme.x().integer_value();
	const auto y = ve.getPosition().y;
	ve.setPosition(val, y);
}

void RenderCache::handle_y(const Schematic& scheme, VisualElement& ve) {
	const auto& val = scheme.y().integer_value();
	const auto x = ve.getPosition().x;
	ve.setPosition(x, val);
}

void RenderCache::handle_rule_x(const SceneGraph& graph, const Schematic& scheme, VisualElement& ve, const u64 index) {
	const auto& val = scheme.x();
	if (val.is_float()) {
		const auto [x, y] = ve.getPosition();
		const auto w = ve.getSize().x;
		// Relative position in the box
		const float rel_pos = static_cast<i32>(w * val.float_value());
		// Absolute position on the window
		const float abs_pos = rel_pos + x;

		ve.setPosition(abs_pos, y);
	} else if (val.is_instruction()) {
		using namespace data_types;

		const auto parent_index = graph.get_parent_index(index);
		float x, y, w;
		if (parent_index == graph.length()) {
			const auto& root_scheme = graph.root().active_schematic().get();
			x = root_scheme.x().integer_value();
			y = ve.getPosition().y;
			w = root_scheme.width().integer_value();
		} else {
			const auto& parent_ve = this->operator[](parent_index + 1);
			x = parent_ve.getPosition().x;
			y = ve.getPosition().y;
			w = parent_ve.getSize().x;
		}

		switch (val.instruction().active()) {
			case Functions::Left: {
				ve.setPosition(x, y);
				break;
			}
			case Functions::Right: {
				ve.setPosition(x + w, y);
				break;
			}
			default: {
			}
		}
	}
}

void RenderCache::handle_rule_y(const SceneGraph& graph, const Schematic& scheme, VisualElement& ve, const u64 index) {
	const auto& val = scheme.y();
	if (val.is_float()) {
		const auto [x, y] = ve.getPosition();
		const auto h = ve.getSize().y;
		// Relative position in the box
		const float rel_pos = static_cast<i32>(h * val.float_value());
		// Absolute position on the window
		const float abs_pos = rel_pos + y;

		ve.setPosition(x, abs_pos);
	} else if (val.is_instruction()) {
		using namespace data_types;

		const auto parent_index = graph.get_parent_index(index);
		float x, y, h;
		if (parent_index == graph.length()) {
			const auto& root_scheme = graph.root().active_schematic().get();
			x = ve.getPosition().x;
			y = root_scheme.y().integer_value();
			h = root_scheme.height().integer_value();
		} else {
			const auto& parent_ve = this->operator[](parent_index + 1);
			x = ve.getPosition().y;
			y = parent_ve.getPosition().x;
			h = parent_ve.getSize().y;
		}

		switch (val.instruction().active()) {
			case Functions::Top: {
				ve.setPosition(x, y);
				break;
			}
			case Functions::Bottom: {
				ve.setPosition(x, y + h);
				break;
			}
			default: {
			}
		}
	}
}

}	 // namespace cui

#endif	  // CUI_SFML_RENDER_CACHE_HPP
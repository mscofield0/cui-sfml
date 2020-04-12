#ifndef CUI_SFML_RENDER_CACHE_HPP
#define CUI_SFML_RENDER_CACHE_HPP

#include <cui/utils/print.hpp>
#include <string>
#include <algorithm>

#include <tsl/hopscotch_map.h>
#include <cui/containers/vector.hpp>
#include <cui/utils/get_path_head.hpp>
#include <cui/visual/node.hpp>
#include <cui/visual/scene_graph.hpp>
#include <render_context/visual_element.hpp>
#include <render_context/detail/set_x.hpp>
#include <render_context/detail/set_y.hpp>
#include <render_context/detail/iterator_pair.hpp>

#include <render_context/detail/intermediaries/color.hpp>
#include <aliases.hpp>

namespace cui {

class RenderCache : public Vector<VisualElement>
{
public:
	[[nodiscard]] auto len() const noexcept -> u64 {
		return this->size() - 1;
	}

	void sort(const SceneGraph& graph) {
		auto begin = IteratorPair{graph.begin(), this->begin()};
		auto end = IteratorPair{graph.end(), this->end()};
		std::sort(begin, end, [&graph](const auto& lhs, const auto& rhs) { return lhs.depth() > rhs.depth(); });
	}

	static RenderCache populate(SceneGraph& graph);
	void update_ve(const SceneGraph& graph, const Node& node, u64 index);
	void update_root(const SceneGraph& graph);
	void update_cache(const SceneGraph& graph);

	void handle_background(const Schematic& scheme, VisualElement& ve);
	static void handle_x(const Schematic& scheme, VisualElement& ve);
	static void handle_y(const Schematic& scheme, VisualElement& ve);
	static void handle_width(const Schematic& scheme, VisualElement& ve);
	static void handle_height(const Schematic& scheme, VisualElement& ve);

	void handle_rule_x(const VisualElement& parent_ve, const Schematic& scheme, VisualElement& ve);
	void handle_rule_y(const VisualElement& parent_ve, const Schematic& scheme, VisualElement& ve);
	void handle_rule_width(const VisualElement& parent_ve, const Schematic& scheme, VisualElement& ve);
	void handle_rule_height(const VisualElement& parent_ve, const Schematic& scheme, VisualElement& ve);

private:
	tsl::hopscotch_map<std::string, sf::Texture> textures_;
};

RenderCache RenderCache::populate(SceneGraph& graph) {
	RenderCache cache;

	for (auto& node : graph) {
		auto& node_data = node.data();
		auto& d_scheme = node_data.default_schematic();
		auto& e_schemes = node_data.event_schematics();
		if (d_scheme.background().is_string()) {
			const auto& path_head = get_path_head(d_scheme.background().string());
			if (!cache.textures_.contains(path_head)) {
				cache.textures_[path_head].loadFromFile(d_scheme.background().string());
				d_scheme.background() = path_head;
			}
		}
		for (auto it = e_schemes.begin(); it != e_schemes.end(); ++it) {
			if (it.value().background().is_string()) {
				const auto& path_head = get_path_head(it.value().background().string());
				if (cache.textures_.contains(path_head)) continue;
				cache.textures_[path_head].loadFromFile(it.value().background().string());
				it.value().background() = path_head;
			}
		}
	}

	cache.reserve(graph.length() + 1);
	cache.emplace_back();
	cache.update_cache(graph);

	return cache;
}

void RenderCache::update_cache(const SceneGraph& graph) {
	update_root(graph);

	for (std::size_t i = 0; i < graph.length(); ++i) {
		if (i >= len()) this->emplace_back();
		const auto& node_data = graph[i].data();
		update_ve(graph, node_data, i);
	}

	sort(graph);
}

void RenderCache::update_root(const SceneGraph& graph) {
	const auto& root = graph.root();

	update_ve(graph, root, SceneGraph::root_index);
}

void RenderCache::update_ve(const SceneGraph& graph, const Node& node, const u64 index) {
	auto& ve = this->operator[](index + 1);
	const auto& scheme = node.active_schematic().get();
	auto parent_index = graph.get_parent_index(index);
	parent_index = parent_index == graph.length() ? 0 : parent_index + 1;
	const auto& parent_ve = this->operator[](parent_index);

	println("Node name:", node.name());
	println("Parent index:", parent_index);
	println();

	if (scheme.width_rule()) {
		handle_rule_width(parent_ve, scheme, ve);
	} else {
		handle_width(scheme, ve);
	}

	if (scheme.height_rule()) {
		handle_rule_height(parent_ve, scheme, ve);
	} else {
		handle_height(scheme, ve);
	}

	if (scheme.x_rule()) {
		handle_rule_x(parent_ve, scheme, ve);
	} else {
		handle_x(scheme, ve);
	}

	if (scheme.y_rule()) {
		handle_rule_y(parent_ve, scheme, ve);
	} else {
		handle_y(scheme, ve);
	}

	handle_background(scheme, ve);
}

void RenderCache::handle_background(const Schematic& scheme, VisualElement& ve) {
	const auto& val = scheme.background();
	// Add support for images later
	if (val.is_string()) {
		ve.setTexture(&textures_.at(val.string()));
		return;
	}
	ve.setFillColor(intermediary::Color{val.rgba()});
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

void RenderCache::handle_width(const Schematic& scheme, VisualElement& ve) {
	const auto& val = scheme.width().integer_value();
	const auto h = ve.getSize().y;
	ve.setSize(sf::Vector2f(val, h));
}

void RenderCache::handle_height(const Schematic& scheme, VisualElement& ve) {
	const auto& val = scheme.height().integer_value();
	const auto w = ve.getSize().x;
	ve.setSize(sf::Vector2f(w, val));
}

void RenderCache::handle_rule_x(const VisualElement& parent_ve, const Schematic& scheme, VisualElement& ve) {
	const auto& val = scheme.x();

	if (val.is_float()) {
		const auto [x, y] = ve.getPosition();
		const auto w = parent_ve.getSize().x;

		// Relative position in the box
		const float rel_pos = static_cast<i32>(w * val.float_value());
		// Absolute position on the window
		const float abs_pos = rel_pos + x;

		ve.setPosition(abs_pos, y);
	} else if (val.is_instruction()) {
		using namespace data_types;

		const auto x = parent_ve.getPosition().x;
		const auto y = ve.getPosition().y;
		const auto w = parent_ve.getSize().x;

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

void RenderCache::handle_rule_y(const VisualElement& parent_ve, const Schematic& scheme, VisualElement& ve) {
	const auto& val = scheme.y();
	if (val.is_float()) {
		const auto [x, y] = ve.getPosition();
		const auto h = parent_ve.getSize().y;

		// Relative position in the box
		const float rel_pos = static_cast<i32>(h * val.float_value());
		// Absolute position on the window
		const float abs_pos = rel_pos + y;

		ve.setPosition(x, abs_pos);
	} else if (val.is_instruction()) {
		using namespace data_types;

		const auto x = ve.getPosition().x;
		const auto y = parent_ve.getPosition().y;
		const auto h = parent_ve.getSize().y;

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

void RenderCache::handle_rule_width(const VisualElement& parent_ve, const Schematic& scheme, VisualElement& ve) {
	const auto& val = scheme.width();
	const auto w = parent_ve.getSize().x;
	const auto h = ve.getSize().y;

	ve.setSize(sf::Vector2f(w * val.float_value(), h));
}

void RenderCache::handle_rule_height(const VisualElement& parent_ve, const Schematic& scheme, VisualElement& ve) {
	const auto& val = scheme.height();
	const auto w = ve.getSize().x;
	const auto h = parent_ve.getSize().y;

	ve.setSize(sf::Vector2f(w, h * val.float_value()));
}

}	 // namespace cui

#endif	  // CUI_SFML_RENDER_CACHE_HPP
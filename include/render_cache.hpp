#ifndef CUI_SFML_RENDER_CACHE_HPP
#define CUI_SFML_RENDER_CACHE_HPP

#include <cui/utils/print.hpp>
#include <cui/compile_time/format/fmt.hpp>

#include <string>
#include <algorithm>

#include <tsl/hopscotch_map.h>
#include <cui/containers/vector.hpp>
#include <cui/utils/get_path_head.hpp>
#include <cui/visual/node.hpp>
#include <cui/visual/scene_graph.hpp>
#include <visual_element.hpp>
#include <detail/iterator_pair.hpp>

#include <detail/intermediaries/color.hpp>

#include <aliases.hpp>

namespace cui {

class RenderCache : public Vector<VisualElement>
{
public:
	[[nodiscard]] auto len() const noexcept -> u64 {
		return this->size() - 1;
	}

	void sort(const SceneGraph& graph);

	static RenderCache populate(SceneGraph& graph);
	void update_ve(const SceneGraph& graph, const Node& node, u64 index);
	void update_root(const SceneGraph& graph);
	void update_cache(const SceneGraph& graph);

	void handle_background(Schematic& scheme, VisualElement& ve);
	void handle_font(Schematic& scheme, VisualElement& ve);
	void handle_x(const Schematic& scheme, VisualElement& ve);
	void handle_y(const Schematic& scheme, VisualElement& ve);
	void handle_width(const Schematic& scheme, VisualElement& ve);
	void handle_height(const Schematic& scheme, VisualElement& ve);
	void handle_font_size(const Schematic& scheme, VisualElement& ve);
	void handle_text_color(const Schematic& scheme, VisualElement& ve);
	void handle_text_position(const Schematic& scheme, VisualElement& ve);

	void handle_rule_x(const VisualElement& parent_ve, const Schematic& scheme, VisualElement& ve);
	void handle_rule_y(const VisualElement& parent_ve, const Schematic& scheme, VisualElement& ve);
	void handle_rule_width(const VisualElement& parent_ve, const Schematic& scheme, VisualElement& ve);
	void handle_rule_height(const VisualElement& parent_ve, const Schematic& scheme, VisualElement& ve);

private:
	tsl::hopscotch_map<std::string, sf::Texture> textures_;
	tsl::hopscotch_map<std::string, sf::Font> fonts_;
};

RenderCache RenderCache::populate(SceneGraph& graph) {
	RenderCache cache;

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

void RenderCache::sort(const SceneGraph& graph) {
	std::vector<std::size_t> v;
	v.reserve(graph.length());
	for (const auto& node : graph) {
		v.push_back(node.depth());
	}

	auto begin = IteratorPair{v.begin(), this->begin()};
	auto end = IteratorPair{v.end(), this->end()};
	std::sort(begin, end, [&graph](const auto& lhs, const auto& rhs) { return lhs > rhs; });
}

void RenderCache::update_ve(const SceneGraph& graph, const Node& node, const u64 index) {
	auto& ve = this->operator[](index + 1);
	auto& scheme = node.active_schematic().get();
	auto parent_index = graph.get_parent_index(index);
	parent_index = parent_index == graph.length() ? 0 : parent_index + 1;
	const auto& parent_ve = this->operator[](parent_index);

	ve.text().setString(node.text());

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
	if (!scheme.font().is_none()) {
		handle_font(scheme, ve);
	}
	handle_font_size(scheme, ve);
	handle_text_color(scheme, ve);
	handle_text_position(scheme, ve);
}

void RenderCache::handle_background(Schematic& scheme, VisualElement& ve) {
	auto& val = scheme.background();
	// Add support for images later
	if (val.is_string()) {
		const auto& path_head = get_path_head(val.string());
		if (!textures_.contains(path_head)) {
			textures_[path_head].loadFromFile(val.string());
			val = path_head;
		}
		ve.setTexture(&textures_.at(val.string()));
		return;
	}
	ve.setFillColor(intermediary::Color{val.rgba()});
}

void RenderCache::handle_font(Schematic& scheme, VisualElement& ve) {
	auto& val = scheme.font();

	const auto& path_head = get_path_head(val.string());
	if (!fonts_.contains(path_head)) {
		fonts_[path_head].loadFromFile(val.string());
		val = path_head;
	}
	ve.text().setFont(fonts_[path_head]);
	return;
}

void RenderCache::handle_x(const Schematic& scheme, VisualElement& ve) {
	const auto& val = scheme.x().integer_value();
	const auto y = ve.getPosition().y;
	ve.setPosition(val, y, this->front());
}

void RenderCache::handle_y(const Schematic& scheme, VisualElement& ve) {
	const auto& val = scheme.y().integer_value();
	const auto x = ve.getPosition().x;
	ve.setPosition(x, val, this->front());
}

void RenderCache::handle_width(const Schematic& scheme, VisualElement& ve) {
	const auto& val = scheme.width().integer_value();
	const auto h = ve.getSize().y;
	ve.setSize(sf::Vector2f(val, h), this->front());
}

void RenderCache::handle_height(const Schematic& scheme, VisualElement& ve) {
	const auto& val = scheme.height().integer_value();
	const auto w = ve.getSize().x;
	ve.setSize(sf::Vector2f(w, val), this->front());
}

void RenderCache::handle_rule_x(const VisualElement& parent_ve, const Schematic& scheme, VisualElement& ve) {
	const auto& val = scheme.x();

	if (val.is_float()) {
		const auto x = parent_ve.getPosition().x;
		const auto y = ve.getPosition().y;
		const auto w = parent_ve.getSize().x;

		// Relative position in the box
		const float rel_pos = static_cast<i32>(w * val.float_value());
		// Absolute position on the window
		const float abs_pos = rel_pos + x;

		ve.setPosition(abs_pos, y, this->front());
	} else if (val.is_instruction()) {
		using namespace data_types;

		const auto [x, y] = ve.getPosition();
		const auto [w, h] = ve.getSize();
		const auto [px, py] = parent_ve.getPosition();
		const auto [pw, ph] = parent_ve.getSize();

		switch (val.instruction().active()) {
			case Functions::Left: {
				ve.setPosition(px, y, this->front());
				break;
			}
			case Functions::Right: {
				ve.setPosition(px + pw - w, y, this->front());
				break;
			}
			case Functions::Center: {
				ve.setPosition((px + pw) / 2 - w / 2, y, this->front());
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
		const auto x = ve.getPosition().x;
		const auto y = parent_ve.getPosition().y;
		const auto h = parent_ve.getSize().y;

		// Relative position in the box
		const float rel_pos = static_cast<i32>(h * val.float_value());
		// Absolute position on the window
		const float abs_pos = rel_pos + y;

		ve.setPosition(x, abs_pos, this->front());
	} else if (val.is_instruction()) {
		using namespace data_types;

		const auto [x, y] = ve.getPosition();
		const auto [w, h] = ve.getSize();
		const auto [px, py] = parent_ve.getPosition();
		const auto [pw, ph] = parent_ve.getSize();

		switch (val.instruction().active()) {
			case Functions::Top: {
				ve.setPosition(x, py, this->front());
				break;
			}
			case Functions::Bottom: {
				ve.setPosition(x, py + ph - h, this->front());
				break;
			}
			case Functions::Center: {
				ve.setPosition(x, (py + ph) / 2 - h / 2, this->front());
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

	ve.setSize(sf::Vector2f(w * val.float_value(), h), this->front());
}

void RenderCache::handle_rule_height(const VisualElement& parent_ve, const Schematic& scheme, VisualElement& ve) {
	const auto& val = scheme.height();
	const auto w = ve.getSize().x;
	const auto h = parent_ve.getSize().y;

	ve.setSize(sf::Vector2f(w, h * val.float_value()), this->front());
}

void RenderCache::handle_font_size(const Schematic& scheme, VisualElement& ve) {
	const auto& val = scheme.font_size();

	ve.text().setCharacterSize(val.integer_value());
}

void RenderCache::handle_text_color(const Schematic& scheme, VisualElement& ve) {
	const auto& val = scheme.text_color();

	ve.text().setFillColor(intermediary::Color{val.rgba()});
}

void RenderCache::handle_text_position(const Schematic& scheme, VisualElement& ve) {
	const auto& val = scheme.text_position();
	const auto [x, y] = ve.getPosition();
	const auto [w, h] = ve.getSize();

	const auto [_0, _1, tw, th] = ve.text().getGlobalBounds();
	float nx, ny;

	using namespace data_types;
	switch (val.instruction().active()) {
		case Functions::TopLeft: {
			nx = x;
			ny = y;

			break;
		}
		case Functions::Top: {
			nx = x + w / 2 - tw / 2;
			ny = y;

			break;
		}
		case Functions::TopRight: {
			nx = x + w - tw;
			ny = y;

			break;
		}
		case Functions::Left: {
			nx = x;
			ny = y + h / 2 - th / 2;

			break;
		}
		case Functions::Center: {
			nx = x + w / 2 - tw / 2;
			ny = y + h / 2 - th / 2;

			break;
		}
		case Functions::Right: {
			nx = x + w - tw;
			ny = y + h / 2 - th / 2;

			break;
		}
		case Functions::BottomLeft: {
			nx = x;
			ny = y + h - th;

			break;
		}
		case Functions::Bottom: {
			nx = x + w / 2 - tw / 2;
			ny = y + h - th;

			break;
		}
		case Functions::BottomRight: {
			nx = x + w - tw;
			ny = y + h - th;

			break;
		}
	}

	ve.text().setPosition(nx, ny);
}

}	 // namespace cui

#endif	  // CUI_SFML_RENDER_CACHE_HPP
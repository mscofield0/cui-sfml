#ifndef CUI_SFML_RENDER_CACHE_HPP
#define CUI_SFML_RENDER_CACHE_HPP

#include <cui/compile_time/format/fmt.hpp>
#include <cui/utils/print.hpp>

#include <algorithm>
#include <string>

#include <aliases.hpp>
#include <cui/utils/get_path_head.hpp>
#include <cui/visual/node.hpp>
#include <cui/visual/scene_graph.hpp>
#include <detail/intermediaries/color.hpp>
#include <detail/iterator_pair.hpp>
#include <tsl/hopscotch_map.h>
#include <visual_element.hpp>

namespace cui {

/// \brief Class for transforming CUI attributes and rules into a \sa cui::VisualElement
/// \details Holds all VEs in a \sa cui::Vector which gets sorted according to the \sa cui::SceneGraph node depths
class RenderCache : public std::vector<VisualElement>
{
public:
	[[nodiscard]] auto len() const noexcept -> u64 {
		return this->size() - 1;
	}

	void sort(const SceneGraph& graph);

	void cache_resource(Node& node);

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

public:
	tsl::hopscotch_map<std::string, sf::Texture> textures;
	tsl::hopscotch_map<std::string, sf::Font> fonts;
};

/// \brief Caches \sa cui::Node resources such as images and fonts
/// \details Does not cache twice, the resource exists throughout the existence of the \sa cui::window
/// \param node The node from which to cache resources
void RenderCache::cache_resource(Node& node) {
	{
		auto& scheme = node.default_schematic();
		auto& background = scheme.background();
		auto& font = scheme.font();
		if (background.is_string()) {
			const auto path_head = get_path_head(background.string());
			if (!textures.contains(path_head)) {
				println("Added texture named:", path_head);
				textures[path_head].loadFromFile(background.string());
			}
			background = path_head;
		}
		if (font.is_string()) {
			const auto path_head = get_path_head(font.string());
			if (!fonts.contains(path_head)) {
				println("Added font named:", path_head);
				fonts[path_head].loadFromFile(font.string());
			}
			font = path_head;
		}
	}

	for (auto kvp_it = node.event_schematics().begin(); kvp_it != node.event_schematics().end(); ++kvp_it) {
		auto& background = kvp_it.value().background();
		auto& font = kvp_it.value().font();
		if (background.is_string()) {
			const auto path_head = get_path_head(background.string());
			if (!textures.contains(path_head)) {
				println("Added texture named:", path_head);
				textures[path_head].loadFromFile(background.string());
			}
			background = path_head;
		}
		if (font.is_string()) {
			const auto path_head = get_path_head(font.string());
			if (!fonts.contains(path_head)) {
				println("Added font named:", path_head);
				fonts[path_head].loadFromFile(font.string());
			}
			font = path_head;
		}
	}
}

/// \brief Updates the cache
/// \details Iterates through the graph and updates each \sa cui::VisualElement
void RenderCache::update_cache(const SceneGraph& graph) {
	update_root(graph);

	for (std::size_t i = 0; i < graph.length(); ++i) {
		if (i >= len()) this->emplace_back();
		const auto& node_data = graph[i].data();
		update_ve(graph, node_data, i);
	}

	sort(graph);
}

/// \brief Updates the root node of the \sa cui::SceneGraph
/// \details Calls \sa cui::RenderCache::update_ve() on the root node
/// \param graph The graph from which to update the root node
void RenderCache::update_root(const SceneGraph& graph) {
	const auto& root = graph.root();

	update_ve(graph, root, SceneGraph::root_index);
}

/// \brief Sorts the cache according to the \sa cui::SceneGraph node depths
/// \details Creates a zip iterator to the depths vector of the \sa cui::SceneGraph and
/// the cache vector, then sorts it accordingly
/// \param graph The graph from which to sort the cache
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

/// \brief Updates the \sa cui::VisualElement according to the \sa cui::SceneGraph node attributes and rules
/// \details Updates the entire \sa cui::VisualElement and does no attempt to figure out which ones do not need
/// to be updated
/// \param graph The \sa cui::SceneGraph from which the node is used to update the corresponding \sa cui::VisualElement
/// \param node The node being used to update the corresponding \sa cui::VisualElement
/// \param index The index of the node in the \sa cui::SceneGraph nodes vector
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
	handle_font(scheme, ve);
	handle_font_size(scheme, ve);
	handle_text_color(scheme, ve);
	handle_text_position(scheme, ve);
}

void RenderCache::handle_background(Schematic& scheme, VisualElement& ve) {
	auto& val = scheme.background();
	// Add support for images later
	if (val.is_string()) {
		ve.setTexture(&textures.at(val.string()));
		return;
	}
	ve.setFillColor(intermediary::Color{val.rgba()});
}

void RenderCache::handle_font(Schematic& scheme, VisualElement& ve) {
	auto& val = scheme.font();
	if (val.is_none()) return;

	ve.text().setFont(fonts.at(val.string()));
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
		const auto [x, y] = ve.getPosition();
		const auto [w, h] = ve.getSize();
		const auto [px, py] = parent_ve.getPosition();
		const auto [pw, ph] = parent_ve.getSize();

		switch (val.instruction()) {
			case Instruction::Left: {
				ve.setPosition(px, y, this->front());
				break;
			}
			case Instruction::Right: {
				ve.setPosition(px + pw - w, y, this->front());
				break;
			}
			case Instruction::Center: {
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
		const auto [x, y] = ve.getPosition();
		const auto [w, h] = ve.getSize();
		const auto [px, py] = parent_ve.getPosition();
		const auto [pw, ph] = parent_ve.getSize();

		switch (val.instruction()) {
			case Instruction::Top: {
				ve.setPosition(x, py, this->front());
				break;
			}
			case Instruction::Bottom: {
				ve.setPosition(x, py + ph - h, this->front());
				break;
			}
			case Instruction::Center: {
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
	float nx = 0, ny = 0;

	switch (val.instruction()) {
		case Instruction::TopLeft: {
			nx = x;
			ny = y;

			break;
		}
		case Instruction::Top: {
			nx = x + w / 2 - tw / 2;
			ny = y;

			break;
		}
		case Instruction::TopRight: {
			nx = x + w - tw;
			ny = y;

			break;
		}
		case Instruction::Left: {
			nx = x;
			ny = y + h / 2 - th / 2;

			break;
		}
		case Instruction::Center: {
			nx = x + w / 2 - tw / 2;
			ny = y + h / 2 - th / 2;

			break;
		}
		case Instruction::Right: {
			nx = x + w - tw;
			ny = y + h / 2 - th / 2;

			break;
		}
		case Instruction::BottomLeft: {
			nx = x;
			ny = y + h - th;

			break;
		}
		case Instruction::Bottom: {
			nx = x + w / 2 - tw / 2;
			ny = y + h - th;

			break;
		}
		case Instruction::BottomRight: {
			nx = x + w - tw;
			ny = y + h - th;

			break;
		}
	}

	ve.text().setPosition(nx, ny);
}

}	 // namespace cui

#endif	  // CUI_SFML_RENDER_CACHE_HPP
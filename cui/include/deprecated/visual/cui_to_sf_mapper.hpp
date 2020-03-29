#ifndef CUI_TO_SF_MAPPER_HPP
#define CUI_TO_SF_MAPPER_HPP

#include <SFML/Graphics.hpp>
#include "./draw_instruction.hpp"
#include "../ct/styles_data.hpp"

namespace cui {

template <typename Drawable>
struct sf_mapper
{};

template <>
struct sf_mapper<sf::RectangleShape>
{
private:
	static void set_fill_color(sf::RectangleShape& drawable, cui::attribute const& attr) {
		auto const& color = attr.value.rgba;
		drawable.setFillColor(sf::Color{static_cast<sf::Uint32>(color.rgba)});
	}

	template <typename SceneGraph>
	static void set_size(sf::RectangleShape& drawable, cui::attribute const& attr, SceneGraph const* graph) {
		auto const& val = attr.value;
		if (val.active == val.active_::INSTR) {
			auto instr = val.binstr;
			using instr_ = cui::block_instructions;
			// Apply different instructions
			switch (instr) {
				case instr_::CONTAIN: {
					auto const parent = graph->find_parent(graph->current_tracked_node);
					if (parent == graph->nodes.size()) {
						auto const [x, y] = graph->root.drawable.getSize();
						drawable.setSize(x, y);
					} else {
						auto const [x, y] = graph->nodes[parent].drawable.getSize();
						drawable.setSize(x, y);
					}
					break;
				}
			}
			return;
		}
		auto const& size = val.v2;
		if (size.active == size.active_::INT) {
			auto const& [x, y] = size.val_i;
			drawable.setSize(sf::Vector2f{x, y});
		} else if (size.active == size.active_::FLOAT) {
			auto const& [x, y] = size.val_f;
			drawable.setSize(sf::Vector2f{x, y});
		}
	}

public:
	template <typename SceneGraph>
	static void apply_attribute(sf::RectangleShape& drawable, cui::attribute const& attr, SceneGraph const* graph) {
		enum attr_code : uint8_t
		{

			FILL_COLOR = 0,
			SIZE,
			POSITION,
			TEXTURE,
			TEXTURE_RECT,
			OUTLINE_COLOR,
			OUTLINE_THICKNESS,
			ROTATION,
			SCALE,
			ORIGIN,
			INVALID
		} code = INVALID;

		for (uint8_t i = 0; i < cui::attr_types__.size(); ++i) {
			if (attr.type == cui::attr_types__[i]) {
				code = static_cast<attr_code>(i);
				break;
			}
		}

		// Shouldn't ever happen
		if (code == INVALID) return;

		switch (code) {
			case FILL_COLOR: {
				set_fill_color(drawable, attr);
				break;
			}
			case SIZE: {
				set_size(drawable, attr, graph);
				break;
			}
			case POSITION: {
				auto const& pos = attr.value.v2;
				if (pos.active == pos.active_::INT) {
					auto& [x, y] = pos.val_i;
					drawable.setPosition(sf::Vector2f{x, y});
				} else if (pos.active == pos.active_::FLOAT) {
					auto& [x, y] = pos.val_f;
					drawable.setPosition(sf::Vector2f{x, y});
				}
				break;
			}
		}
	}
};

}	 // namespace cui

#endif	  // CUI_TO_SF_MAPPER_HPP
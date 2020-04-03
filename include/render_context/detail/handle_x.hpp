#ifndef CUI_SFML_DETAIL_HANDLE_X_HPP
#define CUI_SFML_DETAIL_HANDLE_X_HPP

#include <render_context/detail/dependencies.hpp>

namespace cui {

void handle_x(RenderContext& ctx, VisualElement& ve, const RenderContext::node_iterator_t& p_node) {
	const auto& node = p_node.data();
	const auto& index = p_node.index();
	const auto& graph = ctx.graph().get();

	const auto& scheme = node.active_schematic().get();

	if (scheme.x_rule()) {
		const auto& val = scheme.x();
		if (val.is_float()) {
			const auto [x, y] = ve.getPosition();
			const auto [w, h] = ve.getSize();
			// Relative position in the box
			const float rel_pos = static_cast<i32>(w * val.float_value());
			// Absolute position on the window
			const float abs_pos = rel_pos + x;

			ve.setPosition(abs_pos, y);
		} else if (val.is_instruction()) {
			using namespace data_types;

			const auto parent_index = graph.get_parent_index(index);
			const auto& parent_ve = ctx.cache()[parent_index];

			const auto [x, _] = parent_ve.getPosition();
			const auto [_, y] = ve.getPosition();

			switch (val.instruction().active()) {
				case Functions::Left: {
					ve.setPosition(x, y);
					break;
				}
				case Functions::Right: {
					const auto [w, _] = parent_ve.getSize();
					ve.setPosition(x + w, y);
					break;
				}
				default: {
				}
			}
		}
	} else {
		const auto& val = scheme.x().integer_value();
		const auto [_, y] = ve.getPosition();
		ve.setPosition(val, y);
	}
}

}	 // namespace cui

#endif	  // CUI_SFML_DETAIL_HANDLE_X_HPP
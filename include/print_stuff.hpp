#ifndef CUI_SFML_PRINT_STUFF_HPP
#define CUI_SFML_PRINT_STUFF_HPP

#include <cui/compile_time/scene.hpp>
#include <cui/compile_time/string/string_view.hpp>
#include <cui/compile_time/styles/detail/constants.hpp>
#include <cui/compile_time/value_data.hpp>
#include <cui/visual/node.hpp>
#include <cui/visual/scene_graph.hpp>
#include <detail/intermediaries/color.hpp>
#include <iostream>
#include <type_traits>
#include <visual_element.hpp>

std::ostream& operator<<(std::ostream& os, const cui::ct::StringView str) {
	for (const auto ch : str) os << ch;
	return os;
}

template <std::size_t Size>
std::ostream& operator<<(std::ostream& os, const cui::ct::Format<Size>& format) {
	os << format.data().data();
	return os;
}

std::ostream& operator<<(std::ostream& os, const cui::ct::styles::Definition& val) {
	os << val.identifier() << " {\n\tEvents: {\n\t\t";
	if (!val.events().empty()) {
		for (size_t i = 0; i < val.events().size() - 1; ++i) {
			os << val.events()[i] << "\n\t\t";
		}
		os << val.events().back();
	}
	os << "\n\t}\n\tAttributes: {\n\t\t";
	if (!val.attributes().empty()) {
		for (size_t i = 0; i < val.attributes().size() - 1; ++i) {
			os << val.attributes()[i].type() << "-->" << val.attributes()[i].value() << "\n\t\t";
		}
		os << val.attributes().back().type() << "-->" << val.attributes().back().value();
	}
	os << "\n\t}\n\t";

	return os;
}

std::ostream& operator<<(std::ostream& os, const cui::ValueData& val) {
	switch (val.active()) {
		case cui::DataTypes::None: {
			os << "Nothing";
			break;
		}
		case cui::DataTypes::Color: {
			os << "rgba(" << val.rgba().red() << "," << val.rgba().green() << "," << val.rgba().blue() << ',' << val.rgba().alpha() << ')';
			break;
		}
		case cui::DataTypes::Float: {
			os << val.float_value();
			break;
		}
		case cui::DataTypes::Int: {
			os << val.integer_value();
			break;
		}
		case cui::DataTypes::Vec2: {
			os << "vec(";
			for (size_t i = 0; i < 1; ++i) {
				os << val.vec2()[i] << ',';
			}
			os << val.vec2().back() << ')';
			break;
		}
		case cui::DataTypes::Vec3: {
			os << "vec(";
			for (size_t i = 0; i < 2; ++i) {
				os << val.vec3()[i] << ',';
			}
			os << val.vec3().back() << ')';
			break;
		}
		case cui::DataTypes::Vec4: {
			os << "vec(";
			for (size_t i = 0; i < 3; ++i) {
				os << val.vec4()[i] << ',';
			}
			os << val.vec4().back() << ')';
			break;
		}
		case cui::DataTypes::Instruction: {
			os << "Instruction";
			break;
		}
		case cui::DataTypes::String: {
			os << val.string();
			break;
		}
		default: {
			throw std::logic_error("Shouldn't happen!");
		}
	}
	return os;
}

std::ostream& operator<<(std::ostream& os, const cui::ct::Style& style) {
	os << style.name() << " {\n\tEvents: {\n\t\t";
	if (!style.events().empty()) {
		for (size_t i = 0; i < style.events().size() - 1; ++i) {
			os << style.events()[i] << "\n\t\t";
		}
		os << style.events().back();
	}
	os << "\n\t}\n\tAttributes: {\n\t\t";
	if (!style.attributes().empty()) {
		for (size_t i = 0; i < style.attributes().size() - 1; ++i) {
			os << style.attributes()[i].type() << "-->" << style.attributes()[i].value() << "\n\t\t";
		}
		os << style.attributes().back().type() << "-->" << style.attributes().back().value();
	}
	os << "\n\t}\n}";

	return os;
}

template <std::size_t N>
std::ostream& operator<<(std::ostream& os, const cui::ct::Scene<N>& val) {
	for (std::size_t i = 0; i < val.blocks().size(); ++i) {
		const auto& block = val.blocks()[i];
		os << "Block: {\n\tName: " << block.name() << "\n\tStyle list: {\n\t\t";
		if (!block.style_list().empty()) {
			for (std::size_t j = 0; j < block.style_list().size() - 1; ++j) {
				const auto& style = block.style_list()[j];
				os << style << ",\n\t\t";
			}
			os << block.style_list().back();
		}
		os << "\n\t}\n\tChildren: {\n\t\t";
		if (!val.children()[i].empty()) {
			for (std::size_t j = 0; j < val.children()[i].size() - 1; ++j) {
				const auto& child = val.blocks()[val.children()[i][j]];
				os << child.name() << ",\n\t\t";
			}
			os << val.blocks()[val.children()[i].back()].name();
		}
		os << "\n\t}\n\tDepth: " << val.depths()[i] << "\n}\n--------------------------------------------------\n";
	}

	return os;
}

std::ostream& operator<<(std::ostream& os, const cui::Schematic& sg) {
	os << "x: " << sg.x() << (sg.x_rule() ? "[RULE]" : "") << "\n\t\t";
	os << "y: " << sg.y() << (sg.y_rule() ? "[RULE]" : "") << "\n\t\t";
	os << "width: " << sg.width() << (sg.width_rule() ? "[RULE]" : "") << "\n\t\t";
	os << "height: " << sg.height() << (sg.height_rule() ? "[RULE]" : "") << "\n\t\t";
	os << "background: " << sg.background() << "\n\t\t";
	os << "text_color: " << sg.text_color() << "\n\t\t";
	os << "font_size: " << sg.font_size() << "\n\t\t";
	os << "font: " << sg.font() << "\n\t\t";
	return os;
}

std::ostream& operator<<(std::ostream& os, const cui::SceneGraph& sg) {
	os << sg.root().name() << "{\n\t";
	os << "Text: " << sg.root().text() << "\n\t";
	os << "Default attributes:\n\t\t";
	os << sg.root().default_schematic() << "\n\t";
	for (const auto& kvp : sg.root().event_schematics()) {
		os << "Event schematic (" << kvp.first << ")\n\t\t";
		os << kvp.second << "\n\t";
	}
	os << "Active schematic\n\t\t";
	os << sg.root().active_schematic().get();
	os << "\n}";

	for (std::size_t i = 0; i < sg.length(); ++i) {
		const auto& node = sg[i];

		os << node.data().name() << "{\n\t";
		os << "Text: " << node.data().text() << "\n\t";
		os << "Index: " << i << " | ";
		os << "Depth: " << node.depth() << "\n\t";
		os << "Default attributes:\n\t\t";
		os << node.data().default_schematic() << "\n\t";
		for (const auto& kvp : node.data().event_schematics()) {
			os << "Event schematic (" << kvp.first << ")\n\t\t";
			os << kvp.second << "\n\t";
		}
		os << "Active schematic\n\t\t";
		os << node.data().active_schematic().get();
		os << "\n}";
	}
	return os;
}

std::ostream& operator<<(std::ostream& os, const cui::VisualElement& ve) {
	const auto [x, y] = ve.getPosition();
	const auto [w, h] = ve.getSize();
	const cui::Color bg = cui::intermediary::Color{ve.getFillColor()};
	const auto is_visible = ve.visible();

	os << "VE:";
	os << "\n\tX:" << x << ',' << "Y:" << y;
	os << "\n\tW:" << w << ',' << "H:" << h;
	os << "\n\trgba(" << bg.red() << "," << bg.green() << "," << bg.blue() << ',' << bg.alpha() << ')';
	os << "\n\ttext: " << ve.text().getString().toAnsiString();
	os << "\n\tVisible?: " << (is_visible ? "true" : "false");
	return os;
}

#endif	  // CUI_SFML_PRINT_STUFF_HPP
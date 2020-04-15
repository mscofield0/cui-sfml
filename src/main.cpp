#include <cui/compile_time/styles/detail/constants.hpp>
#include <cui/compile_time/style.hpp>
#include <cui/compile_time/scene.hpp>
#include <cui/compile_time/value_data.hpp>
#include <cui/compile_time/styles/parse_styles.hpp>
#include <cui/compile_time/scenes/parse_scenes.hpp>
#include <cui/compile_time/string/string_view.hpp>

#include <cui/visual/scene_graph.hpp>
#include <cui/visual/node.hpp>
#include <window.hpp>

#include <aliases.hpp>
#include <iostream>
#include <type_traits>
#include <utility>
#include <random>
#include <string>

#include <window_options.hpp>
#include <render_cache.hpp>
#include <detail/intermediaries/color.hpp>
#include <cui/utils/print.hpp>

std::ostream& operator<<(std::ostream& os, const cui::ct::StringView str) {
	for (const auto ch : str) os << ch;
	return os;
}

template <cui::u32 Size>
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
		case cui::data_types::DataTypes::None: {
			os << "Nothing";
			break;
		}
		case cui::data_types::DataTypes::Color: {
			os << "rgba(" << val.rgba().red() << "," << val.rgba().green() << "," << val.rgba().blue() << ','
			   << val.rgba().alpha() << ')';
			break;
		}
		case cui::data_types::DataTypes::Float: {
			os << val.float_value();
			break;
		}
		case cui::data_types::DataTypes::Int: {
			os << val.integer_value();
			break;
		}
		case cui::data_types::DataTypes::Vec2: {
			os << "vec(";
			for (size_t i = 0; i < 1; ++i) {
				os << val.vec2()[i] << ',';
			}
			os << val.vec2().back() << ')';
			break;
		}
		case cui::data_types::DataTypes::Vec3: {
			os << "vec(";
			for (size_t i = 0; i < 2; ++i) {
				os << val.vec3()[i] << ',';
			}
			os << val.vec3().back() << ')';
			break;
		}
		case cui::data_types::DataTypes::Vec4: {
			os << "vec(";
			for (size_t i = 0; i < 3; ++i) {
				os << val.vec4()[i] << ',';
			}
			os << val.vec4().back() << ')';
			break;
		}
		case cui::data_types::DataTypes::Instruction: {
			os << "Instruction";
			break;
		}
		case cui::data_types::DataTypes::String: {
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

template <cui::u64 N>
std::ostream& operator<<(std::ostream& os, const cui::ct::Scene<N>& val) {
	for (cui::u64 i = 0; i < val.blocks().size(); ++i) {
		const auto& block = val.blocks()[i];
		os << "Block: {\n\tName: " << block.name() << "\n\tStyle list: {\n\t\t";
		if (!block.style_list().empty()) {
			for (cui::u64 j = 0; j < block.style_list().size() - 1; ++j) {
				const auto& style = block.style_list()[j];
				os << style << ",\n\t\t";
			}
			os << block.style_list().back();
		}
		os << "\n\t}\n\tChildren: {\n\t\t";
		if (!val.children()[i].empty()) {
			for (cui::u64 j = 0; j < val.children()[i].size() - 1; ++j) {
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

template <typename TNodeCache>
std::ostream& operator<<(std::ostream& os, const cui::SceneGraph<TNodeCache>& sg) {
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
	const cui::data_types::Color bg = cui::intermediary::Color{ve.getFillColor()};
	const auto is_visible = ve.visible();

	os << "VE:";
	os << "\n\tX:" << x << ',' << "Y:" << y;
	os << "\n\tW:" << w << ',' << "H:" << h;
	os << "\n\trgba(" << bg.red() << "," << bg.green() << "," << bg.blue() << ',' << bg.alpha() << ')';
	os << "\n\tVisible?: " << (is_visible ? "true" : "false");
	return os;
}

#define STATIC_STRING_HOLDER(name) static constexpr const char name[] =
#define END_STATIC_STRING_HOLDER ;

int main() {
	using namespace cui;

	STATIC_STRING_HOLDER(style__)
#include "file.styles"
	END_STATIC_STRING_HOLDER

	STATIC_STRING_HOLDER(scene__)
#include "file.scene"
	END_STATIC_STRING_HOLDER

	using win_t = Window;

	std::unique_ptr<win_t> window;

	std::unique_ptr<std::random_device> device;
	std::unique_ptr<std::mt19937> gen;
	std::uniform_int_distribution<int> dist(0, 255);

	constexpr auto scenes_variant = ct::scenes::parse_scenes<scene__>();

	if constexpr (scenes_variant.is_type_b()) {
		println(scenes_variant.type_b());
		return 0;
	}

	constexpr auto styles_variant = ct::styles::parse_styles<style__>();
	if constexpr (styles_variant.is_type_a()) {
		std::vector<ct::Style> sty;
		sty.reserve(styles_variant.type_a().size());
		for (const auto& el : styles_variant.type_a()) {
			const auto parsed_variant = ct::Style::create(el);
			if (parsed_variant.is_type_b()) {
				println(parsed_variant.type_b());
				return 0;
			}
			const auto& parsed = parsed_variant.type_a();
			sty.push_back(parsed);
		}

		window = std::make_unique<win_t>(std::move(sty), std::move(scenes_variant.type_a()));
	} else {
		println(styles_variant.type_b());
		return 0;
	}

	println("Before register_global_event()");
	window->register_global_event(sf::Event::EventType::Closed, "on_close", [&window](const auto& event_data) {
		window->close();
	});

	println("Before register_event()");
	window->register_event(sf::Event::EventType::MouseButtonPressed,
						   "on_click_btn",
						   [&window, &gen, &dist](const auto& event_data) {
							   const auto r = dist(*gen);
							   const auto g = dist(*gen);
							   const auto b = dist(*gen);
							   auto& graph = window->active_scene().graph();
							   Schematic& scheme = graph.root().active_schematic();
							   scheme.background() = cui::Color{r, g, b};
							   window->update_cache();
						   });

	println("Before attach_event_to_node()");
	window->attach_event_to_node("button", "on_click_btn");

	println("Creating the renderwindow...");
	window->init({800, 600, "Title", sf::Style::Default, sf::ContextSettings{}, 60});

	println(window->active_scene().graph());

	for (const auto& ve : window->cache_) {
		println(ve, "\n///////////////////////////////////////////////////\n");
	}

	println("Starting the app loop...");

	println("Window is running: ", window->window()->isOpen());

	println("After using the newly initialized window variable!");

	while (window->is_running()) {
		window->handle_events();
		window->render();
	}
}
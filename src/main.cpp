#include <compile_time/styles/detail/constants.hpp>
#include <compile_time/style.hpp>
#include <compile_time/scene.hpp>
#include <compile_time/value_data.hpp>
#include <compile_time/styles/parse_styles.hpp>
#include <compile_time/scenes/parse_scenes.hpp>
#include <compile_time/string/string_view.hpp>

#include <visual/scene_graph.hpp>
#include <visual/node.hpp>
#include <window.hpp>

#include <visual/base_render_context.hpp>
#include <engine/base_event_manager.hpp>

#include <aliases.hpp>
#include <iostream>
#include <type_traits>
#include <utility>

std::ostream& operator<<(std::ostream& os, const cui::ct::StringView str) {
	for (const auto ch : str) os << ch;
	return os;
}

template <cui::u32 Size>
std::ostream& operator<<(std::ostream& os, const cui::ct::Format<Size>& format) {
	os << format.data().data();
	return os;
}

template <typename... Args>
void print(Args... args) {
	((std::cout << args << ' '), ...);
}

template <typename... Args>
void println(Args... args) {
	((std::cout << args << ' '), ...);
	std::cout << '\n';
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
		default: {
			os << "Instruction";
			break;
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
	return os;
}

std::ostream& operator<<(std::ostream& os, const cui::SceneGraph& sg) {
	for (const auto& node : sg) {
		os << node.data().name() << "{\n\t";
		os << "Text: " << node.data().text() << "\n\t";
		os << "Index: " << node.index() << " | ";
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

#include <cui/window.hpp>
#include <render_context/render_context.hpp>
#include <render_context/window_options.hpp>
#include <render_context/render_cache.hpp>
#include <render_context/render_context.hpp>
#include <engine/event_manager.hpp>
#include <engine/test_event_manager.hpp>
#include <utility>

#define STATIC_STRING_HOLDER(name) static constexpr const char name[] =
#define END_STATIC_STRING_HOLDER ;

int main() {
	using namespace cui;
	using win_t = Window<TestRenderContext, TestEventManager, WindowOptions>;

	std::unique_ptr<win_t> window;

	STATIC_STRING_HOLDER(style__)
#include "file.styles"
	END_STATIC_STRING_HOLDER

	STATIC_STRING_HOLDER(scene__)
#include "file.scene"
	END_STATIC_STRING_HOLDER

	constexpr auto scenes_variant = ct::scenes::parse_scenes<scene__>();

	if constexpr (scenes_variant.is_type_b()) {
		println(scenes_variant.type_b());
		return 0;
	}

	constexpr auto styles_variant = ct::styles::parse_styles<style__>();
	if constexpr (styles_variant.is_type_a()) {
		StaticVector<ct::Style, styles_variant.type_a().size()> sty;
		for (const auto& el : styles_variant.type_a()) {
			const auto parsed_variant = ct::Style::create(el);
			if (parsed_variant.is_type_b()) {
				println(parsed_variant.type_b());
				return 0;
			}
			const auto& parsed = parsed_variant.type_a();
			sty.push_back(parsed);
		}

		println(SceneGraph{scenes_variant.type_a(), sty});

		window = std::make_unique<win_t>(sty, scenes_variant.type_a());
		println(window->current_scene().graph()[0].data().active_schematic().get().x());
	} else {
		println(styles_variant.type_b());
		return 0;
	}

	/*
	constexpr auto styles_variant = ct::styles::parse_styles<style__>();
		if constexpr (styles_variant.is_type_a()) {
			StaticVector<ct::Style, styles_variant.type_a().size()> sty;
			for (const auto& el : styles_variant.type_a()) {
				const auto parsed_variant = ct::Style::create(el);
				if (parsed_variant.is_type_b()) {
					println(parsed_variant.type_b());
					return 0;
				}
				const auto& parsed = parsed_variant.type_a();
				sty.push_back(parsed);
			}

			println(SceneGraph{scenes_variant.type_a(), sty});

			window = std::make_unique<win_t>(sty, scenes_variant.type_a());
			println(window->current_scene().graph()[0].data().active_schematic().get().x());
		} else {
			println(styles_variant.type_b());
			return 0;
		}
	*/

	// println("Creating the renderwindow...");
	// window->init({800, 600, "Title", sf::Style::Default, sf::ContextSettings{}});

	// println("Starting the app loop...");

	// println("Window is running: ", window->ctx_.window()->isOpen());

	// while (window->is_running()) {
	// 	window->handle_events();
	// 	window->render();
	// }
}
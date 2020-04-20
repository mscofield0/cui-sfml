
#include <any>
#include <iostream>
#include <print_stuff.hpp>
#include <random>
#include <string>
#include <type_traits>
#include <utility>
#include <visual_element.hpp>

#include <cui/compile_time/scene.hpp>
#include <cui/compile_time/scenes/parse_scenes.hpp>
#include <cui/compile_time/string/string_view.hpp>
#include <cui/compile_time/style.hpp>
#include <cui/compile_time/styles/detail/constants.hpp>
#include <cui/compile_time/styles/parse_styles.hpp>
#include <cui/compile_time/value_data.hpp>
#include <cui/utils/print.hpp>
#include <cui/visual/node.hpp>
#include <cui/visual/scene_graph.hpp>
#include <cursors.hpp>
#include <detail/intermediaries/color.hpp>
#include <detail/templates/on_click.hpp>
#include <detail/templates/on_hover.hpp>
#include <detail/templates/on_resize.hpp>
#include <detail/templates/switch_schematic.hpp>
#include <render_cache.hpp>
#include <window.hpp>
#include <window_options.hpp>

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

	// Typedefs for easy use
	using win_t = Window;
	using node_t = Node;
	using event_data_t = EventData<node_t>;

	std::unique_ptr<win_t> window;

	// Parse the .scene file
	constexpr auto scenes_variant = ct::scenes::parse_scenes<scene__>();
	// Handle if error
	if constexpr (scenes_variant.is_type_b()) {
		println(scenes_variant.type_b());
		return 0;
	}

	// Parse the .styles file
	constexpr auto styles_variant = ct::styles::parse_styles<style__>();
	// Handle if error
	if constexpr (styles_variant.is_type_b()) {
		println(styles_variant.type_b());
		return 0;
	}
	// Populate the Window with the parsed data
	else
	{
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

		// Create the Window
		window = std::make_unique<win_t>(std::move(sty), std::move(scenes_variant.type_a()));
	}

	// Create a typedef for the event types
	using EventType = sf::Event::EventType;

	// Initialize the static global sf::Cursor variables
	cursors::init_cursors();

	// Register the on_close event [OBLIGATORY, otherwise you cannot close via conventional methods]
	window->register_global_event(EventType::Closed, "on_close", [&window](auto event_data) { window->close(); });

	// Register the on_resize event [OPTIONAL, but a resize event is one of the events that should update the scene graph]
	window->register_global_event(EventType::Resized, "on_resize", [&window](auto event_data) { templates::OnResize((*window), event_data); });

	// Register the on_click_btn event [OPTIONAL, defines functionality on button click]
	window->register_event(EventType::MouseButtonPressed, "on_click_btn1", [&window](event_data_t event_data) {
		// Uses CUI's GUI helper template `OnClick` to provide functionality on click
		templates::OnClick((*window), event_data, [](Window& window, event_data_t& event_data) {
			println("Clicked on btn1");
			auto& graph = window.active_scene().graph();
			auto text_box = std::find_if(graph.begin(), graph.end(), [](const auto& node) { return node.data().name() == "text_box"; });
			println("Name of text_box:", text_box->data().name());
			const auto index = static_cast<std::size_t>(std::distance(graph.begin(), text_box));
			println("Index of text_box:", index);
			auto& scheme = text_box->data().active_schematic().get();
			scheme.text_color() = cui::Color(255, 0, 0);
			println("Text color:", scheme.text_color());
			window.cache()[index].text().setString("Red text");
			println("Text:", window.cache()[index].text().getString().toAnsiString());

			// Schedule the render cache to be updated
			window.schedule_to_update_cache();
		});
	});

	// Register the on_click_btn event [OPTIONAL, defines functionality on button click]
	window->register_event(EventType::MouseButtonPressed, "on_click_btn2", [&window](event_data_t event_data) {
		// Uses CUI's GUI helper template `OnClick` to provide functionality on click
		templates::OnClick((*window), event_data, [](Window& window, event_data_t& event_data) {
			auto& graph = window.active_scene().graph();
			auto text_box = std::find_if(graph.begin(), graph.end(), [](const auto& node) { return node.data().name() == "text_box"; });
			const auto index = static_cast<std::size_t>(std::distance(graph.begin(), text_box));
			auto& scheme = text_box->data().active_schematic().get();
			scheme.text_color() = cui::Color(0, 255, 0);
			window.cache()[index].text().setString("Green text");

			// Schedule the render cache to be updated
			window.schedule_to_update_cache();
		});
	});

	// Register the on_click_btn event [OPTIONAL, defines functionality on button click]
	window->register_event(EventType::MouseButtonPressed, "on_click_btn3", [&window](event_data_t event_data) {
		// Uses CUI's GUI helper template `OnClick` to provide functionality on click
		templates::OnClick((*window), event_data, [](Window& window, event_data_t& event_data) {
			auto& graph = window.active_scene().graph();
			auto text_box = std::find_if(graph.begin(), graph.end(), [](const auto& node) { return node.data().name() == "text_box"; });
			const auto index = static_cast<std::size_t>(std::distance(graph.begin(), text_box));
			auto& scheme = text_box->data().active_schematic().get();
			scheme.text_color() = cui::Color(0, 0, 255);

			window.cache()[index].text().setString("Blue text");

			// Schedule the render cache to be updated
			window.schedule_to_update_cache();
		});
	});

	// Register the on_hover event [OPTIONAL, defines functionality on hover]
	window->register_event(EventType::MouseMoved, "on_hover", [&window](event_data_t event_data) {
		// Uses CUI's GUI helper template `OnHover` to provide functionality on hover
		templates::OnHover((*window),
						   event_data,
						   event_data.caller()->name(),
						   [](Window& window, event_data_t& event_data) {
							   templates::SwitchToDefaultSchematic(window, event_data);
							   window.window()->setMouseCursor(cursors::Arrow);
						   },
						   [](Window& window, event_data_t& event_data) {
							   templates::SwitchToEventSchematic(window, event_data);
							   window.window()->setMouseCursor(cursors::Hand);
						   });
	});

	// Attaches the registered event `on_click_btn` to the node named `button`
	window->attach_event_to_node("button1", "on_click_btn1");
	//window->attach_event_to_node("button2", "on_click_btn2");
	//window->attach_event_to_node("button3", "on_click_btn3");
	// Attaches the registered event `on_hover` to the node named `button`
	window->attach_event_to_node("button1", "on_hover");
	//window->attach_event_to_node("button2", "on_hover");
	//window->attach_event_to_node("button3", "on_hover");

	// Initialize the window
	window->init({800, 600, "Title", sf::Style::Default, sf::ContextSettings{}, 60});
}
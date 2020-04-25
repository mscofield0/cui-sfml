#include <any>
#include <iostream>
#include <print_stuff.hpp>
#include <random>
#include <string>
#include <type_traits>
#include <utility>

#include <cui/compile_time/scene.hpp>
#include <cui/compile_time/scenes/parse_scenes.hpp>
#include <cui/compile_time/style.hpp>
#include <cui/compile_time/styles/parse_styles.hpp>
#include <cui/compile_time/value_data.hpp>
#include <cui/utils/print.hpp>
#include <cui/visual/node.hpp>
#include <cui/visual/scene_graph.hpp>
#include <cursors.hpp>
#include <detail/intermediaries/color.hpp>
#include <detail/templates/on_click.hpp>
#include <detail/templates/on_resize.hpp>
#include <detail/templates/switch_schematic.hpp>
#include <visual_element.hpp>
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
	static auto rand_device = std::make_unique<std::random_device>();
	static auto mt_rand = std::make_unique<std::mt19937>((*rand_device)());
	static std::uniform_int_distribution rand_255(0, 255);

	// Create a scope so it destructs at the end
	{
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
	}

	// Create a typedef for the event types
	using EventType = sf::Event::EventType;

	// Register the on_close event [OBLIGATORY, otherwise you cannot close via conventional methods]
	window->register_global_event(EventType::Closed, "on_close", [&window](auto event_data) { window->close(); });

	// Register the on_resize event [OPTIONAL, but a resize event is one of the events that should update the scene graph]
	window->register_global_event(EventType::Resized, "on_resize", [&window](auto event_data) { templates::OnResize((*window), event_data); });

	// Register the on_click_btn event [OPTIONAL, defines functionality on button click]
	window->register_event(EventType::MouseButtonPressed, "on_click_btn", [&window](event_data_t event_data) {
		// Uses CUI's GUI helper template `OnClick` to provide functionality on click
		templates::OnClick((*window), event_data, [](Window& window, event_data_t& event_data) {
			auto& graph = window.active_scene().graph();
			auto& root = graph.root();

			const auto r = rand_255(*mt_rand);
			const auto g = rand_255(*mt_rand);
			const auto b = rand_255(*mt_rand);

			root.active_schematic().get().background() = cui::Color(r, g, b);

			// Schedule the render cache to be updated
			window.schedule_to_update_cache();
		});
	});

	// Attaches the registered event `on_click_btn` to the button nodes
	window->attach_event_to_node("button", "on_click_btn");

	// Initialize the window
	window->init({800, 600, "Title", sf::Style::Default, sf::ContextSettings{}, 60});
}
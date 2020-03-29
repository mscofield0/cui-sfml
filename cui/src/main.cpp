
/*
#define CT_VALIDATE_RESULT(result) \
	if constexpr (result.active == decltype(result)::active_::T2_) { \
		throw std::system_error(); \
	}

#define VALIDATE_RESULT(result, err) \
	if (result.active == decltype(result)::active_::T2_) { \
		return cui::ct::error_report{err}; \
	}

template <uint32_t N> cui::ct::variant<std::array<cui::ct::styles_data, N>, cui::ct::error_report>
convert_to_styles_data(std::array<cui::ct::styles_definition, N> const& defs) {
	std::array<cui::ct::styles_data, N> data{};

	uint32_t i = 0;
	for (auto const& def : defs) {
		auto r = cui::ct::styles_data::create(def);
		VALIDATE_RESULT(r, "Could not create styles_data.");
		data[i++] = r.type1;
	}

	return data;
}
*/
/*

// #define CUI_ENABLE_CONSTEXPR_PARSING

#include <scenes/parser.hpp>
#include <styles/parser.hpp>
#include <type_traits>
#include <compile_time/string_view.hpp>

#include <global/array.hpp>
#include <global/tracked_array.hpp>

#include <visual/scene_graph.hpp>

template <typename T>
std::ostream& operator<<(std::ostream& os, const cui::NodeIterator<T>& node) {
	os << "Index: " << node.index() << " | ";
	os << "Depth: " << node.depth() << " | ";
	os << "Data: " << node.data();
	return os;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const cui::ConstNodeIterator<T>& node) {
	os << "Index: " << node.index() << " | ";
	os << "Depth: " << node.depth() << " | ";
	os << "Data: " << node.data();
	return os;
}

#include <iostream>
#include <compile_time/string_view.hpp>
#include <compile_time/string_utils/trim_sv.hpp>

std::ostream& operator<<(std::ostream& os, const cui::StringView& str) {
	for (auto it = str.begin(); it < str.end(); ++it) {
		os << *it;
	}
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

#ifdef CUI_ENABLE_CONSTEXPR_PARSING

template <size_t N>
std::ostream& operator<<(std::ostream& os, const cui::scenes::Scene<N>& val) {
	for (size_t i = 0; i < val.blocks().size(); ++i) {
		const auto& block = val.blocks()[i];
		os << "Block: {\n\tName: " << block.name() << "\n\tStyle list: {\n\t\t";
		if (!block.style_list().empty()) {
			for (size_t j = 0; j < block.style_list().size() - 1; ++j) {
				const auto& style = block.style_list()[j];
				std::cerr << '\n' << "Style.back() = " << style.back() << '\n';
				if (style.back() == '"') {
					std::cerr << "There is, indeed, an apostrophy";
				}
				os << style << ",\n\t\t";
			}
			os << block.style_list().back();
		}
		os << "\n\t}\n\tChildren: {\n\t\t";
		if (!val.children()[i].empty()) {
			for (size_t j = 0; j < val.children()[i].size() - 1; ++j) {
				const auto& child = val.blocks()[val.children()[i][j]];
				os << child.name() << ",\n\t\t";
			}
			os << val.blocks()[val.children()[i].back()].name();
		}
		os << "\n\t}\n\tDepth: " << val.depths()[i] << "\n}\n--------------------------------------------------\n";
	}

	return os;
}

#else

std::ostream& operator<<(std::ostream& os, const cui::scenes::Scene& val) {
	for (size_t i = 0; i < val.blocks().size(); ++i) {
		const auto& block = val.blocks()[i];
		os << "Block: {\n\tName: " << block.name() << "\n\tStyle list: {\n\t\t";
		if (!block.style_list().empty()) {
			for (size_t j = 0; j < block.style_list().size() - 1; ++j) {
				const auto& style = block.style_list()[j];
				os << style << ",\n\t\t";
			}
			os << block.style_list().back();
		}
		os << "\n\t}\n\tChildren: {\n\t\t";
		if (!val.children()[i].empty()) {
			for (size_t j = 0; j < val.children()[i].size() - 1; ++j) {
				const auto& child = val.children()[i][j];
				os << child << ",\n\t\t";
			}
			os << val.children()[i].back();
		}
		os << "\n\t}\n\tDepth: " << val.depths()[i] << "\n}\n--------------------------------------------------\n";
	}

	return os;
}

#endif

std::ostream& operator<<(std::ostream& os, const cui::styles::Definition& val) {
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

#include <styles/style.hpp>
#include <styles/data.hpp>

std::ostream& operator<<(std::ostream& os, const cui::styles::ValueData& val) {
	switch (val.active()) {
		case cui::styles::DataTypes::None: {
			os << "Nothing";
			return os;
		}
		case cui::styles::DataTypes::Color: {
			os << "rgba(" << val.rgba().red() << "," << val.rgba().green() << "," << val.rgba().blue() << ','
			   << val.rgba().alpha() << ')';
			return os;
		}
		case cui::styles::DataTypes::Float: {
			os << val.float_value();
			return os;
		}
		case cui::styles::DataTypes::Int: {
			os << val.integer_value();
			return os;
		}
		case cui::styles::DataTypes::Vector2: {
			os << "vec(";
			for (size_t i = 0; i < 1; ++i) {
				os << val.vector2()[i] << ',';
			}
			os << val.vector2().back() << ')';
			return os;
		}
		case cui::styles::DataTypes::Vector3: {
			os << "vec(";
			for (size_t i = 0; i < 2; ++i) {
				os << val.vector3()[i] << ',';
			}
			os << val.vector3().back() << ')';
			return os;
		}
		case cui::styles::DataTypes::Vector4: {
			os << "vec(";
			for (size_t i = 0; i < 3; ++i) {
				os << val.vector4()[i] << ',';
			}
			os << val.vector4().back() << ')';
			return os;
		}
		default: {
			os << "Instruction";
			return os;
		}
	}
}

std::ostream& operator<<(std::ostream& os, const cui::styles::Style& style) {
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

std::ostream& operator<<(std::ostream& os, const cui::SceneGraph& sg) {
	for (const auto& node : sg) {
		os << "Index: " << node.index() << " | ";
		os << "Depth: " << node.depth();
	}
}

#include <parse_utils/stream.hpp>
#include <styles/detail/parse_attribute_value.hpp>

#include <global/make_array.hpp>

#include <visual/window.hpp>
#include <visual/render_context.hpp>
#include <engine/event_manager.hpp>
#include <utility>
*/

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
		case cui::ct::data_types::DataTypes::None: {
			os << "Nothing";
			break;
		}
		case cui::ct::data_types::DataTypes::Color: {
			os << "rgba(" << val.rgba().red() << "," << val.rgba().green() << "," << val.rgba().blue() << ','
			   << val.rgba().alpha() << ')';
			break;
		}
		case cui::ct::data_types::DataTypes::Float: {
			os << val.float_value();
			break;
		}
		case cui::ct::data_types::DataTypes::Int: {
			os << val.integer_value();
			break;
		}
		case cui::ct::data_types::DataTypes::Vec2: {
			os << "vec(";
			for (size_t i = 0; i < 1; ++i) {
				os << val.vec2()[i] << ',';
			}
			os << val.vec2().back() << ')';
			break;
		}
		case cui::ct::data_types::DataTypes::Vec3: {
			os << "vec(";
			for (size_t i = 0; i < 2; ++i) {
				os << val.vec3()[i] << ',';
			}
			os << val.vec3().back() << ')';
			break;
		}
		case cui::ct::data_types::DataTypes::Vec4: {
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
		os << node.data().default_schematic();
		os << "\n}";
	}
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

	using win_t = Window<BaseRenderContext, engine::BaseEventManager>;
	std::unique_ptr<win_t> win;

	constexpr auto scenes_variant = ct::scenes::parse_scenes<scene__>();

	if constexpr (scenes_variant.is_type_b()) {
		println(scenes_variant.type_b());
		return 0;
	} else {
		constexpr auto styles_variant = ct::styles::parse_styles<style__>();
		if constexpr (styles_variant.is_type_b()) {
			println(styles_variant.type_b());
			return 0;
		} else {
			constexpr auto style_vec = styles_variant.type_a();
			StaticVector<ct::Style, style_vec.size()> sty;
			for (const auto& el : style_vec) {
				const auto parsed_variant = ct::Style::create(el);
				if (parsed_variant.is_type_b()) {
					println(parsed_variant.type_b());
					return 0;
				}
				const auto& parsed = parsed_variant.type_a();
				sty.push_back(parsed);
			}
			win = std::make_unique<win_t>(sty, scenes_variant.type_a());
		}
	}

	println(ct::fmt("Number of SceneGraphs: {}", win->scenes().size()));
	println(win->current_scene());

	/*
	using namespace cui;

	STATIC_STRING_HOLDER(style__)
#include "file.styles"
	END_STATIC_STRING_HOLDER

	STATIC_STRING_HOLDER(scene__)
#include "file.scene"
	END_STATIC_STRING_HOLDER

	using win = cui::Window<BaseRenderContext, engine::BaseEventManager>;

	std::unique_ptr<win> window;

#ifdef CUI_ENABLE_CONSTEXPR_PARSING
	{
		// Scenes parsing
		constexpr auto scenes_variant = scenes::parse<scene__>();

		if constexpr (scenes_variant.is_type_a()) {
			println("It's type a");
			println(scenes_variant.type_a());
		} else if constexpr (scenes_variant.is_type_b()) {
			println("It's type b");
			println(scenes_variant.type_b().data().data());
			return 0;
		}

		// Styles parsing

		constexpr auto style_variant = styles::parse<style__>();

		if constexpr (style_variant.is_type_a()) {
			println("It's type a");
			for (const auto& def : style_variant.type_a()) {
				println(def, '\n');
			}
		} else if constexpr (style_variant.is_type_b()) {
			println("It's type b");
			println(style_variant.type_b().data().data());
			return 0;
		}

		cui::static_vector<styles::Style, style_variant.type_a().size()> sty;
		for (const auto& def : style_variant.type_a()) {
			auto t_style_variant = styles::Style::create(def);
			if (t_style_variant.is_type_b()) {
				println(t_style_variant.type_b().data().data());
				return 0;
			}

			sty.push_back(t_style_variant.type_a());
		}

		for (const auto& el : sty) {
			println(el);
		}

		window = std::make_unique<win>(sty, scenes_variant.type_a());
	}

#else

	{
		// Scenes parsing
		auto scenes_variant = scenes::parse(scene__);

		if (scenes_variant.is_type_a()) {
			println("It's type a");
			println(scenes_variant.type_a());
		} else if (scenes_variant.is_type_b()) {
			println("It's type b");
			println(scenes_variant.type_b().data().data());
			return 0;
		}

		// Styles parsing

		auto style_variant = styles::parse(style__);

		if (style_variant.is_type_a()) {
			println("It's type a");
			for (const auto& def : style_variant.type_a()) {
				println(def, '\n');
			}
		} else if (style_variant.is_type_b()) {
			println("It's type b");
			println(style_variant.type_b().data().data());
			return 0;
		}

		std::vector<styles::Style> sty;
		sty.reserve(style_variant.type_a().size());
		for (const auto& def : style_variant.type_a()) {
			auto t_style_variant = styles::Style::create(def);
			if (t_style_variant.is_type_b()) {
				println(t_style_variant.type_b().data().data());
				return 0;
			}

			sty.push_back(t_style_variant.type_a());
		}

		for (const auto& el : sty) {
			println(el);
		}

		window = std::make_unique<win>(cui::make_array(scenes_variant.type_a()), sty);
	}

#endif
*/

	/*
	STATIC_STRING_HOLDER(style__)
#include "file.styles"
	END_STATIC_STRING_HOLDER

	STATIC_STRING_HOLDER(scene__)
#include "file.scene"
	END_STATIC_STRING_HOLDER

	constexpr auto scene_result = cui::ct::parse_scene<scene__>();
	CT_VALIDATE_RESULT(scene_result);
	constexpr auto style_result = cui::ct::parse_styles<style__, cui::ct::get_amount_of_definitions<style__>()>();
	CT_VALIDATE_RESULT(style_result);

	auto sd_temp = convert_to_styles_data(style_result.type1);
	if(sd_temp.active == sd_temp.active_::T2_) {
		throw std::logic_error{""};
	}
	auto sd = sd_temp.type1;

	auto graph =
	  cui::scene_graph<sf::RectangleShape, cui::sf_mapper<sf::RectangleShape>>::create(scene_result.type1, sd);

	cui::println("Nodes: ");
	for (auto& node : graph.nodes) {
		cui::println(node.block_name);

		// node.drawable.setSize(sf::Vector2f{200.f, 200.f});
	}

	cui::println("\nDepths: ");

	for (auto const& depth : graph.depths) {
		cui::println(depth);
	}

	cui::println("\nChildren: ");
	auto c = 0;
	for (auto const& childarr : graph.children) {
		cui::println(graph.nodes[c].block_name);
		for (auto const& child : childarr) {
			cui::println(child);
		}
		++c;
	}

	auto color = graph.nodes[0].drawable.getFillColor();
	cui::println("Color:", static_cast<int>(color.r), static_cast<int>(color.g), static_cast<int>(color.b));

	auto win = sf::RenderWindow{sf::VideoMode{800, 600}, "Title"};

	// auto reng = std::make_unique<std::random_device>();
	// auto mt = std::make_unique<std::mt19937>((*reng)());
	// auto gen = std::make_unique<std::uniform_int_distribution<int>>(0, 600);

	//auto before = std::chrono::steady_clock::now();
	sf::Event evt;
	while (win.isOpen()) {
		while (win.pollEvent(evt)) {
			switch (evt.type) {
				case sf::Event::EventType::Closed: {
					win.close();
				}
			}
		}

		// auto after = std::chrono::steady_clock::now();

		win.clear();
		//auto timelapse = std::chrono::duration_cast<std::chrono::milliseconds>(after - before).count();
		//win.setTitle(std::to_string(timelapse));

		win.draw(graph.root.drawable);
		for (auto const& node : graph.nodes) {
			win.draw(node.drawable);
		}

		win.display();
	}

	cui::println("Done.");

	return 0;
	*/
}
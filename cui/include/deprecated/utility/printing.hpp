#ifndef CUI_PRINTING_HPP
#define CUI_PRINTING_HPP

#include <iostream>
#include "../ct/styles.hpp"
#include "../ct/scene.hpp"
#include "../ct/styles_data.hpp"

#ifndef BARK
#define BARK std::cout << __PRETTY_FUNCTION__ << std::endl
#endif

#ifndef PRINT_TYPE
#define PRINT_TYPE(var) cui::print_type<decltype(var)>()
#endif

namespace cui {

template <typename T>
inline constexpr void print_type() {
	BARK;
}

/// @brief Prints multiple arguments to std::cout with spaces in between
template <typename... Args>
inline void print(Args... args) {
	((std::cout << args << ' '), ...);
}

/// @brief Prints multiple arguments to std::cout with spaces
/// in between recursively with parameter packs and followed by a newline
template <typename... Args>
inline void println(Args... args) {
	((std::cout << args << ' '), ...);
	std::cout << '\n';
}

std::ostream& operator<<(std::ostream& stream, cui::ct::string const& str) {
	if (str.size == 0) return stream;
	for (uint32_t i = 0; i < str.size; ++i) {
		stream << str[i];
	}
	return stream;
}

template <uint32_t N>
std::ostream& operator<<(std::ostream& stream, cui::ct::string_with_size<N> const& str) {
	for (uint32_t i = 0; i < str.size && str[i] != 0; ++i) {
		stream << str[i];
	}
	return stream;
}

std::ostream& operator<<(std::ostream& stream, cui::ct::styles_definition const& def) {
	stream << "Style name: " << def.styles_identifier << "\nEvents {\n";
	for (uint32_t i = 0; i < def.event_idx; ++i) {
		stream << "Name: " << def.events[i];
		if (i != def.event_idx - 1) stream << ',';
		stream << '\n';
	}
	stream << "}\nAttributes {\n";
	for (uint32_t i = 0; i < def.attr_idx; ++i) {
		stream << "Name: " << def.attributes[i].type << " | Value: ";
		stream << def.attributes[i].value;

		if (i != def.attr_idx - 1) stream << ',';
		stream << '\n';
	}
	stream << "}";
	return stream;
}

std::ostream& operator<<(std::ostream& stream, cui::ct::styles_data const& data) {
	stream << "Style name: " << data.name << "\nEvents {\n";
	for (uint32_t i = 0; i < data.events_size; ++i) {
		stream << "Name: " << data.events[i];
		if (i != data.events_size - 1) stream << ',';
		stream << '\n';
	}
	stream << "}\nAttributes {\n";
	for (uint32_t i = 0; i < data.attributes_size; ++i) {
		stream << "Name: " << data.attributes[i].type << " | Value: ";
		switch (data.attributes[i].value.active) {
			case cui::ct::styles_value_data::active_::NONE: {
				stream << "Nothing";
				break;
			}
			case cui::ct::styles_value_data::active_::INT: {
				stream << data.attributes[i].value.val_i;
				break;
			}
			case cui::ct::styles_value_data::active_::FLOAT: {
				stream << "Nothing";
				break;
			}
			case cui::ct::styles_value_data::active_::COLOR_RGBA: {
				auto const& color = data.attributes[i].value.rgba;
				stream << "RGB(" << color.r() << "," << color.g() << "," << color.b() << "," << color.a() << ")";
				break;
			}
			default: {
				stream << "Fatal error";
				return stream;
			}
		}

		if (i != data.attributes_size - 1) stream << ',';
		stream << '\n';
	}
	stream << "}";
	return stream;
}

std::ostream& operator<<(std::ostream& stream, cui::ct::error_report const& err) {
	stream << err.what();
	return stream;
}

std::ostream& operator<<(std::ostream& stream, cui::ct::block const& block) {
	stream << "Block name: " << block.block_name << "\nStyle list: {\n";
	if (block.idx == 0) {
		return stream << "}\n";
	}
	for (int i = 0; i < block.idx - 1; ++i) {
		if (block.style_list[i].size == 0) return stream;
		stream << block.style_list[i] << ",\n";
	}
	stream << block.style_list[block.idx - 1] << "\n}";
	return stream;
}
template <uint32_t S1>
std::ostream& operator<<(std::ostream& stream, cui::ct::scene_result<S1> const& pair) {
	auto const& f = pair.blocks;
	auto const& s = pair.children;
	stream << "Scene elements:\n";
	for (int i = 0; i < S1; ++i) {
		stream << f[i] << "\n------------------\n";
	}
	stream << "Scene graph:\n";
	for (int i = 0; i < S1; ++i) {
		stream << f[i].block_name << "{\n";
		for (int j = 0; j < pair.child_idx[i]; ++j) {
			stream << f[s[i][j]].block_name << ",\n";
		}
		stream << "}\n";
	}
	return stream;
}
}	 // namespace cui

#endif	  //CUI_PRINTING_HPP

#ifndef DRAW_INSTRUCTION_HPP
#define DRAW_INSTRUCTION_HPP

#include "../ct/styles_data.hpp"
#include "../ct/string.hpp"
#include "./scene_graph.hpp"

#include <vector>
#include <string>

namespace cui {

struct attribute
{
	std::string type;
	cui::ct::styles_value_data value;

	attribute() : type(), value() {}
	attribute(std::string type, cui::ct::styles_value_data value) : type(type), value(value) {}
	attribute(cui::ct::string type, cui::ct::styles_value_data value) : value(value) {
		cui::from_ct_string(this->type, type);
	}
	attribute(cui::ct::attribute_data const& attr) : value(attr.value) {
		cui::from_ct_string(type, attr.type);
	}
};

template <typename Drawable, typename Mapper>
struct scene_graph;

template <typename Mapper>
struct draw_instruction
{
	std::vector<attribute> attributes;

	template <typename Drawable>
	void operator()(Drawable& drawable, cui::scene_graph<Drawable, Mapper> const* graph) {
		for (auto const& attr : attributes) {
			Mapper::apply_attribute(drawable, attr, graph);
		}
	}
};

}	 // namespace cui

#endif	  // DRAW_INSTRUCTION_HPP
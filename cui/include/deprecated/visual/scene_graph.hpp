#ifndef CUI_SCENE_GRAPH_HPP
#define CUI_SCENE_GRAPH_HPP

#include "../ct/styles_data.hpp"
#include "../ct/styles.hpp"
#include "../ct/string.hpp"
#include "../ct/scene.hpp"
#include "../utility/utility.hpp"
#include "./draw_instruction.hpp"
#include "../utility/diag.hpp"

#include <vector>
#include <array>
#include "../tsl/hopscotch_map.h"

namespace cui {

template <typename Drawable, typename Mapper>
struct scene_graph;

template <typename Drawable, typename Mapper>
struct node
{
	std::string block_name;
	cui::draw_instruction<Mapper> default_style;
	tsl::hopscotch_map<std::string, cui::draw_instruction<Mapper>> event_styles;

	Drawable drawable;
	cui::scene_graph<Drawable, Mapper>* graph;
	// Parse through styles list
	// Store styles_definitions
	// Create a sequence to apply the attributes
	// Store the attributes

	template <uint32_t N>
	static node create(cui::ct::block const& block,
					   std::array<cui::ct::styles_data, N> const& styles_data_arr,
					   cui::scene_graph<Drawable, Mapper>& graph) {
		// Create node
		node<Drawable, Mapper> node;

		// Assign the graph
		node.graph = &graph;

		// Assign the block name
		cui::from_ct_string(node.block_name, block.block_name);

		// Iterate through the block style list
		for (int i = 0; i < block.idx; ++i) {
			// Iterate through the styles data
			for (int j = 0; j < N; ++j) {
				// Check if the names are equal
				if (styles_data_arr[j].name == block.style_list[i]) {
					// If they are and the events_size is 0, then there are no events and
					// the style is a default one
					if (styles_data_arr[j].events_size == 0) {
						cui::draw_instruction<Mapper> instr;
						for (int k = 0; k < styles_data_arr[j].attributes_size; ++k) {
							instr.attributes.push_back(styles_data_arr[j].attributes[k]);
						}
						node.default_style = instr;

						continue;
					}

					// Populate the draw instruction with attributes
					cui::draw_instruction<Mapper> instr;
					for (int k = 0; k < styles_data_arr[j].attributes_size; ++k) {
						instr.attributes.push_back(styles_data_arr[j].attributes[k]);
					}

					// Iterate through the current styles_data's events and add them to event_styles map
					for (int k = 0; k < styles_data_arr[j].events_size; ++k) {
						std::string temp;
						cui::from_ct_string(temp, styles_data_arr[j].events[k]);
						node.event_styles[temp] = instr;
					}
				}
			}
		}

		// Assign the default style to the drawable
		node.default_style(node.drawable, node.graph);

		return node;
	}
};

template <typename Drawable, typename Mapper>
struct scene_graph
{
private:
	scene_graph() : nodes{}, depths{}, children{}, current_tracked_node(0) {}

	int partition(int l, int h) {
		const int pivot = depths[h];
		int i = l - 1;

		for (int j = l; j < h; ++j) {
			if (depths[j] <= pivot) {
				++i;

				swap_node_details(i, j);
			}
		}

		swap_node_details(i + 1, h);
		return i + 1;
	}

public:
	node<Drawable, Mapper> root;
	std::vector<node<Drawable, Mapper>> nodes;
	std::vector<uint32_t> depths;
	std::vector<std::vector<uint32_t>> children;

	uint32_t current_tracked_node;

	void swap_node_details(int a_idx, int b_idx) {
		for (int l = 0; l < children.size(); ++l) {
			for (int k = 0; k < children[l].size(); ++k) {
				if (children[l][k] == a_idx) {
					children[l][k] = b_idx;
					break;
				} else if (children[l][k] == b_idx) {
					children[l][k] = a_idx;
					break;
				}
			}
		}

		children[a_idx].swap(children[b_idx]);

		{
			auto const t_node = nodes[a_idx];
			nodes[a_idx] = nodes[b_idx];
			nodes[b_idx] = t_node;
		}

		{
			auto const t_depth = depths[a_idx];
			depths[a_idx] = depths[b_idx];
			depths[b_idx] = t_depth;
		}
	}

	void sort_graph() {
		int l = 0, h = depths.size() - 1;
		int stack[h - l + 1];

		int top = 1;

		stack[0] = l;
		stack[1] = h;

		while (top >= 0) {
			h = stack[top--];
			l = stack[top--];

			int p = partition(l, h);

			if (p - 1 > l) {
				stack[++top] = l;
				stack[++top] = p - 1;
			}

			if (p + 1 < h) {
				stack[++top] = p + 1;
				stack[++top] = h;
			}
		}
	}

	uint32_t find_parent(uint32_t node_idx) const {
		uint32_t idx = 0;
		for (auto const& el : children) {
			for (auto const& child_idx : el) {
				if (child_idx == node_idx) return idx;
			}
			++idx;
		}
		return idx;
	}

	template <uint32_t AOB, uint32_t N>
	static scene_graph create(cui::ct::scene_result<AOB> const& sr, std::array<cui::ct::styles_data, N> const& sd) {
		scene_graph<Drawable, Mapper> sg;
		using node_t = node<Drawable, Mapper>;

		sg.nodes.reserve(AOB);
		for (auto const& block : sr.blocks) {
			sg.nodes.push_back(node_t::create(block, sd, sg));
		}

		sg.depths.reserve(AOB);
		for (auto const& depth : sr.depths) {
			sg.depths.push_back(depth);
		}

		sg.children.reserve(AOB);
		for (int i = 0; i < AOB; ++i) {
			auto const& children = sr.children[i];
			sg.children.push_back(std::vector<uint32_t>{});
			auto& t = sg.children[sg.children.size() - 1];
			for (auto const& item : children) {
				if (item == -1) break;
				t.push_back(item);
			}
		}

		sg.sort_graph();

		return sg;
	}
};

}	 // namespace cui

#endif	  // CUI_SCENE_GRAPH_HPP
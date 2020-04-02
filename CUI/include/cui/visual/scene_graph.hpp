#ifndef CUI_SCENE_GRAPH_HPP
#define CUI_SCENE_GRAPH_HPP

#include <containers/nary_tree.hpp>
#include <compile_time/scene.hpp>
#include <compile_time/style.hpp>
#include <visual/node.hpp>
#include <aliases.hpp>

namespace cui {

class SceneGraph : public NaryTree<Node>
{
public:
	using tree_t = NaryTree<Node>;
	using value_type = typename tree_t::value_type;
	using size_type = typename tree_t::size_type;

	SceneGraph() : tree_t() {}

	template <u64 AOB, template <typename, u64> typename Container, u64 N>
	SceneGraph(const ct::Scene<AOB>& sr, const Container<ct::Style, N>& sc);

	void add_node(const_reference val);

	void add_node(value_type&& val);

	void add_node(const_reference val, size_type idx);

	void add_node(value_type&& val, size_type idx);

	void sort();

private:
	void swap_node_details(size_type a_idx, size_type b_idx);
	auto partition(int l, int h) -> int;

private:
	value_type root_;
};

template <u64 AOB, template <typename, u64> typename Container, u64 N>
SceneGraph::SceneGraph(const ct::Scene<AOB>& sr, const Container<ct::Style, N>& sc) : tree_t{} {
	this->nodes().reserve(AOB);
	this->depths().reserve(AOB);
	this->children().reserve(AOB);

	for (const auto& block : sr.blocks()) {
		this->nodes().emplace_back(block.name(), block.text());
		auto& node = this->nodes().back();
		for (const auto style_name : block.style_list()) {
			bool style_name_exists = false;
			for (const auto& t_style : sc) {
				if (style_name != t_style.name()) continue;
				if (style_name_exists == false) style_name_exists = true;
				if (t_style.events().empty()) {
					for (const auto& attr_data : t_style.attributes()) {
						node.default_schematic().assign(attr_data);
					}
					continue;
				}
				auto& map_el = node.event_schematics()[t_style.events().front().data()];
				for (const auto& attr_data : t_style.attributes()) {
					map_el.assign(attr_data);
				}
				for (auto it = t_style.events().begin() + 1; it != t_style.events().end(); ++it) {
					node.event_schematics()[it->data()] = map_el;
				}
			}
			// Maybe add a global logging buffer/system?
		}
	}

	for (const auto& depth : sr.depths()) {
		this->depths().push_back(depth);
	}

	for (const auto& c : sr.children()) {
		this->children().emplace_back();

		for (const auto& item : c) {
			this->children().back().push_back(item);
		}
	}

	sort();
}

void SceneGraph::sort() {
	int l = 0;
	int h = this->length() - 1;

	int* stack = new int[h - l + 1];
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
	delete[] stack;
}

void SceneGraph::add_node(const_reference val) {
	tree_t::add_node(val);
}

void SceneGraph::add_node(value_type&& val) {
	tree_t::add_node(std::move(val));
}

void SceneGraph::add_node(const_reference val, size_type idx) {
	tree_t::add_node(val, idx);
}

void SceneGraph::add_node(value_type&& val, size_type idx) {
	tree_t::add_node(std::move(val), idx);
}

void SceneGraph::swap_node_details(size_type a_idx, size_type b_idx) {
	for (auto& v1 : this->children()) {
		for (auto& el : v1) {
			if (el == a_idx) {
				el = b_idx;
				break;
			} else if (el == b_idx) {
				el = a_idx;
				break;
			}
		}
	}

	this->children()[a_idx].swap(this->children()[b_idx]);

	{
		const auto t_node = this->nodes()[a_idx];
		this->nodes()[a_idx] = this->nodes()[b_idx];
		this->nodes()[b_idx] = t_node;
	}

	{
		const auto t_depth = this->depths()[a_idx];
		this->depths()[a_idx] = this->depths()[b_idx];
		this->depths()[b_idx] = t_depth;
	}
}

auto SceneGraph::partition(int l, int h) -> int {
	const auto pivot = this->depths()[h];
	int i = l - 1;

	for (int j = l; j < h; ++j) {
		if (this->depths()[j] <= pivot) {
			++i;

			swap_node_details(i, j);
		}
	}

	swap_node_details(i + 1, h);
	return i + 1;
}

}	 // namespace cui

#endif	  // CUI_VISUAL_SCENE_GRAPH_HPP
#ifndef CUI_VISUAL_SCENE_GRAPH_HPP
#define CUI_VISUAL_SCENE_GRAPH_HPP

#include <global/nary_tree.hpp>
#include <scenes/scene.hpp>
#include <styles/style.hpp>
#include <styles/style_collection.hpp>

#include <visual/metadata/node.hpp>

namespace cui {

class SceneGraph : public NaryTree<metadata::Node>
{
public:
	using tree_t = NaryTree<metadata::Node>;
	using value_type = typename tree_t::value_type;
	using size_type = typename tree_t::size_type;

	SceneGraph() : tree_t() {}

#ifdef CUI_ENABLE_CONSTEXPR_PARSING
	template <size_type AOB, template <typename, size_type> typename Container, size_type N>
	SceneGraph(const scenes::Scene<AOB>& sr, const Container<styles::Style, N>& sc) : tree_t{} {
		this->nodes().reserve(AOB);
		this->depths().reserve(AOB);
		this->children().reserve(AOB);
		for (const auto& block : sr.blocks()) {
			this->nodes().emplace_back(block.name());
			auto& node = this->nodes().back().node();
			for (const auto style_name : block.style_list()) {
				for (const auto& t_style : sc) {
					if (style_name != t_style.name()) continue;
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

#else
	SceneGraph(const scenes::Scene& sr, const styles::StyleCollection& sc) : tree_t{} {
		const auto len = sr.blocks().size();
		this->nodes().reserve(len);
		this->depths().reserve(len);
		this->children().reserve(len);
		for (const auto& block : sr.blocks()) {
			this->nodes().emplace_back(block.name());
			auto& node = this->nodes().back().node();
			for (const auto style_name : block.style_list()) {
				for (const auto& t_style : sc) {
					if (style_name != t_style.name()) continue;
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
#endif

	void add_node(const_reference val) {
		tree_t::add_node(val);
		sort();
	}

	void add_node(value_type&& val) {
		tree_t::add_node(std::move(val));
		sort();
	}

	void add_node(const_reference val, size_type idx) {
		tree_t::add_node(val, idx);
		sort();
	}

	void add_node(value_type&& val, size_type idx) {
		tree_t::add_node(std::move(val), idx);
		sort();
	}

	void sort() {
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

private:
	void swap_node_details(size_type a_idx, size_type b_idx) {
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

	int partition(int l, int h) {
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
};

}	 // namespace cui

#endif	  // CUI_VISUAL_SCENE_GRAPH_HPP
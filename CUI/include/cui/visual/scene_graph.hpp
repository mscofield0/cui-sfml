#ifndef CUI_SCENE_GRAPH_HPP
#define CUI_SCENE_GRAPH_HPP

#include <algorithm>
#include <optional>
#include <iterator>

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
	static constexpr u64 root_index = -1;

	SceneGraph() : tree_t() {}

	template <u64 AOB, template <typename, u64> typename Container, u64 N>
	SceneGraph(const ct::Scene<AOB>& sr, const Container<ct::Style, N>& sc);

	template <u64 AOB, template <typename> typename Container>
	SceneGraph(const ct::Scene<AOB>& sr, const Container<ct::Style>& sc);

	[[nodiscard]] auto get_parent_index(size_type index) const noexcept -> size_type;

	[[nodiscard]] auto get_parent(size_type index) const noexcept -> std::optional<const_iterator>;

	void add_node(const_reference val);

	void add_node(value_type&& val);

	void add_node(const_reference val, size_type idx);

	void add_node(value_type&& val, size_type idx);

	void sort();

	void apply_style(value_type& node, const ct::Style& style);

	[[nodiscard]] auto root() noexcept -> value_type& {
		return root_;
	}

	[[nodiscard]] auto root() const noexcept -> const value_type& {
		return root_;
	}

private:
	void swap_node_details(size_type a_idx, size_type b_idx);
	auto partition(int l, int h) -> int;

	value_type root_;
};

template <u64 AOB, template <typename, u64> typename Container, u64 N>
SceneGraph::SceneGraph(const ct::Scene<AOB>& sr, const Container<ct::Style, N>& sc) : tree_t{}, root_() {
	this->nodes().reserve(AOB);
	this->depths().reserve(AOB);
	this->children().reserve(AOB);

	for (const auto& block : sr.blocks()) {
		this->nodes().emplace_back(block.name(), block.text());
		auto& node = this->nodes().back();
		for (const auto style_name : block.style_list()) {
			bool style_name_exists = false;
			for (const auto& t_style : sc) {
				if (style_name != t_style.name() || t_style.name().compare("root") == 0) continue;
				if (style_name_exists == false) style_name_exists = true;
				if (t_style.events().empty()) {
					for (const auto& attr_data : t_style.attributes()) {
						node.default_schematic().assign(attr_data);
					}
					continue;
				}

				auto sv = t_style.events().front();
				auto& map_el = node.event_schematics()[std::string{sv.begin(), sv.end()}];
				for (const auto& attr_data : t_style.attributes()) {
					map_el.assign(attr_data);
				}
				for (auto it = t_style.events().begin() + 1; it != t_style.events().end(); ++it) {
					node.event_schematics()[std::string{it->begin(), it->end()}] = map_el;
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

template <u64 AOB, template <typename> typename Container>
SceneGraph::SceneGraph(const ct::Scene<AOB>& sr, const Container<ct::Style>& sc) {
	this->nodes().reserve(AOB);
	this->depths().reserve(AOB);
	this->children().reserve(AOB);

	for (const auto& block : sr.blocks()) {
		this->nodes().emplace_back(block.name(), block.text());
		auto& node = this->nodes().back();
		for (const auto style_name : block.style_list()) {
			bool style_name_exists = false;
			for (const auto& t_style : sc) {
				if (t_style.name().compare("root") == 0) {
					apply_style(root_, t_style);
				}
				if (style_name != t_style.name()) continue;
				if (style_name_exists == false) style_name_exists = true;
				apply_style(node, t_style);
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

auto SceneGraph::get_parent_index(const size_type index) const noexcept -> size_type {
	if (index == root_index) return length();
	const auto it = std::find_if(children().begin(), children().end(), [index](const auto& v) {
		return std::find(v.begin(), v.end(), index) != v.end();
	});

	return std::distance(children().begin(), it);
}

auto SceneGraph::get_parent(const size_type index) const noexcept -> std::optional<const_iterator> {
	const auto idx = get_parent_index(index);
	if (idx == length()) return std::nullopt;
	return tree_t::operator[](idx);
}

void SceneGraph::apply_style(value_type& node, const ct::Style& style) {
	if (style.events().empty()) {
		for (const auto& attr_data : style.attributes()) {
			node.default_schematic().assign(attr_data);
		}
		return;
	}

	auto sv = style.events().front();
	auto& map_el = node.event_schematics()[std::string{sv.begin(), sv.end()}];
	for (const auto& attr_data : style.attributes()) {
		map_el.assign(attr_data);
	}
	for (auto it = style.events().begin() + 1; it != style.events().end(); ++it) {
		node.event_schematics()[std::string{it->begin(), it->end()}] = map_el;
	}
}

}	 // namespace cui

#endif	  // CUI_VISUAL_SCENE_GRAPH_HPP
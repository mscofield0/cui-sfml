#ifndef CUI_SCENE_GRAPH_HPP
#define CUI_SCENE_GRAPH_HPP

#include <utils/print.hpp>

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
	using data_type = typename tree_t::data_type;
	using size_type = typename tree_t::size_type;
	static constexpr u64 root_index = -1;

	SceneGraph() : tree_t() {}

	template <u64 AOB, template <typename, u64> typename Container, u64 N>
	SceneGraph(const ct::Scene<AOB>& sr, const Container<ct::Style, N>& sc);

	template <u64 AOB, template <typename> typename Container>
	SceneGraph(const ct::Scene<AOB>& sr, const Container<ct::Style>& sc);

	[[nodiscard]] auto get_parent_index(size_type index) const noexcept -> size_type;

	[[nodiscard]] auto get_parent(size_type index) noexcept -> data_type&;

	void apply_style(data_type& node, const ct::Style& style);

	[[nodiscard]] auto root() noexcept -> data_type& {
		return root_;
	}

	[[nodiscard]] auto root() const noexcept -> const data_type& {
		return root_;
	}

private:
	data_type root_;
};

template <u64 AOB, template <typename, u64> typename Container, u64 N>
SceneGraph::SceneGraph(const ct::Scene<AOB>& sr, const Container<ct::Style, N>& sc) : tree_t{}, root_() {
	this->vec_.reserve(AOB);

	for (const auto& style : sc) {
		if (style.name().compare("root") == 0) {
			apply_style(root_, style);
		}
	}

	for (std::size_t i = 0; i < sr.length(); ++i) {
		using children_t = typename tree_t::node_type::vec_t;

		const auto& [t_block, t_children, t_depth] = sr.get(i);
		this->emplace_back(data_type(t_block.name(), t_block.text()), children_t{}, t_depth);
		auto& node = this->back();
		for (const auto idx : t_children) node.children().push_back(idx);

		auto& node_data = node.data();

		for (const auto style_name : t_block.style_list()) {
			for (const auto& style : sc) {
				if (style.name().compare("root") == 0) continue;
				if (style_name != style.name()) continue;
				apply_style(node_data, style);
			}
		}
	}
}

template <u64 AOB, template <typename> typename Container>
SceneGraph::SceneGraph(const ct::Scene<AOB>& sr, const Container<ct::Style>& sc) {
	this->vec_.reserve(AOB);

	for (const auto& style : sc) {
		if (style.name().compare("root") == 0) {
			apply_style(root_, style);
		}
	}

	for (std::size_t i = 0; i < sr.length(); ++i) {
		using children_t = typename tree_t::node_type::vec_t;

		const auto& [t_block, t_children, t_depth] = sr.get(i);
		this->emplace_back(data_type(t_block.name(), t_block.text()), children_t{}, t_depth);
		auto& node = this->back();
		for (const auto idx : t_children) node.children().push_back(idx);

		auto& node_data = node.data();

		for (const auto style_name : t_block.style_list()) {
			for (const auto& style : sc) {
				if (style.name().compare("root") == 0) continue;
				if (style_name != style.name()) continue;
				apply_style(node_data, style);
			}
		}
	}
}

auto SceneGraph::get_parent_index(const size_type index) const noexcept -> size_type {
	if (index == root_index) return root_index;
	const auto it = std::find_if(this->begin(), this->end(), [index](const auto& node) {
		const auto& children = node.children();
		return std::find(children.begin(), children.end(), index) != children.end();
	});
	if (it == this->end()) return root_index;

	return std::distance(this->begin(), it);
}

auto SceneGraph::get_parent(const size_type index) noexcept -> data_type& {
	const auto idx = get_parent_index(index);
	if (idx == root_index) return root_;

	return this->operator[](idx).data();
}

void SceneGraph::apply_style(data_type& node, const ct::Style& style) {
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
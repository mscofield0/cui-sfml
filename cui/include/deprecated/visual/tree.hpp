#ifndef CUI_TREE_HPP
#define CUI_TREE_HPP

#include <vector>
#include <utility>
#include "../utility/utility.hpp"

namespace cui {

template <typename T>
struct node
{
	node* parent;
	std::vector<node*> children;
	T data;
	int id;
	int depth;

	node(node* parent, T data, int id, int depth) : parent(parent), data(std::move(data)), id(id), depth(depth) {}

	bool add_child(node* child) {
		if (child == nullptr) return false;
		children.push_back(child);
		return true;
	}

	~node() {
		for (auto node : children) delete node;
	}
};

template <typename T>
using node_ptr = node<T>*;

template <typename T>
struct tree
{
	node_ptr<T> root;
	uint32_t id_count;
	std::vector<node<T>*> linear;

	tree(T data) : root(new node<T>(nullptr, data, 0, 0)), id_count(1) {
		linear.push_back(root);
	}

	bool add_node(uint32_t id, T data) {
		if (id >= id_count) return false;
		auto* temp = new node<T>(linear[id], data, id_count, linear[id]->depth + 1);
		linear[id]->add_child(temp);
		linear.push_back(temp);
		++id_count;
		return true;
	}

	std::vector<node_ptr<T>> search_by_depth(int depth) {
		std::vector<node_ptr<T>> result;
		for (auto node : linear) {
			if (node->depth == depth) result.push_back(node);
		}
		return result;
	}

	[[nodiscard]] uint32_t size() const noexcept {
		return id_count;
	}

	[[nodiscard]] node_ptr<T> last_node() const noexcept {
		return linear[id_count - 1];
	}

	~tree() {
		delete root;
	}
};

}	 // namespace cui

#endif	  //CUI_TREE_HPP

#ifndef CUI_VISUAL_METADATA_NODE_HPP
#define CUI_VISUAL_METADATA_NODE_HPP

#include <visual/node.hpp>
#include <global/sized_string.hpp>
#include <global/min.hpp>
#include <compile_time/string_view.hpp>

namespace cui::metadata {

class Node
{
public:
	using str_t = cui::SizedString<64>;
	Node(const cui::StringView str) : node_(), name_(sv_to_ss(str)) {}

	auto node() noexcept -> cui::Node& {
		return node_;
	}

	auto node() const noexcept -> const cui::Node& {
		return node_;
	}

	auto name() noexcept -> str_t& {
		return name_;
	}

	auto name() const noexcept -> const str_t& {
		return name_;
	}

private:
	static constexpr str_t sv_to_ss(const cui::StringView str) {
		const auto size = cui::min(str.size(), size_t{64});
		std::size_t i = 0;
		str_t result;
		while (i < size) {
			result.push_back(str[i++]);
		}
		if (i != size - 1) {
			result.push_back(0);
		}
		return result;
	}

	cui::Node node_;
	cui::SizedString<64> name_;
};

}	 // namespace cui::metadata

#endif	  //  CUI_VISUAL_METADATA_NODE_HPP
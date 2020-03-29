#ifndef CUI_SCENE_GRAPH_HPP
#define CUI_SCENE_GRAPH_HPP

#include <utility.hpp>
#include "scene.hpp"
#include "styles.hpp"
#include <array>

namespace cui::ct {

template <uint32_t AOB, uint32_t Depth>
struct scene_graph
{
	std::array<std::array<cui::ct::block, Depth>, AOB>
};

}	 // namespace cui::ct

#endif	  // CUI_SCENE_GRAPH_HPP
#ifndef CUI_SFML_RENDER_CACHE_HPP
#define CUI_SFML_RENDER_CACHE_HPP

#include <vector>

#include <SFML/Graphics.hpp>
#include <cui/visual/scene_graph.hpp>
#include <cui/containers/vector.hpp>

#include <render_context/visual_element.hpp>

namespace cui {

class RenderCache : public Vector<VisualElement>
{};

}	 // namespace cui

#endif	  // CUI_SFML_RENDER_CACHE_HPP
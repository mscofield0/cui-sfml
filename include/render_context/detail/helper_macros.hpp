#ifndef CUI_SFML_DETAIL_HELPER_MACROS_HPP
#define CUI_SFML_DETAIL_HELPER_MACROS_HPP

#define DEFINE_FUNCTION(fn_name) \
	void fn_name(const SceneGraph& graph, \
				 const RenderCache& cache, \
				 const Node& node, \
				 const u64 index, \
				 VisualElement& ve)

#endif	  // CUI_SFML_DETAIL_DEPENDENCIES_HPP
#ifndef CUI_SFML_TEST_RENDER_CONTEXT_HPP
#define CUI_SFML_TEST_RENDER_CONTEXT_HPP

#include <functional>
#include <utility>

#include <cui/visual/scene_graph.hpp>
#include <render_context/visual_element.hpp>
#include <render_context/test_render_cache.hpp>
#include <render_context/window_options.hpp>
#include <SFML/Graphics.hpp>

namespace cui {

class TestRenderContext
{
public:
	using graph_ref = std::reference_wrapper<SceneGraph>;
	using node_iterator_t = typename SceneGraph::const_iterator;
	using window_ptr_t = std::unique_ptr<sf::RenderWindow>;
	TestRenderContext(SceneGraph& p_graph)
		: graph_(std::ref(p_graph)), cache_(), window_(nullptr) {}

	void init(const WindowOptions& options) {}

	void update() {}

	void render() {}

	[[nodiscard]] auto graph() noexcept -> graph_ref& {
		return graph_;
	}

	[[nodiscard]] auto graph() const noexcept -> graph_ref {
		return graph_;
	}

	[[nodiscard]] auto cache() noexcept -> TestRenderCache& {
		return cache_;
	}

	[[nodiscard]] auto cache() const noexcept -> const TestRenderCache& {
		return cache_;
	}

	[[nodiscard]] auto window() noexcept -> window_ptr_t& {
		return window_;
	}

	[[nodiscard]] auto window() const noexcept -> const window_ptr_t& {
		return window_;
	}

private:
	graph_ref graph_;
	TestRenderCache cache_;
	window_ptr_t window_;
};

}	 // namespace cui

#endif	  // CUI_SFML_TEST_RENDER_CONTEXT_HPP
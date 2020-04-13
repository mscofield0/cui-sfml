#ifndef CUI_SFML_RENDER_CONTEXT_HPP
#define CUI_SFML_RENDER_CONTEXT_HPP

#include <functional>
#include <utility>

#include <cui/visual/scene_graph.hpp>
#include <render_context/visual_element.hpp>
#include <render_context/render_cache.hpp>
#include <render_context/window_options.hpp>
#include <SFML/Graphics.hpp>

namespace cui {

class RenderContext
{
public:
	using graph_ref = std::reference_wrapper<SceneGraph>;
	using node_iterator_t = typename SceneGraph::const_iterator;
	using window_ptr_t = std::unique_ptr<sf::RenderWindow>;
	RenderContext(SceneGraph& p_graph) : graph_(std::ref(p_graph)), cache_(), window_(nullptr) {}

	RenderContext(const RenderContext&) = delete;
	RenderContext(RenderContext&&) = delete;
	RenderContext& operator=(const RenderContext&) = delete;
	RenderContext& operator=(RenderContext&&) = delete;

	void init(const WindowOptions& options);

	void update();

	void render() const noexcept;

	[[nodiscard]] auto graph() noexcept -> graph_ref& {
		return graph_;
	}

	[[nodiscard]] auto graph() const noexcept -> graph_ref {
		return graph_;
	}

	[[nodiscard]] auto cache() noexcept -> RenderCache& {
		return cache_;
	}

	[[nodiscard]] auto cache() const noexcept -> const RenderCache& {
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
	RenderCache cache_;
	window_ptr_t window_;
};

void RenderContext::init(const WindowOptions& options) {
	const auto& [w, h, title, style, ctx_settings, framerate] = options;
	auto& sg = graph().get();
	sg.root().default_schematic().width() = static_cast<int>(w);
	sg.root().default_schematic().height() = static_cast<int>(h);
	auto& e_schemes = sg.root().event_schematics();
	for (auto it = e_schemes.begin(); it != e_schemes.end(); ++it) {
		it.value().width() = static_cast<int>(w);
		it.value().height() = static_cast<int>(h);
	}
	cache_ = RenderCache::populate(graph_.get());

	window_ = std::make_unique<sf::RenderWindow>(sf::VideoMode(w, h), title, style, ctx_settings);
	window_->setFramerateLimit(framerate);
}

void RenderContext::update() {
	cache_.update_cache(graph_.get());
}

void RenderContext::render() const noexcept {
	window_->clear();
	for (const auto& el : cache_) {
		window_->setView(el);
		window_->draw(el);
		window_->draw(el.text());
	}
	window_->display();
}

}	 // namespace cui

#endif	  // CUI_SFML_RENDER_CONTEXT_HPP
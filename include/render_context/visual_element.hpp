#ifndef CUI_SFML_VISUAL_ELEMENT_HPP
#define CUI_SFML_VISUAL_ELEMENT_HPP

#include <SFML/Graphics.hpp>

namespace cui {

// using VisualElement = sf::RectangleShape;

class VisualElement : public sf::View, public sf::RectangleShape
{
public:
	using shape_t = sf::RectangleShape;
	using view_t = sf::View;

	[[nodiscard]] auto text() noexcept -> sf::Text& {
		return text_;
	}

	[[nodiscard]] auto text() const noexcept -> const sf::Text& {
		return text_;
	}

	void setPosition(float x, float y, const VisualElement& root) {
		shape_t::setPosition(x, y);

		const auto [rw, rh] = root.getSize();

		const auto viewport_x = x / rw;
		const auto viewport_y = y / rh;
		const auto [_0, _1, viewport_w, viewport_h] = view_t::getViewport();
		const auto viewport_rect = sf::FloatRect(viewport_x, viewport_y, viewport_w, viewport_h);

		const auto [w, h] = shape_t::getSize();

		view_t::setViewport(viewport_rect);
		view_t::setCenter(x + w / 2, y + h / 2);
	}

	[[nodiscard]] auto getSize() const noexcept -> const sf::Vector2f& {
		return view_t::getSize();
	}

	void setSize(float w, float h, const VisualElement& root) {
		shape_t::setSize(sf::Vector2f{w, h});
		view_t::setSize(w, h);

		const auto [rw, rh] = root.getSize();

		const auto viewport_w = w / rw;
		const auto viewport_h = h / rh;
		const auto [viewport_x, viewport_y, _2, _3] = view_t::getViewport();
		const auto viewport_rect = sf::FloatRect(viewport_x, viewport_y, viewport_w, viewport_h);

		const auto [x, y] = shape_t::getPosition();

		view_t::setViewport(viewport_rect);
		view_t::setCenter(x + w / 2, y + h / 2);
	}

	void setSize(const sf::Vector2f& size, const VisualElement& root) {
		shape_t::setSize(size);
		view_t::setSize(size);

		const auto [rw, rh] = root.getSize();

		const auto viewport_w = size.x / rw;
		const auto viewport_h = size.y / rh;
		const auto [viewport_x, viewport_y, _2, _3] = view_t::getViewport();
		const auto viewport_rect = sf::FloatRect(viewport_x, viewport_y, viewport_w, viewport_h);

		const auto [x, y] = shape_t::getPosition();

		view_t::setViewport(viewport_rect);
		view_t::setCenter(x + size.x / 2, y + size.y / 2);
	}

private:
	sf::Text text_;
};

}	 // namespace cui

#endif	  // CUI_SFML_VISUAL_ELEMENT_HPP
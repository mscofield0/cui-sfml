#include <SFML/Graphics.hpp>
#include <cui/window.hpp>
#include <render_context/render_context.hpp>
#include <utility>

int main() {
	auto win = std::make_unique<sf::RenderWindow>(sf::VideoMode(800, 600), "Title");

	while (win->isOpen()) {
		// check all the window's events that were triggered since the last
		// iteration of the loop
		sf::Event event;
		while (win->pollEvent(event)) {
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed) win->close();
		}

		// clear the window with black color
		win->clear(sf::Color::Black);

		// draw everything here...
		// window.draw(...);

		// end the current frame
		win->display();
	}
}
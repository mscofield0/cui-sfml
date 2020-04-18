#ifndef CUI_SFML_CURSORS_HPP
#define CUI_SFML_CURSORS_HPP

#include <SFML/Window/Cursor.hpp>

/// \brief Collection of system cursors provided by SFML
namespace cui::cursors {

static sf::Cursor Arrow;
static sf::Cursor ArrowWait;
static sf::Cursor Wait;
static sf::Cursor Text;
static sf::Cursor Hand;
static sf::Cursor SizeHorizontal;
static sf::Cursor SizeVertical;
static sf::Cursor SizeTopLeftBottomRight;
static sf::Cursor SizeBottomLeftTopRight;
static sf::Cursor SizeAll;
static sf::Cursor Cross;
static sf::Cursor Help;

void init_cursors() {
	Arrow.loadFromSystem(sf::Cursor::Arrow);
	ArrowWait.loadFromSystem(sf::Cursor::ArrowWait);
	Wait.loadFromSystem(sf::Cursor::Wait);
	Text.loadFromSystem(sf::Cursor::Text);
	Hand.loadFromSystem(sf::Cursor::Hand);
	SizeHorizontal.loadFromSystem(sf::Cursor::SizeHorizontal);
	SizeVertical.loadFromSystem(sf::Cursor::SizeVertical);
	SizeTopLeftBottomRight.loadFromSystem(sf::Cursor::SizeTopLeftBottomRight);
	SizeBottomLeftTopRight.loadFromSystem(sf::Cursor::SizeBottomLeftTopRight);
	SizeAll.loadFromSystem(sf::Cursor::SizeAll);
	Cross.loadFromSystem(sf::Cursor::Cross);
	Help.loadFromSystem(sf::Cursor::Help);
}

}	 // namespace cui::cursors

#endif	  // CUI_SFML_CURSORS_HPP
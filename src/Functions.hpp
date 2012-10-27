#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include <SFML/Config.hpp>
#include <SFML/System/Vector2.hpp>

namespace sf
{
	class CircleShape;
	class RectangleShape;
	class RenderWindow;
	class Text;
}

void centerTextOrigin( sf::Text& text );
void centerTextPosition( const sf::RectangleShape& rect, sf::Text& text );
void fixRect( sf::RenderWindow& window, sf::RectangleShape& rect );
void fixCircle( sf::RenderWindow& window, sf::CircleShape& circ );

// OS specific
void moveToPos( sf::RenderWindow& window, sf::Vector2i pos );
void setTransparent( sf::RenderWindow& window, sf::Uint8 alpha );
void moveToTop( sf::RenderWindow& window );
void focus( sf::RenderWindow& window );
sf::Vector2i getScreenPos();
sf::Vector2i getScreenSize();
void takeSnapshot( sf::RenderWindow& window, sf::RectangleShape& rect );

#endif // FUNCTIONS_HPP

#include "Functions.hpp"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Mouse.hpp>

void centerTextOrigin( sf::Text& text )
{
	text.setOrigin( text.getLocalBounds().width / 2, text.getLocalBounds().height / 2 );
}

void centerTextPosition( const sf::RectangleShape& rect, sf::Text& text )
{
	text.setPosition( rect.getPosition().x + ( rect.getLocalBounds().width / 2 ), rect.getPosition().y + ( rect.getLocalBounds().height / 2 ) - ( text.getCharacterSize() / 2 ) );
}

void fixRect( sf::RenderWindow& window, sf::RectangleShape& rect )
{
	sf::Vector2i mousePos = sf::Mouse::getPosition( window );
	sf::Vector2f rectPos = rect.getPosition();
	rect.setSize( sf::Vector2f( mousePos.x - rectPos.x, mousePos.y - rectPos.y ) );
}

void fixCircle( sf::RenderWindow& window, sf::CircleShape& circ )
{
	sf::Vector2i mousePos = sf::Mouse::getPosition( window );
	circ.setPosition( mousePos.x, mousePos.y );
}

#include "Snapper.hpp"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

#include "Functions.hpp"

void snap( sf::Font& font )
{
	sf::RenderWindow window( sf::VideoMode( getScreenSize().x, getScreenSize().y ), "QuickSnap - Snap", sf::Style::None );
	window.setFramerateLimit( 30 );
	setTransparent( window, 128 );
	moveToPos( window, getScreenPos() );
	
	sf::CircleShape circ;
	circ.setRadius( 4 );
	circ.setFillColor( sf::Color::Black );
	circ.setOrigin( 4, 4 );
	
	sf::RectangleShape rect;
	rect.setOutlineColor( sf::Color::Black );
	rect.setOutlineThickness( 3 );
	rect.setFillColor( sf::Color( 255, 255, 255, 64 ) );
	
	bool didFirstClick = false;
	bool didFirstRelease = false;
	
	bool isRunning = true;
	while ( isRunning )
	{
		moveToTop( window );
		focus( window );
		
		sf::Event event;
		while ( window.pollEvent( event ) )
		{
			if ( event.type == sf::Event::KeyPressed )
			{
				if ( event.key.code == sf::Keyboard::Return )
				{
					window.close();
					takeSnapshot( rect );
					isRunning = false;
				}
				else if ( event.key.code == sf::Keyboard::Escape )
				{
					isRunning = false;
				}
			}
			else if ( event.type == sf::Event::MouseButtonPressed )
			{
				if ( event.mouseButton.button == sf::Mouse::Left )
				{
					didFirstClick = true;
					didFirstRelease = false;
					rect.setPosition( event.mouseButton.x, event.mouseButton.y );
				}
			}
			else if ( event.type == sf::Event::MouseButtonReleased )
			{
				if ( event.mouseButton.button == sf::Mouse::Left )
				{
					didFirstRelease = true;
					fixRect( window, rect );
				}
			}
		}
		
		fixCircle( window, circ );
		if ( !didFirstRelease )
		{
			fixRect( window, rect );
		}
		
		window.clear( sf::Color( 255, 255, 255, 64 ) );
		if ( didFirstClick )
		{
			window.draw( rect );
		}
		window.draw( circ );
		window.display();
	}
}

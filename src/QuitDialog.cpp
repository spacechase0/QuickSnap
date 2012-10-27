#include "QuitDialog.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Event.hpp>

#include "Functions.hpp"

bool runQuitDialog( sf::Font& font )
{
	sf::RenderWindow window( sf::VideoMode( 320, 240 ), "QuickSnap - Quit" );
	window.setFramerateLimit( 30 );
	
	sf::Text mainText;
	mainText.setFont( font );
	mainText.setCharacterSize( 15 );
	mainText.setColor( sf::Color::Black );
	mainText.setString( "Are you sure you want\n  to close QuickSnap?" );
	centerTextOrigin( mainText );
	mainText.setPosition( 320 / 2, ( 240 / 2 ) - 32 );
	
	sf::RectangleShape yesRect;
	yesRect.setSize( sf::Vector2f( 145, 32 ) );
	yesRect.setPosition( 320 - 145 - 10, 240 - 32-  10 );
	yesRect.setFillColor( sf::Color::Green );
	yesRect.setOutlineThickness( 2.f );
	yesRect.setOutlineColor( sf::Color::Black );
	
	sf::Text yesText = mainText;
	yesText.setString( "Yes" );
	centerTextOrigin( yesText );
	centerTextPosition( yesRect, yesText );
	
	sf::RectangleShape noRect = yesRect;
	noRect.setFillColor( sf::Color::Red );
	noRect.move( -145 - 10, 0 );
	
	sf::Text noText = yesText;
	noText.setString( "No" );
	noText.move( -145 - 10, 0 );
	
	bool isRunning = true;
	while ( isRunning )
	{
		moveToTop( window );
		
		sf::Event event;
		while ( window.pollEvent( event ) )
		{
			if ( event.type == sf::Event::Closed )
			{
				return true;
			}
			else if ( event.type == sf::Event::KeyPressed )
			{
				if ( event.key.code == sf::Keyboard::Escape )
				{
					return true;
				}
				else if ( event.key.code == sf::Keyboard::Return )
				{
					return false;
				}
			}
			else if ( event.type == sf::Event::MouseButtonPressed )
			{
				if ( event.mouseButton.button != sf::Mouse::Left )
				{
					continue;
				}
				
				if ( yesRect.getGlobalBounds().contains( event.mouseButton.x, event.mouseButton.y ) )
				{
					return false;
				}
				else if ( noRect.getGlobalBounds().contains( event.mouseButton.x, event.mouseButton.y ) )
				{
					return true;
				}
			}
		}
		
		window.clear( sf::Color::White );
		{
			window.draw( mainText );
			
			window.draw( yesRect );
			window.draw( yesText );
			
			window.draw( noRect );
			window.draw( noText );
		}
		window.display();
	}
	
	return true;
}

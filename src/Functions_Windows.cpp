#include "Functions.hpp"

#include <boost/filesystem.hpp>
#include <SFML/Config.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <util/String.hpp>

namespace fs = boost::filesystem;

#ifdef SFML_SYSTEM_WINDOWS
#define _WIN32_WINNT 0x0500
#include <windows.h>

void moveToPos( sf::RenderWindow& window, sf::Vector2i pos )
{
	SetWindowPos( window.getSystemHandle(), NULL, pos.x, pos.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER );
}

// http://en.sfml-dev.org/forums/index.php?topic=9461.msg64239#msg64239
void setTransparent( sf::RenderWindow& window, sf::Uint8 alpha )
{
	SetWindowLong( window.getSystemHandle(),GWL_EXSTYLE,GetWindowLong(window.getSystemHandle(), GWL_EXSTYLE) | WS_EX_LAYERED); 
	SetLayeredWindowAttributes(window.getSystemHandle(), RGB(0,0,0), alpha, LWA_ALPHA);
	SetWindowPos( window.getSystemHandle(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED );
}

void moveToTop( sf::RenderWindow& window )
{
	SetWindowPos( window.getSystemHandle(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE );
}

void focus( sf::RenderWindow& window )
{
	SetForegroundWindow( window.getSystemHandle() );
}

sf::Vector2i getScreenPos()
{
	sf::Vector2i pos;
	
	pos.x = GetSystemMetrics( SM_XVIRTUALSCREEN );
	pos.y = GetSystemMetrics( SM_YVIRTUALSCREEN );
	
	return pos;
}

sf::Vector2i getScreenSize()
{
	sf::Vector2i size;
	
	size.x = GetSystemMetrics( SM_CXVIRTUALSCREEN );
	size.y = GetSystemMetrics( SM_CYVIRTUALSCREEN );
	
	return size;
}

// http://stackoverflow.com/questions/158151/how-can-i-save-a-screenshot-directly-to-a-file-in-windows
// http://www.programmersheaven.com/mb/graphics/362469/362469/create-bitmap-on-runtime/
void takeSnapshot( sf::RenderWindow& window, sf::RectangleShape& rect )
{
	HWND desktop = GetDesktopWindow();
	HDC dc = GetWindowDC( desktop );
	
	HDC myDc = CreateCompatibleDC( dc );
	
	sf::Vector2f pos = rect.getPosition();
	sf::Vector2f size = rect.getSize();
	if ( size.x < 0 )
	{
		pos.x += size.x;
		size.x = -size.x;
	}
	if ( size.y < 0 )
	{
		pos.y += size.y;
		size.y = -size.y;
	}
	pos.x += getScreenPos().x;
	pos.y += getScreenPos().y;
	
	HBITMAP bitmap = CreateCompatibleBitmap( dc, size.x, size.y );
	HGDIOBJ old = SelectObject( myDc, bitmap );
	{
		BitBlt( myDc, 0, 0, size.x, size.y, dc, pos.x, pos.y, SRCCOPY );
		
		{
			// http://msdn.microsoft.com/en-us/library/windows/desktop/dd183402%28v=vs.85%29.aspx
			BITMAPFILEHEADER   bmfHeader;    
			BITMAPINFOHEADER   bi;
			
			std::vector< sf::Uint8 > bits;
			bits.resize( size.x * size.y * 4 );
			
			bi.biSize = sizeof(BITMAPINFOHEADER);    
			bi.biWidth = size.x;   
			bi.biHeight = size.y;
			bi.biPlanes = 1;    
			bi.biBitCount = 32;    
			bi.biCompression = BI_RGB;    
			bi.biSizeImage = 0;  
			bi.biXPelsPerMeter = 0;    
			bi.biYPelsPerMeter = 0;    
			bi.biClrUsed = 0;    
			bi.biClrImportant = 0;
			
			int got = GetDIBits( myDc, bitmap, 0, size.y, &bits[ 0 ], ( BITMAPINFO* ) &bi, DIB_RGB_COLORS );
			if ( got == 0 )
			{
				std::cout << "ERROR" << std::endl;
			}
			
			sf::Image img;
			img.create( size.x, size.y, sf::Color( 255, 0, 255 ) );
			for ( std::size_t ix = 0; ix < size.x; ++ix )
			{
				for ( std::size_t iy = 0; iy < size.y; ++iy )
				{
					
					sf::Color sfCol;
					/*
					// Slower
					COLORREF col = GetPixel( myDc, ix, iy );
					sfCol.r = ( col >>  0 ) & 0xFF;
					sfCol.g = ( col >>  8 ) & 0xFF;
					sfCol.b = ( col >> 16 ) & 0xFF;
					//*/
					//*
					int y = size.y - iy - 1; // Otherwise, the Y is flipped :P
					std::size_t index = ( ix + ( y * size.x ) ) * 4;
					sfCol.r = bits[ index + 2 ];
					sfCol.g = bits[ index + 1 ];
					sfCol.b = bits[ index + 0 ];
					//*/
					sfCol.a = 255;
					
					img.setPixel( ix, iy, sfCol );
				}
			}
			
			std::string appdata = getenv( "APPDATA" );
			std::string path = appdata + "/quicksnap";
			if ( !fs::exists( path ) )
			{
				fs::create_directory( path );
			}
			img.saveToFile( path + "/" + util::toString( std::time( NULL ) ) + "_" + util::toString( rand() ) + ".png" );
		}
		{
			OpenClipboard( NULL );
			EmptyClipboard();
			SetClipboardData( CF_BITMAP, bitmap );
			CloseClipboard();
		}
	}
	SelectObject( myDc, old );
	DeleteDC( myDc );
	ReleaseDC( desktop, dc );
}

#endif

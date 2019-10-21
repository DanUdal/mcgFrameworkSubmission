#include <SDL/SDL.h>
#include <iostream>
#include "MCG_GFX_Lib.h"


namespace MCG
{
	SDL_Renderer *_renderer;
	SDL_Window *_window;
	glm::ivec2 _winSize;
	unsigned int _lastTime;
}


bool MCG::Init( glm::ivec2 windowSize )
{
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		std::cout << "MCG Framework: Whoops! Something went very wrong, cannot initialise SDL :(" << std::endl;
		return false;
	}

	_winSize = windowSize;
	int winPosX = 100;
	int winPosY = 100;
	_window = SDL_CreateWindow( "MCG GFX Framework",
		winPosX, winPosY,
		_winSize.x, _winSize.y,
		SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE );

	if( !_window )
	{
		std::cout << "MCG Framework: Whoops! Something went very wrong, cannot create SDL window :(" << std::endl;
		return false;
	}
	_renderer = SDL_CreateRenderer( _window, -1, 0 );


	if( !_renderer )
	{
		std::cout << "MCG Framework: Whoops! Something went very wrong, cannot obtain SDL rendering context :(" << std::endl;
		return false;
	}



	_lastTime = SDL_GetTicks();

	return true;
}

void MCG::SetBackground( glm::ivec3 colour )
{
	// Set the colour for drawing
	SDL_SetRenderDrawColor( _renderer, colour.r, colour.g, colour.b, 255 );
	// Clear the entire screen to our selected colour
	SDL_RenderClear( _renderer );

}

void MCG::DrawPixel( glm::ivec2 position, glm::ivec3 colour )
{
	// Set the colour for drawing
	SDL_SetRenderDrawColor( _renderer, colour.r, colour.g, colour.b, 255 );
	// Draw our pixel
	SDL_RenderDrawPoint( _renderer, position.x, position.y );
}


bool MCG::ProcessFrame()
{
	SDL_RenderPresent( _renderer );


	SDL_Event incomingEvent;
	while( SDL_PollEvent( &incomingEvent ) )
	{
		switch( incomingEvent.type )
		{
		case SDL_QUIT:
			return false;


		case SDL_KEYUP:
			switch( incomingEvent.key.keysym.sym )
			{
				case SDLK_ESCAPE:
					return false;
			}
		}
	}
	unsigned int current = SDL_GetTicks();
	float deltaTs = (float)( current - _lastTime ) / 1000.0f;
	_lastTime = current;
	if( deltaTs < ( 1.0f / 60.0f ) )
	{
		SDL_Delay( (unsigned int)( ( ( 1.0f / 60.0f ) - deltaTs )*1000.0f ) );
	}


	return true;

}

void MCG::Cleanup()
{
	SDL_DestroyWindow( _window );
	SDL_Quit();
}


int MCG::ShowAndHold()
{
	SDL_RenderPresent( _renderer );
	bool go = true;
	while( go )
	{
		SDL_Event incomingEvent;
		SDL_WaitEvent( &incomingEvent );
		switch( incomingEvent.type )
		{
		case SDL_QUIT:
			go = false;
			break;


		case SDL_KEYUP:
			switch( incomingEvent.key.keysym.sym )
			{
			case SDLK_ESCAPE:
				return 0;
			}
		}



	}
	Cleanup();
	return 0;
}


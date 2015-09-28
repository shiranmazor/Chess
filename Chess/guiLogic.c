#include "gui.h"

#define malloc(x) myMalloc(x)
#define free(x) myFree(x)
#define calloc(x,y) myCalloc(x,y)
#define realloc(x,y) myRealloc(x,y)

void CreateMainWindow()
{
	/*
	SDL_Surface* mainWindows =
	SDL_Flip(mainWindows);
	//set white background
	SDL_Rect screenRect;
	screenRect.x = screenRect.y = 0;
	screenRect.w = WIN_WIDTH;
	screenRect.h = WIN_HEIGHT;
	Uint32 clearColor = SDL_MapRGB(mainWindows->format, 255, 255, 255);
	SDL_FillRect(mainWindows, &screenRect, clearColor);
	SDL_Flip(mainWindows);
	atexit(SDL_Quit);
	return mainWindows;
	*/
	
}


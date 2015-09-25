#include "SDL.h"
#define WIN_TITLE "Chess"
#define WIN_HEIGHT 600
#define WIN_WIDTH 800

SDL_Surface* loadImage(char * filename, int x, int y, SDL_Surface * window)
{
	SDL_Rect imgrect;
	
	imgrect.x = x;
	imgrect.y = y;

	SDL_Surface* img = SDL_LoadBMP(filename);
	//Apply image to screen
	if (SDL_BlitSurface(img, NULL, window, &imgrect) != 0)
	{
		printf("ERROR: failed to blit image : %s\n", SDL_GetError());
		SDL_FreeSurface(img);
		exit(1);
	}
	//Update Screen
	SDL_Flip(window);

	return img;

}

int main(int argc, char* args[])
{
	//Start SDL
	SDL_Init(SDL_INIT_VIDEO);
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("ERROR: unable to init SDL : %s\n", SDL_GetError());
		return 1;
	}
	
	SDL_WM_SetCaption("Chess","Chess");
	SDL_Surface* win = SDL_SetVideoMode(WIN_WIDTH, WIN_HEIGHT, 0, SDL_HWSURFACE | SDL_DOUBLEBUF);
	SDL_Flip(win);

	//set white background
	SDL_Rect screenRect;
	screenRect.x = screenRect.y = 0;
	screenRect.w = WIN_WIDTH;
	screenRect.h = WIN_HEIGHT;
	Uint32 clearColor = SDL_MapRGB(win->format, 255, 255, 255);
	SDL_FillRect(win, &screenRect, clearColor);
	SDL_Flip(win);

	//add menu images
	SDL_Surface* newGameImg = loadImage("images/NewGame.bmp", 315, 150, win);
	SDL_Surface* loadGameImg = loadImage("images/LoadGame.bmp", 315, 230, win);
	SDL_Surface* quitGameImg = loadImage("images/Quit.bmp", 315, 310, win);
	/*SDL_Rect rect = { 100, 100, 500, 200 };

	SDL_FillRect(win, &rect, 122);
	SDL_Flip(win);*/


	SDL_Delay(2000);
	
	SDL_Event e;
	while (SDL_PollEvent(&e) != 0)
	{
		if (e.type == SDL_QUIT)
		{

			SDL_Quit();
			exit(0);
		}
	}

	SDL_FreeSurface(newGameImg);
	SDL_FreeSurface(loadGameImg);
	SDL_FreeSurface(quitGameImg);
	//Quit SDL
	SDL_Quit();

	return 0;
}
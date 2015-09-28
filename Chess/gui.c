#include "gui.h"

//structures
struct ImgButton
{
	SDL_Surface * surface;
	int x;
	int y;
	char * filename;
};

//Panel contains multiple surfaces (up to 20)
struct Panel
{
	SDL_Surface * surface;
	int x;
	int y;
	
	SDL_Surface * buttonsArr[20];
};



int isButtonClicked(ImgButton btn, int clickedX, int clickedY)
{
	if (clickedX > btn.x && clickedX < btn.x + btn.surface->w &&
		clickedY > btn.y && clickedY < btn.y + btn.surface->h)
	{
		return 1;
	}
	return 0;
}


ImgButton createImgButton(int x, int y, char * filename, SDL_Surface * window)
{
	ImgButton btn;
	btn.x = x;
	btn.y = y;
	btn.filename = filename;

	SDL_Rect imgrect;

	imgrect.x = btn.x;
	imgrect.y = btn.y;

	btn.surface = SDL_LoadBMP(btn.filename);
	//Apply image to screen
	if (SDL_BlitSurface(btn.surface, NULL, window, &imgrect) != 0)
	{
		printf("ERROR: failed to blit image : %s\n", SDL_GetError());
		SDL_FreeSurface(btn.surface);
		exit(1);
	}
	//Update Screen
	SDL_Flip(window);

	return btn;
}

SDL_Surface * init()
{
	//Start SDL
	SDL_Init(SDL_INIT_VIDEO);
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("ERROR: unable to init SDL : %s\n", SDL_GetError());
		exit(1);
		return NULL;
	}

	SDL_WM_SetCaption("Chess", "Chess");
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
	atexit(SDL_Quit);
	return win;
}


int main2(int argc, char* args[])
{
	SDL_Surface * win = init();
	//add menu images
	ImgButton newGameImg = createImgButton(315, 150, "images/NewGame.bmp", win);;
	ImgButton loadGameImg = createImgButton(315, 230, "images/LoadGame.bmp", win);
	ImgButton quitGameImg = createImgButton(315, 310, "images/Quit.bmp", win);

	int shouldQuit = 0;

	while (!shouldQuit)
	{
		SDL_Event e;
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				shouldQuit = 1;
				SDL_Quit();
				exit(0);
			}
			else if (e.type == SDL_MOUSEBUTTONUP)
			{
				int x, y;
				SDL_GetMouseState(&x, &y);

				if (isButtonClicked(newGameImg, x, y))
				{
					
					//start new game
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


				}
				else if (isButtonClicked(loadGameImg, x, y))
				{
					//load game
				}
				else if (isButtonClicked(quitGameImg, x, y))
				{
					shouldQuit = 1;
					SDL_Quit();
					exit(0);
					//todo free resources
				}
			}
		}
		SDL_Delay(1);
	}

	//SDL_FreeSurface(newGameImg);
	//SDL_FreeSurface(loadGameImg);
	//SDL_FreeSurface(quitGameImg);
	//Quit SDL
	
	SDL_Quit();

	return 0;
}
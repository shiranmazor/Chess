#include "gui.h"
#define WIN_TITLE "Chess"
#define WIN_HEIGHT 600
#define WIN_WIDTH 800

struct ImgButton
{
	SDL_Surface * surface;
	int x;
	int y;
	char * filename;
};

//Panel contains multiple ImgButtons (up to 10)
struct Panel
{
	SDL_Surface * surface;
	int x;
	int y;
	
	ImgButton buttonsArr[10];

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

SDL_Surface* loadImage(ImgButton btn, SDL_Surface * window)
{
	SDL_Rect imgrect;

	imgrect.x = btn.x;
	imgrect.y = btn.y;

	SDL_Surface* img = SDL_LoadBMP(btn.filename);
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


int main(int argc, char* args[])
{
	SDL_Surface * win = init();
	//add menu images
	ImgButton newGameImg;
	newGameImg.x = 315;
	newGameImg.y = 150;
	newGameImg.filename = "images/NewGame.bmp";
	newGameImg.surface = loadImage(newGameImg, win);

	ImgButton loadGameImg;
	loadGameImg.x = 315;
	loadGameImg.y = 230;
	loadGameImg.filename = "images/LoadGame.bmp";
	loadGameImg.surface = loadImage(loadGameImg, win);

	ImgButton quitGameImg;
	quitGameImg.x = 315;
	quitGameImg.y = 310;
	quitGameImg.filename = "images/Quit.bmp";
	quitGameImg.surface = loadImage(quitGameImg, win);


	//SDL_Delay(2000);
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
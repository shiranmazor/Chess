#include "gui.h"


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
	int width;
	int height;
	
	Panel * panels[100];
	ImgButton buttons[100];
	int freeButtonSlot;
};

void addButtonToPanel(Panel * p, ImgButton btn)
{	
	ImgButton imgBtn = createImgButton(p->x + btn.x, p->y + btn.y, btn.filename, p->surface);
	memcpy(&(p->buttons[p->freeButtonSlot]), &(imgBtn), sizeof(ImgButton));
	p->freeButtonSlot++;

}

Panel createPanel(int x, int y, int width, int height, SDL_Surface * win, int color)
{
	Panel panel;
	panel.x = x;
	panel.y = y;
	panel.width = width;
	panel.height = height;
	panel.surface = win;
	panel.freeButtonSlot = 0;

	SDL_Rect rect;
	rect.x = panel.x;
	rect.y = panel.y;
	rect.w = width;
	rect.h = height;

	if (SDL_FillRect(win, &rect, color) != 0)
	{
		printf("ERROR: failed to blit image : %s\n", SDL_GetError());
		//SDL_FreeSurface(btn.surface);
		exit(1);
	}
	//Update Screen
	SDL_Flip(win);
	return panel;
}

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


int main(int argc, char* args[])
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

					Panel p = createPanel(WIN_WIDTH - 180, 0, 180, 600, win, SDL_MapRGB(win->format, 255, 255, 255));
					ImgButton saveGameBtn;
					saveGameBtn.x = p.width / 2 - 170 / 2;
					saveGameBtn.y = 50;
					saveGameBtn.filename = "images/SaveGame.bmp";
					addButtonToPanel(&p, saveGameBtn);

					ImgButton mainMenuBtn;
					mainMenuBtn.x = p.width / 2 - 170 / 2;
					mainMenuBtn.y = 100;
					mainMenuBtn.filename = "images/MainMenu.bmp";
					addButtonToPanel(&p, mainMenuBtn);

					ImgButton quitGameBtn;
					quitGameBtn.x = p.width / 2 - 170 / 2;
					quitGameBtn.y = p.height - 100;
					quitGameBtn.filename = "images/Quit.bmp";
					addButtonToPanel(&p, quitGameBtn);

					//vertical line - seperator
					createPanel(p.x - 2, 0, 2, WIN_HEIGHT, win, SDL_MapRGB(win->format, 0, 0, 0));
					
					//draw game board
					Panel gameBoard = createPanel(0, 0, 620, WIN_HEIGHT, win, SDL_MapRGB(win->format, 0, 0, 0));
					ImgButton btn;
					for (int i = 0; i < BOARD_SIZE; i++)
					{
						for (int j = 0; j < BOARD_SIZE; j++)
						{
							btn.x = j * 76;
							btn.y = i * 76;
							btn.filename = (j % 2 == 0 && i % 2 == 0) || (j % 2 == 1 && i % 2 == 1)  ? "images/lightRect.bmp" : "images/darkRect.bmp";
							addButtonToPanel(&gameBoard, btn);
						}
					}
					

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
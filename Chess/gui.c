#include "gui.h"
#define malloc(x) myMalloc(x)
#define free(x) myFree(x)
#define calloc(x,y) myCalloc(x,y)
#define realloc(x,y) myRealloc(x,y)

//UI Tree:
UINode* CreateNewTree(control* window, int childsNumber)
{
	UINode* root = (UINode*)malloc(sizeof(UINode));
	root->control = window;
	root->childsNumber = childsNumber;
	root->children = malloc(sizeof(UINode*)* childsNumber);
	root->father = NULL;
	return root;
}
UINode* addNewControlToTree(UINode* father, int childsNumber, control* control)
{
	UINode* node = (UINode*)malloc(sizeof(UINode));
	node->control = control;
	node->childsNumber = childsNumber;
	node->children = malloc(sizeof(UINode*)* childsNumber);
	node->father = father;
	return node;
}
void addChildToFather(UINode* father, UINode* child)
{
	father->childsNumber++;
	father->children = (UINode*)realloc(father->childsNumber, sizeof(UINode*));
	father->children[father->childsNumber - 1] = child;

}
//events:
int isButtonClicked(ImgButton btn, int clickedX, int clickedY)
{
	if (clickedX > btn.x && clickedX < btn.x + btn.surface->w &&
		clickedY > btn.y && clickedY < btn.y + btn.surface->h)
	{
		return 1;
	}
	return 0;
}

control* CreateWindow(char* title, int with, int high, SDL_Rect* rect)
{
	SDL_WM_SetCaption(title, title);
	SDL_Surface* win = SDL_SetVideoMode(with, high, 0, SDL_HWSURFACE | SDL_DOUBLEBUF);
	if (win == NULL) 
	{
		printf("ERROR: failed to set video mode: %s\n", SDL_GetError());
		return 1;
	}
	control *winControl = (control*)malloc(sizeof(control));
	winControl->surface = win;
	winControl->type = WINDOW;
	winControl->Action = NULL;
	winControl->rect = rect;
	return winControl;
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
SDL_Surface *CreateMainWindow2()
{
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

void init()
{
	//Start SDL
	SDL_Init(SDL_INIT_VIDEO);
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("ERROR: unable to init SDL : %s\n", SDL_GetError());
		exit(1);
		return NULL;
	}

}


int main(int argc, char* args[])
{
	init();
	root = NULL;
	SDL_Surface * win = CreateMainWindow();

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
#include "gui.h"
#define malloc(x) myMalloc(x)
#define free(x) myFree(x)
#define calloc(x,y) myCalloc(x,y)
#define realloc(x,y) myRealloc(x,y)

//UI Tree:
UINode* CreateAndAddUINode(void* control, int childsNumber, char type, UINode* father, void(*Action)(void*))
{
	UINode* node = (UINode*)malloc(sizeof(UINode));
	//control cast:
	if (type == WINDOW)
		node->control = (Window*)control;
	else if (type == BUTTON)
		node->control = (ImgButton*)control;
	else if (type == PANEL)
		node->control = (Panel*)control;
	else if (type == LABEL)
		node->control = (Label*)control;
	node->type = type;
	node->childsNumber = childsNumber;
	node->children = malloc(sizeof(UINode*)* childsNumber);
	node->father = father;//NULL in case of root
	node->Action = Action;//can be NULL for window
	return node;
}

void addChildToFather(UINode* father, UINode* child)
{
	father->childsNumber++;
	father->children = (UINode*)realloc(father->childsNumber, sizeof(UINode*));
	father->children[father->childsNumber - 1] = child;

}
void freeUINode(UINode* root)
{
	if (root == NULL)
		return;
	free(root->control);
	free(root->Action);
	for (int i = 0; i< root->childsNumber; i++)
	{
		freeUINode(root->children[i]);
	}
	free(root->children);
	free(root);
}
/*scan the tree in DFS left-to-right and present the controls*/
void presentUITree(UINode* root)
{
	if (root == NULL)
		return;
	//present root:
	SDL_Flip(root->control);
	for (int i = 0; i<root->childsNumber; i++)
	{
		presentUITree(root->children[i]);
	}
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

UINode* CreateWindow(char* title, int with, int high)
{
	SDL_WM_SetCaption(title, title);
	SDL_Surface* win = SDL_SetVideoMode(with, high, 0, SDL_HWSURFACE | SDL_DOUBLEBUF);
	if (win == NULL) 
	{
		printf("ERROR: failed to set video mode: %s\n", SDL_GetError());
		return 1;
	}
	Window winObj;
	winObj.height = WIN_HEIGHT;
	winObj.width = WIN_WIDTH;
	winObj.title = WIN_TITLE;
	winObj.type = WINDOW;
	winObj.surface = win;
	
	UINode *winControl = CreateAndAddUINode(&winObj, 0, WINDOW, NULL, NULL);
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
	mainWindow = NULL;
	settingWindow = NULL;
	AISettingWindow = NULL;
	gameWindow = NULL;

	/*
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

	*/
	
	return 0;
}
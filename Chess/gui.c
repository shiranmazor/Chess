#include "gui.h"
#define malloc(x) myMalloc(x)
#define free(x) myFree(x)
#define calloc(x,y) myCalloc(x,y)
#define realloc(x,y) myRealloc(x,y)

//UI Tree:
UINode* CreateAndAddUINode(void* control, int childsNumber, char type, UINode* father, void(*Action)(void*))
{
	Window* win;
	Window* win2;

	UINode* node = (UINode*)malloc(sizeof(UINode));
	//control cast:
	if (type == WINDOW)
	{
		node->control = (Window*)control;
	}		
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
	father->children = (UINode*)realloc(father->children,father->childsNumber*sizeof(UINode*));
	father->children[father->childsNumber - 1] = child;

}

void freeControl(void*  control, char type)
{
	if (type == WINDOW)
	{
		Window* win = (Window*)control;
		SDL_FreeSurface(win->surface);
	}
	else if (type == LABEL)
	{
		Label* l = (Label*)control;
		SDL_FreeSurface(l->surface);
	}
	else if (type == PANEL)
	{
		Panel* p = (Panel*)control;
		SDL_FreeSurface(p->surface);
	}
	else if (type == BUTTON)
	{
		ImgButton* b = (ImgButton*)control;
		SDL_FreeSurface(b->surface);
	}
	free(control);
}
void freeUINode(UINode* root)
{
	if (root == NULL)
		return;
	//free surface
	if (root->control != NULL)
		freeControl(root->control, root->type);
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
	//cast control:
	if (root->type == WINDOW)
	{
		Window* win = (Window*)root->control;
		SDL_WM_SetCaption(win->title, win->title);
		SDL_Flip(win->surface);
	}
	else if (root->type == PANEL)
	{
		Panel* control = (Panel*)root->control;
		SDL_Flip(control->surface);
	}
	else if (root->type == BUTTON)
	{
		ImgButton* control = (ImgButton*)root->control;
		SDL_Flip(control->surface);
	}
	else if (root->type == LABEL)
	{
		Label* control = (Label*)root->control;
		SDL_Flip(control->surface);
	}
		
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

UINode* CreateWindow(char* title, int width, int height, int childsNumber, SDL_Rect* rect)
{
	SDL_WM_SetCaption(title, title);
	SDL_Surface* win = SDL_SetVideoMode(width, height, 0, SDL_HWSURFACE | SDL_DOUBLEBUF);
	if (win == NULL) 
	{
		printf("ERROR: failed to set video mode: %s\n", SDL_GetError());
		return 1;
	}
	if (rect != NULL)
	{
		Uint32 clearColor = SDL_MapRGB(win->format, 255, 255, 255);
		SDL_FillRect(win, &rect, clearColor);
	}
	Window *winObj = (Window*)malloc(sizeof(Window));
	winObj->height = height;
	winObj->width = width;
	winObj->title = title;
	winObj->type = WINDOW;
	winObj->surface = win;
	
	UINode *winControl = CreateAndAddUINode(winObj, childsNumber, WINDOW, NULL, NULL);
	return winControl;
}
UINode* CreatePanel(SDL_Surface * surface, int x, int y, int width, int height, int color, UINode *father, int childsNumber)
{
	Panel *panel = (Panel*)malloc(sizeof(Panel));
	panel->x = x;
	panel->y = y;
	panel->type = PANEL;
	panel->surface = surface;

	SDL_Rect rect;
	rect.x = panel->x;
	rect.y = panel->y;
	rect.w = width;
	rect.h = height;
	if (SDL_FillRect(surface, &rect, color) != 0)
	{
		printf("ERROR: failed to blit image : %s\n", SDL_GetError());
			exit(1);
	}
	UINode *panelNode = CreateAndAddUINode(panel, childsNumber, PANEL, father, NULL);
	return panelNode;
}

UINode* CreateButton(SDL_Surface * surface, int x, int y, char * filename, void(*Action)(void*), UINode *father,
	int childsNumber, char* name)
{
	ImgButton* btn = (ImgButton*)malloc(sizeof(ImgButton));
	btn->x = x;
	btn->y = y;
	btn->filename = filename;
	btn->type = BUTTON;
	btn->name = name;
	SDL_Rect imgrect;
	imgrect.x = btn->x;
	imgrect.y = btn->y;

	btn->surface = SDL_LoadBMP(btn->filename);
	//apply image to screen
	if (SDL_BlitSurface(btn->surface, NULL, surface, &imgrect) != 0)
	{
		printf("ERROR: failed to blit image : %s\n", SDL_GetError());
		SDL_FreeSurface(btn->surface);
		exit(1);
	}
	UINode *buttonNode = CreateAndAddUINode(btn, childsNumber, BUTTON, father, Action);
	return buttonNode;
}

UINode* createLabel(SDL_Surface * surface, int x, int y, char * filename, UINode *father, char* name)
{
	Label* label = (Label*)malloc(sizeof(Label));
	label->x = x;
	label->y = y;
	label->filename = filename;
	label->type = LABEL;
	label->name = name;
	
	SDL_Rect imgrect;
	imgrect.x = label->x;
	imgrect.y = label->y;
	label ->surface = SDL_LoadBMP(label->filename);
	//apply image to screen
	if (SDL_BlitSurface(label->surface, NULL, surface, &imgrect) != 0)
	{
		printf("ERROR: failed to blit image : %s\n", SDL_GetError());
		SDL_FreeSurface(label->surface);
		exit(1);
	}
	UINode *labelNode = CreateAndAddUINode(label, 0, LABEL, father, NULL);
	return labelNode;
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

void EventsLoopMainWindow()
{
	
	while (!shouldQuitMainEvents)
	{
		SDL_Event e;
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				shouldQuitMainEvents = 1;
				SDL_Quit();
				exit(0);
			}
			else if (e.type == SDL_MOUSEBUTTONUP)
			{
				int x, y;
				SDL_GetMouseState(&x, &y);
				//check if buttons were clicked
				//get all ImageButtons:
				UINode** buttonNodes = mainWindow->children[0]->children;
				for (int i = 0; i < mainWindow->children[0]->childsNumber; i++)
				{
					ImgButton* btn = (ImgButton*)buttonNodes[i]->control;
					if (isButtonClicked(*btn, x, y))
					{
						buttonNodes[i]->Action(NULL);
						break;
					}
				}

			}
		}
		SDL_Delay(1);
	}
	SDL_Quit();
}
void EventsLoopPlayerSelectionWindow()
{
	while (!shouldQuitSelectionEvents)
	{
		SDL_Event e;
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				shouldQuitSelectionEvents = 1;
				SDL_Quit();
				exit(0);
			}
			else if (e.type == SDL_MOUSEBUTTONUP)
			{
			}

		}
		SDL_Delay(1);
	}
	SDL_Quit();
}
void EventsLoopGameWindow()
{
	while (!shouldQuitGameEvents)
	{
		SDL_Event e;
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				shouldQuitGameEvents = 1;
				SDL_Quit();
				exit(0);
			}
			else if (e.type == SDL_MOUSEBUTTONUP)
			{
				int x, y;
				SDL_GetMouseState(&x, &y);
				//check if buttons were clicked
				//get all ImageButtons:
				UINode** buttonNodes = gameWindow->children[0]->children;
				for (int i = 0; i < gameWindow->children[0]->childsNumber; i++)
				{
					/*
					ImgButton* btn = (ImgButton*)buttonNodes[i]->control;
					if (isButtonClicked(*btn, x, y))
					{
					buttonNodes[i]->Action(NULL);
					}
					*/
					
				}

			}
		}
		SDL_Delay(1);
	}
	SDL_Quit();
}

int main(int argc, char* args[])
{
	init();
	mainWindow = NULL;
	settingWindow = NULL;
	gameWindow = NULL;
	playerSelectionWindow = NULL;
	boardSettingsWindow = NULL;

	shouldQuitMainEvents = 0;
	shouldQuitGameEvents = 0;
	shouldQuitsettingEvents = 0;
	shouldQuitSelectionEvents = 0;
	
	CreateMainWindow();
	presentUITree(mainWindow);
	EventsLoopMainWindow();
	

	//main_old();
	freeUINode(mainWindow);
	return 0;
}
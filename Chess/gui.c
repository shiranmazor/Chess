#include "gui.h"
#define malloc(x) myMalloc(x)
#define free(x) myFree(x)
#define calloc(x,y) myCalloc(x,y)
#define realloc(x,y) myRealloc(x,y)

//UI Tree:
UINode* CreateAndAddUINode(void* control, int childsNumber, char type, UINode* father, void(*Action)(void*))
{
	Window* win;

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

void replaceUINodeChild(UINode* father, UINode* newNode,char* controlNameToreplace)
{
	for (int i = 0; i < father->childsNumber; i++)
	{
		if (strcmp(getUINodeName(father->children[i]), controlNameToreplace) == 0)
		{
			freeUINode(father->children[i]);
			father->children[i] = newNode;
			break;
		}
	}
}
int  getUINodeX(UINode* node)
{

	if (node->type == LABEL)
	{
		Label* l = (Label*)node->control;
		return l->x;
	}
	else if (node->type == PANEL)
	{
		Panel* p = (Panel*)node->control;
		return p->x;
	}
	else if (node->type == BUTTON)
	{
		ImgButton* b = (ImgButton*)node->control;
		return b->x;
	}
}
int getUINodeY(UINode* node)
{

	if (node->type == LABEL)
	{
		Label* l = (Label*)node->control;
		return l->y;
	}
	else if (node->type == PANEL)
	{
		Panel* p = (Panel*)node->control;
		return p->y;
	}
	else if (node->type == BUTTON)
	{
		ImgButton* b = (ImgButton*)node->control;
		return b->y;
	}
}
char* getUINodeName(UINode* node)
{
	if (node->type == LABEL)
	{
		Label* l = (Label*)node->control;
		return l->name;
	}
	else if (node->type == PANEL)
	{
		Panel* p = (Panel*)node->control;
		return p->name;
	}
	else if (node->type == BUTTON)
	{
		ImgButton* b = (ImgButton*)node->control;
		return b->name;
	}
}
UINode* getNodeByName(char* controlName, UINode* root)
{
	UINode* node = NULL;
	if (strcmp(getUINodeName(root), controlName) == 0)
		return root;
	for (int i = 0; i < root->childsNumber; i++)
	{
		node = getNodeByName(controlName, root->children[i]);
		if (node != NULL)
			break;
	}
	return node;
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
		if (l->rect != NULL)
			free(l->rect);
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
		if (b->rect != NULL)
			free(b->rect);
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
		Panel *p;
		Window * win;
		if (root->father->type == PANEL)
			p = (Panel*)root->father->control;
		win = getRoot(root);
		//apply image to screen
		if (SDL_BlitSurface(control->surface, NULL, win->surface, control->rect) != 0)
		{
			printf("ERROR: failed to blit image : %s\n", SDL_GetError());
			SDL_FreeSurface(control->surface);
			exit(1);
		}
		SDL_Flip(control->surface);
		SDL_Flip(win->surface);
	}
	else if (root->type == LABEL)
	{
		Label* control = (Label*)root->control;
		Panel *p;
		if (root->father->type == PANEL)
			p = (Panel*)root->father->control;
		//apply image to screen
		if (SDL_BlitSurface(control->surface, NULL, p->surface, control->rect) != 0)
		{
			printf("ERROR: failed to blit image : %s\n", SDL_GetError());
			SDL_FreeSurface(control->surface);
			exit(1);
		}
		SDL_Flip(control->surface);
		SDL_Flip(p->surface);
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
UINode* CreatePanel(SDL_Surface * surface, int x, int y, int width, int height, int color, UINode *father, int childsNumber, char* name)
{
	Panel *panel = (Panel*)malloc(sizeof(Panel));
	panel->x = x;
	panel->y = y;
	panel->type = PANEL;
	panel->surface = surface;
	panel->name = name;
	panel->width = width;
	panel->height = height;

	SDL_Rect rect;
	rect.x = panel->x;
	rect.y = panel->y;
	rect.w = width;
	rect.h = height;
	panel->rect = &rect;
	if (SDL_FillRect(surface, &rect, color) != 0)
	{
		printf("ERROR: failed to blit image : %s\n", SDL_GetError());
			exit(1);
	}
	UINode *panelNode = CreateAndAddUINode(panel, childsNumber, PANEL, father, NULL);
	return panelNode;
}

Window * getRoot(UINode * node)
{
	UINode * root = node;
	while (root->father != NULL)
		root = root->father;

	return (Window *) root->control;
}
UINode* CreateButton(SDL_Surface * surface, int x, int y, char * filename, void(*Action)(char*), UINode *father,
	int childsNumber, char* name)
{
	Window * win = getRoot(father);

	Uint32 white = SDL_MapRGB(win->surface->format, 255, 255, 255);
	return createButtonWithColor(surface, x, y, filename, Action, father, childsNumber, name, white);
}

UINode * createButtonWithColor(SDL_Surface * surface, int x, int y, char * filename, void(*Action)(char*), UINode *father,
	int childsNumber, char* name, Uint32 color)
{
	int parentX = getUINodeX(father);
	int parentY = getUINodeY(father);

	ImgButton* btn = (ImgButton*)malloc(sizeof(ImgButton));
	btn->x = parentX + x;
	btn->y = parentY + y;
	btn->filename = filename;
	btn->type = BUTTON;
	btn->name = name;
	SDL_Rect *imgrect = (SDL_Rect*)malloc(sizeof(SDL_Rect));
	imgrect->x = btn->x;
	imgrect->y = btn->y;

	btn->surface = SDL_LoadBMP(btn->filename);
	
	btn->rect = imgrect;
	btn->surface->format->Amask = 0xFF000000;
	//btn->surface->format->Ashift = 24;
	Window * win = getRoot(father);
	SDL_SetColorKey(btn->surface, SDL_SRCCOLORKEY, color);

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

	
	SDL_Rect *imgrect = (SDL_Rect*)malloc(sizeof(SDL_Rect));
	imgrect->x = label->x;
	imgrect->y = label->y;
	label->surface = SDL_LoadBMP(label->filename);
	label->rect = imgrect;
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

void toolClicked()
{
}

char * getFilenameByTool(char tool)
{
	char * filename;
	switch (tool)
	{
	case WHITE_K:
		filename = "images/tools/white_king.bmp";
		break;
	case WHITE_B:
		filename = "images/tools/white_bishop.bmp";
		break;
	case WHITE_P:
		filename = "images/tools/white_pawn.bmp";
		break;
	case WHITE_N:
		filename = "images/tools/white_knight.bmp";
		break;
	case WHITE_Q:
		filename = "images/tools/white_queen.bmp";
		break;
	case WHITE_R:
		filename = "images/tools/white_rook.bmp";
		break;
	case BLACK_K:
		filename = "images/tools/black_king.bmp";
		break;
	case BLACK_B:
		filename = "images/tools/black_bishop.bmp";
		break;
	case BLACK_P:
		filename = "images/tools/black_pawn.bmp";
		break;
	case BLACK_N:
		filename = "images/tools/black_knight.bmp";
		break;
	case BLACK_Q:
		filename = "images/tools/black_queen.bmp";
		break;
	case BLACK_R:
		filename = "images/tools/black_rook.bmp";
		break;
	default:
		filename = NULL;
	}
	return filename;
}

char * getToolName(char tool)
{
	char * name;
	switch (tool)
	{
	case WHITE_K:
		name = "king";
		break;
	case WHITE_B:
		name = "bishop";
		break;
	case WHITE_P:
		name = "pawn";
		break;
	case WHITE_N:
		name = "knight";
		break;
	case WHITE_Q:
		name = "queen";
		break;
	case WHITE_R:
		name = "rook";
		break;
	case BLACK_K:
		name = "king";
		break;
	case BLACK_B:
		name = "bishop";
		break;
	case BLACK_P:
		name = "pawn";
		break;
	case BLACK_N:
		name = "knight";
		break;
	case BLACK_Q:
		name = "queen";
		break;
	case BLACK_R:
		name = "rook";
		break;
	default:
		name = NULL;
	}
	return name;
}
void drawBoard(char board[BOARD_SIZE][BOARD_SIZE], UINode * root)
{
	UINode * panel = root->children[0];
	Window * win = (Window * ) root->control;
	Uint32 green = SDL_MapRGB(win->surface->format, 0, 255, 0);
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			UINode * father = panel->children[j*BOARD_SIZE + i];
			/*if (getFilenameByTool(board[i][j]) == NULL)
			{
				if (father->childsNumber > 0)
				{
					//tool already exists, let's repalce it
					for (int k = 0; k < father->childsNumber; k++)
					{
						freeUINode(father->children[k]);
						father->childsNumber = 0;
					}
				}
				continue;
			}*/

			if (father->childsNumber > 0)
			{
				//tool already exists, let's repalce it
				for (int k = 0; k < father->childsNumber; k++)
				{
					freeUINode(father->children[k]);
					father->childsNumber = 0;
				}
				//father->children[0] = createButtonWithColor(win->surface, 13, 13, getFilenameByTool(board[i][j]), toolClicked, father, 0, "aaaaaaaa", green);
			}
			if (getFilenameByTool(board[i][j]) != NULL)
			{
				addChildToFather(father, createButtonWithColor(win->surface, 13, 13, getFilenameByTool(board[i][j]), toolClicked, father, 0, "aaaaaaaa", green));
			}
		}
	}
}

void triggerClickEvent(UINode * root, int clickedX, int clickedY)
{
	if (root == NULL)
		return; 

	
	for (int k = 0; k < root->childsNumber; k++)
	{
		
		if (root->children[k]->type == BUTTON)
		{
			
			ImgButton * btn = (ImgButton *)root->children[k]->control;
			if (isButtonClicked(*btn, clickedX, clickedY))
			{
				if (root->children[k]->Action != NULL)
					root->children[k]->Action(NULL);
				char* btnName = btn->name;
				//in main windows all bottons functions recieve sourcebtnName

				if (strcmp("cube", btnName) == 0)
				{
					int i = clickedX / 76;
					int j = BOARD_SIZE - (clickedY / 76) -1; 

					Window * win = getRoot(root); //(Window *)boardSettingsWindow->control;
					Uint32 green = SDL_MapRGB(win->surface->format, 0, 255, 0);
					if (boardSettingsWindow != NULL && boardSettingsWindow->control != NULL && win == (Window *)boardSettingsWindow->control)
					{
						UINode * panel = boardSettingsWindow->children[0];
						char * filename = getFilenameByTool(lastChosenTool);

						if (lastChosenTool == NULL)
							continue;

						if (lastChosenTool != EMPTY && checkNewBoardValidation(getColor(lastChosenTool), getToolName(lastChosenTool)) == 0)
						{
							//invalid set 
							continue;
							//todo display error message
						}

						board[i][j] = lastChosenTool;
						drawBoard(board, boardSettingsWindow);

						presentUITree(boardSettingsWindow);
					}
					else if (gameWindow != NULL && gameWindow->control != NULL && win == (Window *)gameWindow->control)
					{
						
						//game window context
						Pos pos;
						pos.x = i;
						pos.y = j;

						if (getColor(board[i][j]) == nextPlayer)
						{
							MoveNode  * moveNode = getMove(board, pos, nextPlayer);
							while (moveNode != NULL)
							{
								int destX = moveNode->move->dest->pos->x;
								int destY = moveNode->move->dest->pos->y;

								UINode * parent = gameWindow->children[0]->children[destY*BOARD_SIZE + destX];
								addChildToFather(parent, createButtonWithColor(win->surface, 0, 0, "images/tools/empty.bmp", NULL, parent, 0, "empty", green));

								moveNode = moveNode->next;
							}
							freeMoveNode(moveNode);
						}
						presentUITree(gameWindow);
						drawBoard(board, gameWindow);
						

						/*Move move;
						Pos pos;
						pos.x = i;
						pos.y = j;
						
						move.currPos = &pos;
						*/
					}
				}
			}
		}
		triggerClickEvent(root->children[k], clickedX, clickedY);
	}
}
void EventsLoopboardSettingWindow()
{
	while (!shouldQuitBoardSeEvents)
	{
		SDL_Event e;
		while (SDL_PollEvent(&e) != 0 && !shouldQuitBoardSeEvents)
		{
			if (e.type == SDL_QUIT)
			{
				shouldQuitBoardSeEvents = 1;
				SDL_Quit();
				exit(0);
			}
			else if (e.type == SDL_MOUSEBUTTONDOWN)
			{
				int x, y;
				SDL_GetMouseState(&x, &y);
				triggerClickEvent(boardSettingsWindow, x, y);
			}
		}
		SDL_Delay(1);
	}
	SDL_Quit();
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
						char* btnName = btn->name;
						//in main windows all bottons functions recieve sourcebtnName
						buttonNodes[i]->Action(btnName);
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
				int x, y;
				SDL_GetMouseState(&x, &y);
				UINode** buttons = playerSelectionWindow->children[0]->children;
				int bottunsNumber = playerSelectionWindow->children[0]->childsNumber;
				for (int i = 0; i < bottunsNumber; i++)
				{
					if (buttons[i]->type != BUTTON)
						continue;
					ImgButton* btn = (ImgButton*)buttons[i]->control;
					if (isButtonClicked(*btn, x, y))
					{
						char* btnName = btn->name;
						//in main windows all bottons functions recieve sourcebtnName
						if (buttons[i]->Action != NULL)
						{
							buttons[i]->Action(NULL);
						}							
					}
				}
			}

		}
		SDL_Delay(1);
	}
	SDL_Quit();
}

void EventsLoopSettingWindow()
{
	while (!shouldQuitsettingEvents)
	{
		SDL_Event e;
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				shouldQuitsettingEvents = 1;
				SDL_Quit();
				exit(0);
			}
			else if (e.type == SDL_MOUSEBUTTONUP)
			{
				int x, y;
				SDL_GetMouseState(&x, &y);
				UINode** buttons = settingWindow->children[0]->children;
				int bottunsNumber = settingWindow->children[0]->childsNumber;
				for (int i = 0; i < bottunsNumber; i++)
				{
					if (buttons[i]->type != BUTTON)
						continue;
					ImgButton* btn = (ImgButton*)buttons[i]->control;
					if (isButtonClicked(*btn, x, y))
					{
						char* btnName = btn->name;
						//in main windows all bottons functions recieve sourcebtnName
						if (buttons[i]->Action != NULL)
						{
							buttons[i]->Action(NULL);
						}
					}
				}
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
				triggerClickEvent(gameWindow, x, y);
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
#include "gui.h"

#define malloc(x) myMalloc(x)
#define free(x) myFree(x)
#define calloc(x,y) myCalloc(x,y)
#define realloc(x,y) myRealloc(x,y)

/*creating the uitree for the main window*/
void CreateMainWindow()
{
	mainWindow = CreateWindow("Chess", WIN_WIDTH, WIN_HEIGHT, 0,NULL);

	Window* win = (Window*)mainWindow->control;
	Uint32 clearColor = SDL_MapRGB(win->surface->format, 255, 255, 255);
	UINode* mainPanel = CreatePanel(win->surface, 0, 0, WIN_WIDTH, WIN_HEIGHT, clearColor, mainWindow, 0);
	UINode* newGameBtn = CreateButton(win->surface, 315, 150, "images/NewGame.bmp", startNewGame, mainPanel, 0, "newGame");
	UINode* loadGameBtn = CreateButton(win->surface, 315, 230, "images/LoadGame.bmp", NULL, mainPanel, 0,"loadGame");
	UINode* quitGameBtn = CreateButton(win->surface, 315, 310, "images/Quit.bmp", quitGame, mainPanel, 0, "quit");
	UINode* logoBtn = CreateButton(win->surface, 100, 50, "images/chesslogo.bmp", NULL, mainPanel, 0,"logo");

	//adding children
	addChildToFather(mainWindow, mainPanel);
	addChildToFather(mainPanel, newGameBtn);
	addChildToFather(mainPanel, loadGameBtn);
	addChildToFather(mainPanel, quitGameBtn);
	addChildToFather(mainPanel, logoBtn);
	
}

void CreateGameWindow()
{
	//set white background
	SDL_Rect screenRect;
	screenRect.x = screenRect.y = 0;
	screenRect.w = WIN_WIDTH;
	screenRect.h = WIN_HEIGHT;
	gameWindow = CreateWindow("Chess New Game", WIN_WIDTH, WIN_HEIGHT, 0, NULL);
	Window* win = (Window*)gameWindow->control;
	UINode* leftPanel = CreatePanel(win->surface, 600, 0, 200, WIN_HEIGHT, SDL_MapRGB(win->surface->format, 255, 255, 255), gameWindow, 0);

	Panel* p = (Panel*)leftPanel->control;
	int x = p->width / 2 - 170 / 2;
	UINode* saveGameBtn = CreateButton(win->surface,620, 50, "images/saveGame.bmp", NULL, leftPanel, 0,"saveGame");
	UINode* mainMenuBtn = CreateButton(win->surface, 620, 100, "images/mainMenu.bmp", NULL, leftPanel, 0, "mainMenu");
	UINode* quitBtn = CreateButton(win->surface, 620, 500, "images/Quit.bmp", quitGame, leftPanel, 0, "quit");

	UINode* gamePanel = CreatePanel(win->surface, 0, 0, 600, WIN_HEIGHT, SDL_MapRGB(win->surface->format, 0, 0, 0), gameWindow, 0);
	addChildToFather(gameWindow, leftPanel);
	addChildToFather(gameWindow, gamePanel);

	addChildToFather(leftPanel, saveGameBtn);
	addChildToFather(leftPanel, mainMenuBtn);
	addChildToFather(leftPanel, quitBtn);

	//adding board cells:
	UINode* gameBtn;
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			char* filename = (j % 2 == 0 && i % 2 == 0) || (j % 2 == 1 && i % 2 == 1) ? "images/lightRect.bmp" : "images/darkRect.bmp";
			gameBtn = CreateButton(win->surface, j * 76, i * 76, filename, NULL, gamePanel, 0, "boardBtn");
			addChildToFather(gamePanel, gameBtn);
			
		}
	}

}
/*create gameWindow and present it*/
void startNewGame()
{
	CreateGameWindow();
	presentUITree(gameWindow);
	//call Events loop for new game
	EventsLoopGameWindow();

	shouldQuitMainEvents = 1;	
}
void openSettingWindow()
{
	//create setting window
}

void openPlayerSelectionWindow()
{
	playerSelectionWindow = CreateWindow("Chess Players Selection", WIN_WIDTH, WIN_HEIGHT, 0, NULL);
}



void quitGame()
{
	//free resurces

	if (mainWindow != NULL)
		freeUINode(mainWindow);
	if (gameWindow != NULL)
		freeUINode(gameWindow);
	if (settingWindow != NULL)
		freeUINode(settingWindow);
	if (playerSelectionWindow != NULL)
		freeUINode(playerSelectionWindow);
	shouldQuitMainEvents = 1;
	SDL_Quit();
	exit(0);
}


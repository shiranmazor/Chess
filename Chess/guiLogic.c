#include "gui.h"

#define malloc(x) myMalloc(x)
#define free(x) myFree(x)
#define calloc(x,y) myCalloc(x,y)
#define realloc(x,y) myRealloc(x,y)

/*creating the uitree for the main window*/
void CreateMainWindow()
{
	shouldQuitMainEvents = 0;
	mainWindow = CreateWindow("Chess", WIN_WIDTH, WIN_HEIGHT, 0,NULL);

	Window* win = (Window*)mainWindow->control;
	Uint32 clearColor = SDL_MapRGB(win->surface->format, 255, 255, 255);
	UINode* mainPanel = CreatePanel(win->surface, 0, 0, WIN_WIDTH, WIN_HEIGHT, clearColor, mainWindow, 0,"mainPanel");
	UINode* newGameBtn = CreateButton(win->surface, 315, 150, "images/NewGame.bmp",openPlayerSelectionWindow, mainPanel, 0, "newGame");
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
	UINode* leftPanel = CreatePanel(win->surface, 600, 0, 200, WIN_HEIGHT, SDL_MapRGB(win->surface->format, 255, 255, 255), gameWindow, 0,"leftPanel");

	Panel* p = (Panel*)leftPanel->control;
	int x = p->width / 2 - 170 / 2;
	UINode* saveGameBtn = CreateButton(win->surface,620, 50, "images/saveGame.bmp", NULL, leftPanel, 0,"saveGame");
	UINode* mainMenuBtn = CreateButton(win->surface, 620, 100, "images/mainMenu.bmp", NULL, leftPanel, 0, "mainMenu");
	UINode* quitBtn = CreateButton(win->surface, 620, 500, "images/Quit.bmp", quitGame, leftPanel, 0, "quit");

	UINode* gamePanel = CreatePanel(win->surface, 0, 0, 600, WIN_HEIGHT, SDL_MapRGB(win->surface->format, 0, 0, 0), gameWindow, 0,"gamePanel");
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
void cancelPlayerSelection()
{
	//return to main window

	clear_board();
	shouldQuitSelectionEvents = 1;
	//clean resources
	freeUINode(playerSelectionWindow);
	CreateMainWindow();
	presentUITree(mainWindow);
	EventsLoopMainWindow();
	
}
void twoPlayerMode()
{
	UINode* father = getNodeByName("selectPanel", playerSelectionWindow);
	//check if comp players button is marked and change him to unmark
	UINode* markNodeBtn = getNodeByName("twoPlayersH", playerSelectionWindow);
	if (markNodeBtn != NULL)
	{
		Window* win = (Window*)playerSelectionWindow->control;
		ImgButton* oldBtn = (ImgButton*)markNodeBtn->control;
		UINode* regularBtn = CreateButton(win->surface, oldBtn->x, oldBtn->y, "images/PlayersSelection/playComp.bmp", playerVsComputerMode, markNodeBtn->father, 0, "compPlayers");
		replaceUINodeChild(father, regularBtn, "twoPlayersH");
	}
	gameMode = 1;
	//update the bottom to be in highlight color
	UINode* buttonNode = getNodeByName("twoPlayers", playerSelectionWindow);
	//change the node with new control and load him
	Window* win = (Window*)playerSelectionWindow->control;
	ImgButton* oldBtn = (ImgButton*)buttonNode->control;
	UINode* coloredBtnNode = CreateButton(win->surface, oldBtn->x, oldBtn->y, "images/PlayersSelection/twoplay.bmp", NULL, buttonNode->father, 0, "twoPlayersmark");
	
	
	replaceUINodeChild(father, coloredBtnNode, "twoPlayers");
	presentUITree(playerSelectionWindow);
}

void playerVsComputerMode()
{
	UINode* father = getNodeByName("selectPanel", playerSelectionWindow);
	//check if two players button is marked and change him to unmark
	UINode* markNodeBtn = getNodeByName("twoPlayersmark", playerSelectionWindow);
	if (markNodeBtn != NULL)
	{
		Window* win = (Window*)playerSelectionWindow->control;
		ImgButton* oldBtn = (ImgButton*)markNodeBtn->control;
		UINode* regularBtn = CreateButton(win->surface, oldBtn->x, oldBtn->y, "images/PlayersSelection/twoPlayers.bmp", twoPlayerMode, markNodeBtn->father, 0, "twoPlayers");
		replaceUINodeChild(father, regularBtn, "twoPlayersmark");
	}

	gameMode = 2;
	UINode* buttonNode = getNodeByName("compPlayers", playerSelectionWindow);
	//change the node with new control and load him
	Window* win = (Window*)playerSelectionWindow->control;
	ImgButton* oldBtn = (ImgButton*)buttonNode->control;
	UINode* coloredBtnNode = CreateButton(win->surface, oldBtn->x, oldBtn->y, "images/PlayersSelection/playComH.bmp", NULL, buttonNode->father, 0, "twoPlayersH");

	
	replaceUINodeChild(father, coloredBtnNode, "compPlayers");
	presentUITree(playerSelectionWindow);
}
void openPlayerSelectionWindow(void* sourceBottomName)
{
	//clear mainWindow tree
	freeUINode(mainWindow);
	char* sourceBtnName = (char*)sourceBottomName;
	//init or load the game configuration
	if (strcmp(sourceBtnName, "newGame") == 0)
	{
		//set default values:
		minimax_depth = 1;
		nextPlayer = WHITE;
		pawnPromotionTool = -1000;//queen
		init_board(board);
	}
	else if (strcmp(sourceBtnName, "loadGame") == 0)
	{
		//Todo: load new game to board, wating for niro
	}
	//move to the next window
	//create window
	shouldQuitSelectionEvents = 0;
	playerSelectionWindow = CreateWindow("Chess Players Selection", WIN_WIDTH, WIN_HEIGHT, 0, NULL);
	Window* win = (Window*)playerSelectionWindow->control;
	UINode* selectPanel = CreatePanel(win->surface, 0, 0, WIN_WIDTH, WIN_HEIGHT, SDL_MapRGB(win->surface->format, 255, 255, 255), playerSelectionWindow, 0,"selectPanel");

	Panel* p = (Panel*)selectPanel->control;
	//create labels and buttons
	UINode* gameModeLabel = createLabel(win->surface, 50, 150, "images/PlayersSelection/gameMode.bmp", selectPanel, "gameMode");
	UINode* titleLabel = createLabel(win->surface, 220, 50, "images/PlayersSelection/title1.bmp", selectPanel, "title1");
	UINode* nextPlayerLabel = createLabel(win->surface, 50, 250, "images/PlayersSelection/nextPlayer.bmp", selectPanel, "nextPlayer");
	UINode* cancelBtn = CreateButton(win->surface, 150, 400, "images/PlayersSelection/cancel.bmp", cancelPlayerSelection, selectPanel, 0, "Cancel");
	UINode* BoardSettingsBtn = CreateButton(win->surface, 352, 400, "images/PlayersSelection/boardSettings.bmp", NULL, selectPanel, 0, "boardSet");
	UINode* NextBtn = CreateButton(win->surface, 633, 400, "images/PlayersSelection/next.bmp", NULL, selectPanel, 0, "next");

	UINode* twoPlayersBtn = CreateButton(win->surface, 250, 150, "images/PlayersSelection/twoPlayers.bmp", twoPlayerMode, selectPanel, 0, "twoPlayers");
	UINode* compPlayersBtn = CreateButton(win->surface, 500, 150, "images/PlayersSelection/playComp.bmp", playerVsComputerMode, selectPanel, 0, "compPlayers");
	UINode* whiteBtn = CreateButton(win->surface, 250, 250, "images/PlayersSelection/white.bmp", NULL, selectPanel, 0, "white");
	UINode* blackBtn = CreateButton(win->surface, 350, 250, "images/PlayersSelection/black.bmp", NULL, selectPanel, 0, "black");

	//labels:
	UINode* defaultColLabel = createLabel(win->surface, 450, 245, "images/PlayersSelection/default.bmp", selectPanel, "whitetext");

	addChildToFather(playerSelectionWindow, selectPanel);
	addChildToFather(selectPanel, titleLabel);
	addChildToFather(selectPanel, gameModeLabel);
	addChildToFather(selectPanel, twoPlayersBtn);
	addChildToFather(selectPanel, compPlayersBtn);

	addChildToFather(selectPanel, nextPlayerLabel);
	addChildToFather(selectPanel, whiteBtn);
	addChildToFather(selectPanel, blackBtn);
	addChildToFather(selectPanel, defaultColLabel);
	addChildToFather(selectPanel, cancelBtn);
	addChildToFather(selectPanel, BoardSettingsBtn);
	addChildToFather(selectPanel, NextBtn);

	presentUITree(playerSelectionWindow);
	EventsLoopPlayerSelectionWindow();
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

#include "gui.h"

#define malloc(x) myMalloc(x)
#define free(x) myFree(x)
#define calloc(x,y) myCalloc(x,y)
#define realloc(x,y) myRealloc(x,y)

/*creating the uitree for the main window*/
void CreateMainWindow()
{

	mainWindow = CreateWindow("Chess", WIN_WIDTH, WIN_HEIGHT, 0, NULL, 255,255,255);

	Window* win = (Window*)mainWindow->control;
	Uint32 clearColor = SDL_MapRGB(win->surface->format, 255, 255, 255);
	UINode* mainPanel = CreatePanel(win->surface, 0, 0, WIN_WIDTH, WIN_HEIGHT, clearColor, mainWindow, 0,"mainPanel");
	UINode* newGameBtn = CreateButton(win->surface, 315, 150, "images/NewGame.bmp",openPlayerSelectionWindow, mainPanel, 0, "newGame");
	UINode* loadGameBtn = CreateButton(win->surface, 315, 230, "images/LoadGame.bmp", loadGame, mainPanel, 0,"loadGame");
	UINode* quitGameBtn = CreateButton(win->surface, 315, 310, "images/Quit.bmp", quitGame, mainPanel, 0, "quit");
	UINode* logoBtn = CreateButton(win->surface, 100, 50, "images/chesslogo.bmp", NULL, mainPanel, 0,"logo");

	//adding children
	addChildToFather(mainWindow, mainPanel);
	addChildToFather(mainPanel, newGameBtn);
	addChildToFather(mainPanel, loadGameBtn);
	addChildToFather(mainPanel, quitGameBtn);
	addChildToFather(mainPanel, logoBtn);
	ActiveWindow = mainWindow;
	
}
void doNothing()
{
}

void addBoardToPanel(UINode* gamePanel, Window *win)
{
	//adding board cells:
	UINode* gameBtn;
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			char* filename = (j % 2 == 0 && i % 2 == 0) || (j % 2 == 1 && i % 2 == 1) ? "images/lightRect.bmp" : "images/darkRect.bmp";
			//char btnName1[11];
			//sprintf(btnName1, "cube_%d:%d", i, j);
			//char * btnName = "1234567891011";
			Panel * p = (Panel *) gamePanel->control;
			gameBtn = CreateButton(win->surface,  j * 76, p->height - i * 76 - 76, filename, doNothing, gamePanel, 0, "cube");
			addChildToFather(gamePanel, gameBtn);
		}
	}
}

void goToMainMenu()
{
	clear_board();
	nextPlayer = WHITE;
	
	CreateMainWindow();
	ActiveWindow = mainWindow;
	presentUITree(mainWindow);
	
}

void showBestMove()
{
	if (isGameOver == 1)
		return;

	drawBoard(board,gameWindow);
	if (gameMode == 2)
	{
		//player vs AI
		Move * bestMove = get_best_move(nextPlayer, minimax_depth);
		Window * win = (Window *)gameWindow->control;
		Uint32 green = SDL_MapRGB(win->surface->format, 0, 255, 0);
		int destY = bestMove->dest->pos->y;
		int destX = bestMove->dest->pos->x;
		UINode * parent = gameWindow->children[0]->children[destY*BOARD_SIZE + destX];
		addChildToFather(parent, createButtonWithColor(win->surface, 0, 0, "images/tools/empty.bmp", NULL, parent, 0, "bestMoveEmpty", green));

		int fromX = bestMove->currPos->x;
		int fromY = bestMove->currPos->y;

		parent = gameWindow->children[0]->children[fromY*BOARD_SIZE + fromX];
		addChildToFather(parent, createButtonWithColor(win->surface, 0, 0, "images/tools/empty.bmp", NULL, parent, 0, "bestMoveEmpty", green));

		freeMove(bestMove);
		presentUITree(gameWindow);
	}
	else if (gameMode == 1)
	{
		showDepthOptions();
	}
	
	
}
void saveGameFromSlot(void* name)
{
	char* slotName = (char*)name;
	int slotNum = atoi(slotName);
	GameStatus status;
	copyBoard(board, status.board);
	status.userColor = userColor;
	status.gameMode = gameMode;
	status.nextTurn = nextPlayer;
	status.difficulty = minimax_depth;
	saveFileWithSlotNumber(status, slotNum);
	isGameOver = 0;
	freeUINode(savePanel);
	ActiveWindow = gameWindow;
	presentUITree(ActiveWindow);
}

void showBestMoveByDepth(void* name)
{
	char* depth = (char*)name;
	int depthInt = atoi(depth);
	
	Move *bestMove = get_best_move(nextPlayer, depthInt);

	//draw move
	Window * win = (Window *)gameWindow->control;
	Uint32 green = SDL_MapRGB(win->surface->format, 0, 255, 0);
	int destY = bestMove->dest->pos->y;
	int destX = bestMove->dest->pos->x;
	UINode * parent = gameWindow->children[0]->children[destY*BOARD_SIZE + destX];
	addChildToFather(parent, createButtonWithColor(win->surface, 0, 0, "images/tools/empty.bmp", NULL, parent, 0, "bestMoveEmpty", green));

	int fromX = bestMove->currPos->x;
	int fromY = bestMove->currPos->y;

	parent = gameWindow->children[0]->children[fromY*BOARD_SIZE + fromX];
	addChildToFather(parent, createButtonWithColor(win->surface, 0, 0, "images/tools/empty.bmp", NULL, parent, 0, "bestMoveEmpty", green));

	freeMove(bestMove);

	freeUINode(showDepth);
	showDepth = NULL;
	ActiveWindow = gameWindow;
	presentUITree(ActiveWindow);
	isGameOver = 0;
}

void showDepthOptions()
{
	isGameOver = 1;
	
	Window* win = (Window*)gameWindow->control;

	int color = SDL_MapRGB(win->surface->format, 89, 89, 89);
	CreatePanel(win->surface, 145, 95, 410, 210, color, gameWindow, 0, "borders");
	int color1 = SDL_MapRGB(win->surface->format, 255, 255, 255);
	showDepth = CreatePanel(win->surface, 150, 100, 400, 200, color1, gameWindow, 0, "showDepth");

	Window* win2 = (Window*)showDepth->control;
	UINode* slotNumBtn = createLabel(win2->surface, 100, 10, "images/AISettings/gameDepth.bmp", showDepth, "slotNumSave");
	addChildToFather(showDepth, slotNumBtn);

	UINode* slotNumBtn1 = CreateButton(win->surface, 120, 60, "images/1.bmp", showBestMoveByDepth, showDepth, 0, "1");
	addChildToFather(showDepth, slotNumBtn1);

	UINode* slotNumBtn2 = CreateButton(win->surface, 160, 60, "images/2.bmp", showBestMoveByDepth, showDepth, 0, "2");
	addChildToFather(showDepth, slotNumBtn2);

	UINode* slotNumBtn3 = CreateButton(win->surface, 200, 60, "images/3.bmp", showBestMoveByDepth, showDepth, 0, "3");
	addChildToFather(showDepth, slotNumBtn3);

	UINode* slotNumBtn4 = CreateButton(win->surface, 240, 60, "images/4.bmp", showBestMoveByDepth, showDepth, 0, "4");
	addChildToFather(showDepth, slotNumBtn4);

	presentUITree(showDepth);
	ActiveWindow = showDepth;

}

void saveGame()
{
	isGameOver = 1;
	//open SLOTNUM buttons
	Window* win = (Window*)gameWindow->control;

	int color = SDL_MapRGB(win->surface->format, 89, 89, 89);
	CreatePanel(win->surface, 145, 95, 410, 210, color, gameWindow, 0, "mis");
	int color1 = SDL_MapRGB(win->surface->format, 255, 255, 255);
	savePanel = CreatePanel(win->surface, 150, 100, 400, 200, color1, gameWindow, 0, "saveGamePanel");

	Window* win2 = (Window*)savePanel->control;
	UINode* slotNumBtn = createLabel(win2->surface, 100, 10, "images/slotNum.bmp", savePanel, "slotNumSave");
	addChildToFather(savePanel, slotNumBtn);
	
	if (SLOTS_NUM >= 1)
	{
		UINode* slotNumBtn1 = CreateButton(win->surface, 120, 60, "images/1.bmp", saveGameFromSlot, savePanel, 0, "1");
		addChildToFather(savePanel, slotNumBtn1);
	}
	if (SLOTS_NUM >= 2)
	{
		UINode* slotNumBtn2 = CreateButton(win->surface, 160, 60, "images/2.bmp", saveGameFromSlot, savePanel, 0, "2");
		addChildToFather(savePanel, slotNumBtn2);
	}
	if (SLOTS_NUM >= 3)
	{
		UINode* slotNumBtn3 = CreateButton(win->surface, 200, 60, "images/3.bmp", saveGameFromSlot, savePanel, 0, "3");
		addChildToFather(savePanel, slotNumBtn3);
	}
	if (SLOTS_NUM >= 4)
	{
		UINode* slotNumBtn4 = CreateButton(win->surface, 240, 60, "images/4.bmp", saveGameFromSlot, savePanel, 0, "4");
		addChildToFather(savePanel, slotNumBtn4);
	}
	if (SLOTS_NUM >= 5)
	{
		UINode* slotNumBtn5 = CreateButton(win->surface, 120, 100, "images/5.bmp", saveGameFromSlot, savePanel, 0, "5");
		addChildToFather(savePanel, slotNumBtn5);
	}
	if (SLOTS_NUM >= 6)
	{
		UINode* slotNumBtn6 = CreateButton(win->surface, 160, 100, "images/6.bmp", saveGameFromSlot, savePanel, 0, "6");
		addChildToFather(savePanel, slotNumBtn6);
	}
	if (SLOTS_NUM >= 7)
	{
		UINode* slotNumBtn7 = CreateButton(win->surface, 200, 100, "images/7.bmp", saveGameFromSlot, savePanel, 0, "7");
		addChildToFather(savePanel, slotNumBtn7);
	}
	if (SLOTS_NUM >= 8)
	{
		UINode* slotNumBtn8 = CreateButton(win->surface, 240, 100, "images/8.bmp", saveGameFromSlot, savePanel, 0, "8");
		addChildToFather(savePanel, slotNumBtn8);
	}
	if (SLOTS_NUM >= 9)
	{
		UINode* slotNumBtn9 = CreateButton(win->surface, 120, 140, "images/9.bmp", saveGameFromSlot, savePanel, 0, "9");
		addChildToFather(savePanel, slotNumBtn9);
	}
	if (SLOTS_NUM >= 10)
	{
		UINode* slotNumBtn10 = CreateButton(win->surface, 200, 140, "images/10.bmp", saveGameFromSlot, savePanel, 0, "10");
		addChildToFather(savePanel, slotNumBtn10);
	}

	presentUITree(savePanel);
	ActiveWindow = savePanel;

}

void CreateGameWindow()
{
	isGameOver = 0;
	//set white background
	SDL_Rect screenRect;
	screenRect.x = screenRect.y = 0;
	screenRect.w = WIN_WIDTH;
	screenRect.h = WIN_HEIGHT;
	gameWindow = CreateWindow("Chess New Game", WIN_WIDTH, WIN_HEIGHT, 0, NULL, 255, 255, 255);
	Window* win = (Window*)gameWindow->control;
	
	UINode* gamePanel = CreatePanel(win->surface, 0, 0, 600, WIN_HEIGHT, SDL_MapRGB(win->surface->format, 0, 0, 0), gameWindow, 0, "gamePanel");
	addChildToFather(gameWindow, gamePanel);
	addBoardToPanel(gamePanel, win);


	UINode* leftPanel = CreatePanel(win->surface, 600, 0, 200, WIN_HEIGHT, SDL_MapRGB(win->surface->format, 255, 255, 255), gameWindow, 0,"leftPanel");
	addChildToFather(gameWindow, leftPanel);
	//Panel* p = (Panel*)leftPanel->control;
	//int x = p->width / 2 - 170 / 2;
	UINode* saveGameBtn = CreateButton(win->surface, 20, 50, "images/saveGame.bmp", saveGame, leftPanel, 0, "saveGame");
	UINode* mainMenuBtn = CreateButton(win->surface, 20, 100, "images/mainMenu.bmp", goToMainMenu, leftPanel, 0, "mainMenu");
	UINode* bestMove = CreateButton(win->surface, 20, 200, "images/bestMove.bmp", showBestMove, leftPanel, 0, "bestMove");
	UINode* quitBtn = CreateButton(win->surface, 20, 500, "images/Quit.bmp", quitGame, leftPanel, 0, "quit");

	addChildToFather(leftPanel, saveGameBtn);
	addChildToFather(leftPanel, mainMenuBtn);
	addChildToFather(leftPanel, bestMove);
	addChildToFather(leftPanel, quitBtn);
}


void loadGameFromSlot(void* name)
{
	char* slotName = (char*)name;
	int slotNum = atoi(slotName);
	GameStatus gStatus = readFileWithSlotNumber(slotNum);
	//load parameters to board
	gameMode = gStatus.gameMode;
	copyBoard(gStatus.board,board);
	userColor = gStatus.userColor;
	minimax_depth = gStatus.difficulty;
	nextPlayer = gStatus.nextTurn;


	openPlayerSelectionWindow("loadGame");


}
char* getBtnName(int i)
{
	if (i == 1)
		return "1";
	if (i == 2)
		return "2";
	if (i == 3)
		return "3";
	if (i == 4)
		return "4";
	if (i == 5)
		return "5";
	if (i == 6)
		return "6";
	if (i == 7)
		return "7";
	if (i == 8)
		return "8";
	if (i == 9)
		return "9";
	if (i == 10)
		return "10";
	return "1";

}
void loadGame()
{

	int slotArr[SLOTS_NUM];//init to 1
	//find all FILENAME files in the project dir
	char filename[16];
	for (int k = 0; k < SLOTS_NUM; k++)
	{
		int num = k + 1;
		sprintf(filename, "%s%d.xml", FILENAME, num);
		if (fileExists(filename) == 1)
		{
			slotArr[k] = 1;
		}
			
		else
			slotArr[k] = 0;
	}
	Window* win = (Window*)mainWindow->control;
	UINode* slotNumBtn = createLabel(win->surface, 550, 120, "images/slotNum.bmp", mainWindow->children[0], "slotNum");
	
	int x = 600;
	int x2 = 650;
	int y = 170;
	int lasty = y;
	addChildToFather(mainWindow->children[0], slotNumBtn);
	int numBtn = 0;
	for (int i = 1; i <= SLOTS_NUM; i++)
	{

		int xpos = 0;
		int ypos = 0;
		if (slotArr[i - 1] == 0)
			continue;
		numBtn++;
		char imageName[20];
		char* btnName = getBtnName(i);
		sprintf(imageName, "%s%d.bmp","images/", i);
		if (numBtn % 2 == 0)
		{
			xpos = x2;
			ypos = lasty;
			
		}
		else
		{
			xpos = x;
			ypos = y;
			lasty = y;
		}
			
		UINode* slotNumBtn = CreateButton(win->surface, xpos, ypos, imageName, loadGameFromSlot, mainWindow->children[0], 0, btnName);
		addChildToFather(mainWindow->children[0], slotNumBtn);
		y = y + 20;
	}
	presentUITree(mainWindow);
	ActiveWindow = mainWindow;

}
/*create gameWindow and present it*/
void startNewGame()
{
	CreateGameWindow();
	drawBoard(board, gameWindow);
	presentUITree(gameWindow);
	
	//call Events loop for new game
	ActiveWindow = gameWindow;
	

	checkAndDeclareGameStatus(nextPlayer);
	if (gameMode == 2 && nextPlayer == computerColor)
	{
		ComputerMove();

		checkAndDeclareGameStatus(nextPlayer);
		drawBoard(board, gameWindow);
		presentUITree(gameWindow);
	}
}
void chooseBlackColor()
{
	userColor = BLACK;
	computerColor = WHITE;
	UINode* father = getNodeByName("AISettingPanel", settingWindow);
	//check if comp players button is marked and change him to unmark
	UINode* markNodeBtn = getNodeByName("whiteMark", settingWindow);
	if (markNodeBtn != NULL)
	{
		Window* win = (Window*)settingWindow->control;
		ImgButton* oldBtn = (ImgButton*)markNodeBtn->control;
		UINode* regularBtn = CreateButton(win->surface, oldBtn->x, oldBtn->y, "images/PlayersSelection/white.bmp", chooseWhiteColor, markNodeBtn->father, 0, "white");
		replaceUINodeChild(father, regularBtn, "whiteMark");
	}
	//update the bottom to be in highlight color
	UINode* buttonNode = getNodeByName("black", settingWindow);
	//change the node with new control and load him
	Window* win = (Window*)settingWindow->control;
	ImgButton* oldBtn = (ImgButton*)buttonNode->control;
	UINode* coloredBtnNode = CreateButton(win->surface, oldBtn->x, oldBtn->y, "images/PlayersSelection/blackMark.bmp", NULL, buttonNode->father, 0, "blackMark");
	replaceUINodeChild(father, coloredBtnNode, "black");
	presentUITree(settingWindow);
}
void chooseWhiteColor()
{
	userColor = WHITE;
	computerColor = BLACK;
	Window* win;
	ImgButton* oldBtn;
	userColor = WHITE;
	UINode* coloredBtnNode;
	UINode* father = getNodeByName("AISettingPanel", settingWindow);
	//check if comp players button is marked and change him to unmark
	UINode* markNodeBtn = getNodeByName("blackMark", settingWindow);
	if (markNodeBtn != NULL)
	{
		Window* win = (Window*)settingWindow->control;
		ImgButton* oldBtn = (ImgButton*)markNodeBtn->control;
		UINode* regularBtn = CreateButton(win->surface, oldBtn->x, oldBtn->y, "images/PlayersSelection/black.bmp", chooseBlackColor, markNodeBtn->father, 0, "black");
		replaceUINodeChild(father, regularBtn, "blackMark");
	}
	//update the bottom to be in highlight color
	UINode* buttonNode = getNodeByName("white", settingWindow);
	if (buttonNode == NULL)
		return;
	//change the node with new control and load him
	win = (Window*)settingWindow->control;
	oldBtn = (ImgButton*)buttonNode->control;
	coloredBtnNode = CreateButton(win->surface, oldBtn->x, oldBtn->y, "images/PlayersSelection/whiteMark.bmp", NULL, buttonNode->father, 0, "whiteMark");
	replaceUINodeChild(father, coloredBtnNode, "white");
	presentUITree(settingWindow);
}

void chooseDepth1()
{
	Window* win;
	ImgButton* oldBtn;
	minimax_depth = 1;
	UINode* coloredBtnNode;
	UINode* father = getNodeByName("AISettingPanel", settingWindow);
	//check if comp players button is marked and change him to unmark
	UINode* markNodeBtn2 = getNodeByName("depth2Mark", settingWindow);
	UINode* markNodeBtn3 = getNodeByName("depth3Mark", settingWindow);
	UINode* markNodeBtn4 = getNodeByName("depth4Mark", settingWindow);
	UINode* markNodeBest = getNodeByName("depthBestMark", settingWindow);
	
	if (markNodeBtn2 != NULL)
	{
		Window* win = (Window*)settingWindow->control;
		ImgButton* oldBtn = (ImgButton*)markNodeBtn2->control;
		UINode* regularBtn = CreateButton(win->surface, oldBtn->x, oldBtn->y, "images/AISettings/2.bmp", chooseDepth2, markNodeBtn2->father, 0, "depth2");
		replaceUINodeChild(father, regularBtn, "depth2Mark");
	}
	if (markNodeBtn3 != NULL)
	{
		Window* win = (Window*)settingWindow->control;
		ImgButton* oldBtn = (ImgButton*)markNodeBtn3->control;
		UINode* regularBtn = CreateButton(win->surface, oldBtn->x, oldBtn->y, "images/AISettings/3.bmp", chooseDepth3, markNodeBtn3->father, 0, "depth3");
		replaceUINodeChild(father, regularBtn, "depth3Mark");
	}
	if (markNodeBtn4 != NULL)
	{
		Window* win = (Window*)settingWindow->control;
		ImgButton* oldBtn = (ImgButton*)markNodeBtn4->control;
		UINode* regularBtn = CreateButton(win->surface, oldBtn->x, oldBtn->y, "images/AISettings/4.bmp", chooseDepth4, markNodeBtn4->father, 0, "depth4");
		replaceUINodeChild(father, regularBtn, "depth4Mark");
	}
	if (markNodeBest!= NULL)
	{
		Window* win = (Window*)settingWindow->control;
		ImgButton* oldBtn = (ImgButton*)markNodeBest->control;
		UINode* regularBtn = CreateButton(win->surface, oldBtn->x, oldBtn->y, "images/AISettings/best.bmp", chooseDepthBest, markNodeBest->father, 0, "best");
		replaceUINodeChild(father, regularBtn, "depthBestMark");
	}
	//update the bottom to be in highlight color
	UINode* buttonNode = getNodeByName("depth1", settingWindow);
	if (buttonNode == NULL)
		return;
	//change the node with new control and load him
	win = (Window*)settingWindow->control;
	oldBtn = (ImgButton*)buttonNode->control;
	coloredBtnNode = CreateButton(win->surface, oldBtn->x, oldBtn->y, "images/AISettings/1m.bmp", chooseDepth1, buttonNode->father, 0, "depth1Mark");
	replaceUINodeChild(father, coloredBtnNode, "depth1");
	presentUITree(settingWindow);
}
void chooseDepth2()
{
	Window* win;
	ImgButton* oldBtn;
	minimax_depth = 2;
	UINode* coloredBtnNode;
	UINode* father = getNodeByName("AISettingPanel", settingWindow);
	//check if comp players button is marked and change him to unmark
	UINode* markNodeBtn1 = getNodeByName("depth1Mark", settingWindow);
	UINode* markNodeBtn3 = getNodeByName("depth3Mark", settingWindow);
	UINode* markNodeBtn4 = getNodeByName("depth4Mark", settingWindow);
	UINode* markNodeBest = getNodeByName("depthBestMark", settingWindow);

	if (markNodeBtn1 != NULL)
	{
		Window* win = (Window*)settingWindow->control;
		ImgButton* oldBtn = (ImgButton*)markNodeBtn1->control;
		UINode* regularBtn = CreateButton(win->surface, oldBtn->x, oldBtn->y, "images/AISettings/1.bmp", chooseDepth1, markNodeBtn1->father, 0, "depth1");
		replaceUINodeChild(father, regularBtn, "depth1Mark");
	}

	if (markNodeBtn3 != NULL)
	{
		Window* win = (Window*)settingWindow->control;
		ImgButton* oldBtn = (ImgButton*)markNodeBtn3->control;
		UINode* regularBtn = CreateButton(win->surface, oldBtn->x, oldBtn->y, "images/AISettings/3.bmp", chooseDepth3, markNodeBtn3->father, 0, "depth3");
		replaceUINodeChild(father, regularBtn, "depth3Mark");
	}
	if (markNodeBtn4 != NULL)
	{
		Window* win = (Window*)settingWindow->control;
		ImgButton* oldBtn = (ImgButton*)markNodeBtn4->control;
		UINode* regularBtn = CreateButton(win->surface, oldBtn->x, oldBtn->y, "images/AISettings/4.bmp", chooseDepth4, markNodeBtn4->father, 0, "depth4");
		replaceUINodeChild(father, regularBtn, "depth4Mark");
	}
	if (markNodeBest != NULL)
	{
		Window* win = (Window*)settingWindow->control;
		ImgButton* oldBtn = (ImgButton*)markNodeBest->control;
		UINode* regularBtn = CreateButton(win->surface, oldBtn->x, oldBtn->y, "images/AISettings/best.bmp", chooseDepthBest, markNodeBest->father, 0, "best");
		replaceUINodeChild(father, regularBtn, "depthBestMark");
	}
	//update the bottom to be in highlight color
	UINode* buttonNode = getNodeByName("depth2", settingWindow);
	if (buttonNode == NULL)
		return;
	//change the node with new control and load him
	win = (Window*)settingWindow->control;
	oldBtn = (ImgButton*)buttonNode->control;
	coloredBtnNode = CreateButton(win->surface, oldBtn->x, oldBtn->y, "images/AISettings/2m.bmp", chooseDepth2, buttonNode->father, 0, "depth2Mark");
	replaceUINodeChild(father, coloredBtnNode, "depth2");
	presentUITree(settingWindow);
}
void chooseDepth3()
{
	Window* win;
	ImgButton* oldBtn;
	minimax_depth = 3;
	UINode* coloredBtnNode;
	UINode* father = getNodeByName("AISettingPanel", settingWindow);
	//check if comp players button is marked and change him to unmark
	UINode* markNodeBtn1 = getNodeByName("depth1Mark", settingWindow);
	UINode* markNodeBtn2 = getNodeByName("depth2Mark", settingWindow);
	UINode* markNodeBtn4 = getNodeByName("depth4Mark", settingWindow);
	UINode* markNodeBest = getNodeByName("depthBestMark", settingWindow);

	if (markNodeBtn1 != NULL)
	{
		Window* win = (Window*)settingWindow->control;
		ImgButton* oldBtn = (ImgButton*)markNodeBtn1->control;
		UINode* regularBtn = CreateButton(win->surface, oldBtn->x, oldBtn->y, "images/AISettings/1.bmp", chooseDepth1, markNodeBtn1->father, 0, "depth1");
		replaceUINodeChild(father, regularBtn, "depth1Mark");
	}

	if (markNodeBtn2 != NULL)
	{
		Window* win = (Window*)settingWindow->control;
		ImgButton* oldBtn = (ImgButton*)markNodeBtn2->control;
		UINode* regularBtn = CreateButton(win->surface, oldBtn->x, oldBtn->y, "images/AISettings/2.bmp", chooseDepth2, markNodeBtn2->father, 0, "depth2");
		replaceUINodeChild(father, regularBtn, "depth2Mark");
	}
	if (markNodeBtn4 != NULL)
	{
		Window* win = (Window*)settingWindow->control;
		ImgButton* oldBtn = (ImgButton*)markNodeBtn4->control;
		UINode* regularBtn = CreateButton(win->surface, oldBtn->x, oldBtn->y, "images/AISettings/4.bmp", chooseDepth4, markNodeBtn4->father, 0, "depth4");
		replaceUINodeChild(father, regularBtn, "depth4Mark");
	}
	if (markNodeBest != NULL)
	{
		Window* win = (Window*)settingWindow->control;
		ImgButton* oldBtn = (ImgButton*)markNodeBest->control;
		UINode* regularBtn = CreateButton(win->surface, oldBtn->x, oldBtn->y, "images/AISettings/best.bmp", chooseDepthBest, markNodeBest->father, 0, "best");
		replaceUINodeChild(father, regularBtn, "depthBestMark");
	}
	//update the bottom to be in highlight color
	UINode* buttonNode = getNodeByName("depth3", settingWindow);
	if (buttonNode == NULL)
		return;
	//change the node with new control and load him
	win = (Window*)settingWindow->control;
	oldBtn = (ImgButton*)buttonNode->control;
	coloredBtnNode = CreateButton(win->surface, oldBtn->x, oldBtn->y, "images/AISettings/3m.bmp", chooseDepth3, buttonNode->father, 0, "depth3Mark");
	replaceUINodeChild(father, coloredBtnNode, "depth3");
	presentUITree(settingWindow);
}
void chooseDepth4()
{
	Window* win;
	ImgButton* oldBtn;
	minimax_depth = 4;
	UINode* coloredBtnNode;
	UINode* father = getNodeByName("AISettingPanel", settingWindow);
	//check if comp players button is marked and change him to unmark
	UINode* markNodeBtn1 = getNodeByName("depth1Mark", settingWindow);
	UINode* markNodeBtn2 = getNodeByName("depth2Mark", settingWindow);
	UINode* markNodeBtn3 = getNodeByName("depth3Mark", settingWindow);
	UINode* markNodeBest = getNodeByName("depthBestMark", settingWindow);

	if (markNodeBtn1 != NULL)
	{
		Window* win = (Window*)settingWindow->control;
		ImgButton* oldBtn = (ImgButton*)markNodeBtn1->control;
		UINode* regularBtn = CreateButton(win->surface, oldBtn->x, oldBtn->y, "images/AISettings/1.bmp", chooseDepth1, markNodeBtn1->father, 0, "depth1");
		replaceUINodeChild(father, regularBtn, "depth1Mark");
	}

	if (markNodeBtn2 != NULL)
	{
		Window* win = (Window*)settingWindow->control;
		ImgButton* oldBtn = (ImgButton*)markNodeBtn2->control;
		UINode* regularBtn = CreateButton(win->surface, oldBtn->x, oldBtn->y, "images/AISettings/2.bmp", chooseDepth2, markNodeBtn2->father, 0, "depth2");
		replaceUINodeChild(father, regularBtn, "depth2Mark");
	}
	if (markNodeBtn3 != NULL)
	{
		Window* win = (Window*)settingWindow->control;
		ImgButton* oldBtn = (ImgButton*)markNodeBtn3->control;
		UINode* regularBtn = CreateButton(win->surface, oldBtn->x, oldBtn->y, "images/AISettings/3.bmp", chooseDepth3, markNodeBtn3->father, 0, "depth3");
		replaceUINodeChild(father, regularBtn, "depth3Mark");
	}
	if (markNodeBest != NULL)
	{
		Window* win = (Window*)settingWindow->control;
		ImgButton* oldBtn = (ImgButton*)markNodeBest->control;
		UINode* regularBtn = CreateButton(win->surface, oldBtn->x, oldBtn->y, "images/AISettings/best.bmp", chooseDepthBest, markNodeBest->father, 0, "best");
		replaceUINodeChild(father, regularBtn, "depthBestMark");
	}
	//update the bottom to be in highlight color
	UINode* buttonNode = getNodeByName("depth4", settingWindow);
	if (buttonNode == NULL)
		return;
	//change the node with new control and load him
	win = (Window*)settingWindow->control;
	oldBtn = (ImgButton*)buttonNode->control;
	coloredBtnNode = CreateButton(win->surface, oldBtn->x, oldBtn->y, "images/AISettings/4m.bmp", chooseDepth3, buttonNode->father, 0, "depth4Mark");
	replaceUINodeChild(father, coloredBtnNode, "depth4");
	presentUITree(settingWindow);
}
void chooseDepthBest()
{
	Window* win;
	ImgButton* oldBtn;
	minimax_depth = 4;
	UINode* coloredBtnNode;
	UINode* father = getNodeByName("AISettingPanel", settingWindow);
	//check if comp players button is marked and change him to unmark
	UINode* markNodeBtn1 = getNodeByName("depth1Mark", settingWindow);
	UINode* markNodeBtn2 = getNodeByName("depth2Mark", settingWindow);
	UINode* markNodeBtn3 = getNodeByName("depth3Mark", settingWindow);
	UINode* markNodeBtn4 = getNodeByName("depth4Mark", settingWindow);

	if (markNodeBtn1 != NULL)
	{
		Window* win = (Window*)settingWindow->control;
		ImgButton* oldBtn = (ImgButton*)markNodeBtn1->control;
		UINode* regularBtn = CreateButton(win->surface, oldBtn->x, oldBtn->y, "images/AISettings/1.bmp", chooseDepth1, markNodeBtn1->father, 0, "depth1");
		replaceUINodeChild(father, regularBtn, "depth1Mark");
	}

	if (markNodeBtn2 != NULL)
	{
		Window* win = (Window*)settingWindow->control;
		ImgButton* oldBtn = (ImgButton*)markNodeBtn2->control;
		UINode* regularBtn = CreateButton(win->surface, oldBtn->x, oldBtn->y, "images/AISettings/2.bmp", chooseDepth2, markNodeBtn2->father, 0, "depth2");
		replaceUINodeChild(father, regularBtn, "depth2Mark");
	}
	if (markNodeBtn3 != NULL)
	{
		Window* win = (Window*)settingWindow->control;
		ImgButton* oldBtn = (ImgButton*)markNodeBtn3->control;
		UINode* regularBtn = CreateButton(win->surface, oldBtn->x, oldBtn->y, "images/AISettings/3.bmp", chooseDepth3, markNodeBtn3->father, 0, "depth3");
		replaceUINodeChild(father, regularBtn, "depth3Mark");
	}
	if (markNodeBtn4 != NULL)
	{
		Window* win = (Window*)settingWindow->control;
		ImgButton* oldBtn = (ImgButton*)markNodeBtn4->control;
		UINode* regularBtn = CreateButton(win->surface, oldBtn->x, oldBtn->y, "images/AISettings/4.bmp", chooseDepth4, markNodeBtn4->father, 0, "depth4");
		replaceUINodeChild(father, regularBtn, "depth4Mark");
	}
	//update the bottom to be in highlight color
	UINode* buttonNode = getNodeByName("best", settingWindow);
	if (buttonNode == NULL)
		return;
	//change the node with new control and load him
	win = (Window*)settingWindow->control;
	oldBtn = (ImgButton*)buttonNode->control;
	coloredBtnNode = CreateButton(win->surface, oldBtn->x, oldBtn->y, "images/AISettings/bestm.bmp", chooseDepthBest, buttonNode->father, 0, "depthBestMark");
	replaceUINodeChild(father, coloredBtnNode, "best");
	presentUITree(settingWindow);
}
void returnFunc()
{
	openPlayerSelectionWindow("None");
}
void openSettingWindow()
{

	char* whiteBtnName = "whiteMark";
	char* blackBtnName = "black";
	char* depth1BtnName = "depth1Mark";
	char* depth2BtnName = "depth2";
	char* depth3BtnName = "depth3";
	char* depth4BtnName = "depth4";
	char* depthBestBtnName = "best";

	char* whiteBtnP = "images/PlayersSelection/whiteMark.bmp";
	char* blackBtnP = "images/PlayersSelection/black.bmp";
	char* depth1BtnP = "images/AISettings/1m.bmp";
	char* depth2BtnP = "images/AISettings/2.bmp";
	char* depth3BtnP = "images/AISettings/3.bmp";
	char* depth4BtnP = "images/AISettings/4.bmp";
	char* depthBestBtnP = "images/AISettings/best.bmp";
	//load real values in case of load game:
	if (userColor == BLACK)
	{
		whiteBtnName = "white";
		blackBtnName = "blackMark";
		whiteBtnP = "images/PlayersSelection/white.bmp";
		blackBtnP = "images/PlayersSelection/blackMark.bmp";

	}
	if (minimax_depth == 2)
	{
		depth1BtnName = "depth1";
		depth2BtnName = "depth2Mark";
		depth3BtnName = "depth3";
		depth4BtnName = "depth4";
		depthBestBtnName = "best";
		depth1BtnP = "images/AISettings/1.bmp";
		depth2BtnP = "images/AISettings/2m.bmp";
		depth3BtnP = "images/AISettings/3.bmp";
		depth4BtnP = "images/AISettings/4.bmp";
		depthBestBtnP = "images/AISettings/best.bmp";
	}
	else if (minimax_depth == 3)
	{
		depth1BtnName = "depth1";
		depth2BtnName = "depth2";
		depth3BtnName = "depth3Mark";
		depth4BtnName = "depth4";
		depthBestBtnName = "best";
		depth1BtnP = "images/AISettings/1.bmp";
		depth2BtnP = "images/AISettings/2.bmp";
		depth3BtnP = "images/AISettings/3m.bmp";
		depth4BtnP = "images/AISettings/4.bmp";
		depthBestBtnP = "images/AISettings/best.bmp";
	}
	else if (minimax_depth == 4)
	{
		depth1BtnName = "depth1";
		depth2BtnName = "depth2";
		depth3BtnName = "depth3";
		depth4BtnName = "depth4Mark";
		depthBestBtnName = "best";
		depth1BtnP = "images/AISettings/1.bmp";
		depth2BtnP = "images/AISettings/2.bmp";
		depth3BtnP = "images/AISettings/3.bmp";
		depth4BtnP = "images/AISettings/4m.bmp";
		depthBestBtnP = "images/AISettings/best.bmp";
	}
	else if (minimax_depth == -1)
	{
		depth1BtnName = "depth1";
		depth2BtnName = "depth2";
		depth3BtnName = "depth3";
		depth4BtnName = "depth4";
		depthBestBtnName = "depthBestMark";
		depth1BtnP = "images/AISettings/1.bmp";
		depth2BtnP = "images/AISettings/2.bmp";
		depth3BtnP = "images/AISettings/3.bmp";
		depth4BtnP = "images/AISettings/4.bmp";
		depthBestBtnP = "images/AISettings/bestm.bmp";
	}

	settingWindow = CreateWindow("Chess Game AI Settings", WIN_WIDTH, WIN_HEIGHT, 0, NULL, 255, 255, 255);
	Window* win = (Window*)settingWindow->control;
	UINode* settingPanel = CreatePanel(win->surface, 0, 0, WIN_WIDTH, WIN_HEIGHT, SDL_MapRGB(win->surface->format, 255, 255, 255), settingWindow, 0, "AISettingPanel");

	UINode* userColorLabel = createLabel(win->surface, 50, 150, "images/AISettings/userColor.bmp", settingPanel, "userColorLabel");
	UINode* titleLabel = createLabel(win->surface, 300, 50, "images/AISettings/title.bmp", settingPanel, "titleLabel");
	UINode* depthLabel = createLabel(win->surface, 50, 250, "images/AISettings/gameDepth.bmp", settingPanel, "depthLabel");
	
	UINode* whiteBtn = CreateButton(win->surface, 250, 150, whiteBtnP, chooseWhiteColor, settingPanel, 0, whiteBtnName);
	UINode* blackBtn = CreateButton(win->surface, 500, 150, blackBtnP, chooseBlackColor, settingPanel, 0, blackBtnName);
	UINode* depth1 = CreateButton(win->surface, 250, 250, depth1BtnP, chooseDepth1, settingPanel, 0, depth1BtnName);
	UINode* depth2 = CreateButton(win->surface, 350, 250, depth2BtnP, chooseDepth2, settingPanel, 0, depth2BtnName);
	UINode* depth3 = CreateButton(win->surface, 450, 250, depth3BtnP, chooseDepth3, settingPanel, 0, depth3BtnName);
	UINode* depth4 = CreateButton(win->surface, 550, 250, depth4BtnP, chooseDepth4, settingPanel, 0, depth4BtnName);
	UINode* depthBest = CreateButton(win->surface, 650, 250, depthBestBtnP, chooseDepthBest, settingPanel, 0, depthBestBtnName);

	UINode* returnBtn = CreateButton(win->surface, 250, 350, "images/AISettings/return.bmp", returnFunc, settingPanel, 0, "return");
	UINode* playBtn = CreateButton(win->surface, 550, 350, "images/AISettings/play.bmp", startNewGame, settingPanel, 0, "play");

	addChildToFather(settingWindow, settingPanel);
	addChildToFather(settingPanel, userColorLabel);
	addChildToFather(settingPanel, depthLabel);
	addChildToFather(settingPanel, titleLabel);
	addChildToFather(settingPanel, whiteBtn);
	addChildToFather(settingPanel, blackBtn);
	addChildToFather(settingPanel, depth1);
	addChildToFather(settingPanel, depth2);
	addChildToFather(settingPanel, depth3);
	addChildToFather(settingPanel, depth4);
	addChildToFather(settingPanel, depthBest);
	addChildToFather(settingPanel, returnBtn);
	addChildToFather(settingPanel, playBtn);

	//add user color label, white and black button
	presentUITree(settingWindow);
	ActiveWindow = settingWindow;
}
void NextButtomClicked()
{
	if (gameMode == 2)//player vs computer
	{
		openSettingWindow();
	}
	else
	{
		startNewGame();
	}
}



void setChosenToolToBlackPawn()
{
	lastChosenTool = BLACK_P;
}

void setChosenToolToWhitePawn()
{
	lastChosenTool = WHITE_P;
}

void setChosenToolToBlackKing()
{
	lastChosenTool = BLACK_K;
}

void setChosenToolToWhiteKing()
{
	lastChosenTool = WHITE_K;
}

void setChosenToolToBlackQueen()
{
	lastChosenTool = BLACK_Q;
}

void setChosenToolToWhiteQueen()
{
	lastChosenTool = WHITE_Q;
}

void setChosenToolToBlackBishop()
{
	lastChosenTool = BLACK_B;
}

void setChosenToolToWhiteBishop()
{
	lastChosenTool = WHITE_B;
}

void setChosenToolToBlackRook()
{
	lastChosenTool = BLACK_R;
}

void setChosenToolToWhiteRook()
{
	lastChosenTool = WHITE_R;
}

void setChosenToolToBlackKnight()
{
	lastChosenTool = BLACK_N;
}

void setChosenToolToWhiteKnight()
{
	lastChosenTool = WHITE_N;
}

void setChosenToolToTrash()
{
	lastChosenTool = EMPTY;
}

void startNewGameIfBoardValid()
{
	if (countKings() < 2)
	{
		//todo display error message
	}
	else
	{
		startNewGame();
	}
}
void clearBoard()
{
	clear_board();
	drawBoard(board, boardSettingsWindow);
	presentUITree(boardSettingsWindow);
}
void openBoardSettingWindow()
{
	boardSettingsWindow = CreateWindow("Chess Board Settings", WIN_WIDTH, WIN_HEIGHT, 0, NULL, 255, 255, 255);
	Window* win = (Window*)boardSettingsWindow->control;

	UINode *boardPanel = CreatePanel(win->surface, 0, 0, 608, 608, 0, boardSettingsWindow, 0, "Board Panel");
	addChildToFather(boardSettingsWindow, boardPanel);
	addBoardToPanel(boardPanel, win);

	UINode *menuPanel = CreatePanel(win->surface, 610, 0, WIN_WIDTH - 572, WIN_HEIGHT, SDL_MapRGB(win->surface->format, 255, 255, 255), boardSettingsWindow, 0, "Menu Panel");
	addChildToFather(boardSettingsWindow, menuPanel);

	Uint32 green = SDL_MapRGB(win->surface->format, 0, 255, 0);
	addChildToFather(menuPanel, createButtonWithColor(win->surface, 0, 0, "images/tools/black_bishop.bmp", setChosenToolToBlackBishop, menuPanel, 0, "Bishop", green));
	addChildToFather(menuPanel, createButtonWithColor(win->surface, 46, 0, "images/tools/black_queen.bmp", setChosenToolToBlackQueen, menuPanel, 0, "Bishop", green));
	addChildToFather(menuPanel, createButtonWithColor(win->surface, 46 * 2, 0, "images/tools/black_king.bmp", setChosenToolToBlackKing, menuPanel, 0, "Bishop", green));
	addChildToFather(menuPanel, createButtonWithColor(win->surface, 46 * 3, 0, "images/tools/black_pawn.bmp", setChosenToolToBlackPawn, menuPanel, 0, "Bishop", green));
	addChildToFather(menuPanel, createButtonWithColor(win->surface, 0, 46, "images/tools/black_rook.bmp", setChosenToolToBlackRook, menuPanel, 0, "Bishop", green));
	addChildToFather(menuPanel, createButtonWithColor(win->surface, 46, 46, "images/tools/black_knight.bmp", setChosenToolToBlackKnight, menuPanel, 0, "Bishop", green));
	addChildToFather(menuPanel, createButtonWithColor(win->surface, 46 * 2, 46, "images/tools/white_bishop.bmp", setChosenToolToWhiteBishop, menuPanel, 0, "Bishop", green));
	addChildToFather(menuPanel, createButtonWithColor(win->surface, 46 * 3, 46, "images/tools/white_queen.bmp", setChosenToolToWhiteQueen, menuPanel, 0, "Bishop", green));
	addChildToFather(menuPanel, createButtonWithColor(win->surface, 0, 92, "images/tools/white_king.bmp", setChosenToolToWhiteKing, menuPanel, 0, "Bishop", green));
	addChildToFather(menuPanel, createButtonWithColor(win->surface, 46 * 1, 92, "images/tools/white_pawn.bmp", setChosenToolToWhitePawn, menuPanel, 0, "Bishop", green));
	addChildToFather(menuPanel, createButtonWithColor(win->surface, 46 * 2, 92, "images/tools/white_rook.bmp", setChosenToolToWhiteRook, menuPanel, 0, "Bishop", green));
	addChildToFather(menuPanel, createButtonWithColor(win->surface, 46 * 3, 92, "images/tools/white_knight.bmp", setChosenToolToWhiteKnight, menuPanel, 0, "Bishop", green));
	addChildToFather(menuPanel, CreateButton(win->surface, 10, 150, "images/trash.bmp", setChosenToolToTrash, menuPanel, 0, "trash"));
	addChildToFather(menuPanel, CreateButton(win->surface, 15, 400, "images/PlayersSelection/next.bmp", startNewGameIfBoardValid, menuPanel, 0, "next"));
	addChildToFather(menuPanel, CreateButton(win->surface, 15, 460, "images/AISettings/clear.bmp", clearBoard, menuPanel, 0, "clear"));
	UINode* returnBtn = CreateButton(win->surface, 15, 520, "images/AISettings/return.bmp", returnFunc, menuPanel, 0, "return");
	addChildToFather(menuPanel, returnBtn);

	//init_board(board);
	drawBoard(board, boardSettingsWindow);
	presentUITree(boardSettingsWindow);
	ActiveWindow = boardSettingsWindow;


}
void cancelPlayerSelection()
{
	//return to main window

	clear_board();
	nextPlayer = WHITE;
	//clean resources
	CreateMainWindow();
	presentUITree(mainWindow);
	ActiveWindow = mainWindow;
	
}
void twoPlayerMode()
{
	UINode* father = getNodeByName("selectPanel", playerSelectionWindow);
	//check if comp players button is marked and change him to unmark
	UINode* markNodeBtn = getNodeByName("playerVsCompMark", playerSelectionWindow);
	if (markNodeBtn != NULL)
	{
		Window* win = (Window*)playerSelectionWindow->control;
		ImgButton* oldBtn = (ImgButton*)markNodeBtn->control;
		UINode* regularBtn = CreateButton(win->surface, oldBtn->x, oldBtn->y, "images/PlayersSelection/playComp.bmp", playerVsComputerMode, markNodeBtn->father, 0, "playerVsComp");
		replaceUINodeChild(father, regularBtn, "playerVsCompMark");
	}
	gameMode = 1;
	//update the bottom to be in highlight color
	UINode* buttonNode = getNodeByName("twoPlayers", playerSelectionWindow);
	if (buttonNode == NULL)
	{
		presentUITree(playerSelectionWindow);
		return;
	}
	//change the node with new control and load him
	Window* win = (Window*)playerSelectionWindow->control;
	ImgButton* oldBtn = (ImgButton*)buttonNode->control;
	UINode* coloredBtnNode = CreateButton(win->surface, oldBtn->x, oldBtn->y, "images/PlayersSelection/twoplay.bmp", NULL, buttonNode->father, 0, "twoPlayersMark");
	
	replaceUINodeChild(father, coloredBtnNode, "twoPlayers");
	presentUITree(playerSelectionWindow);
}

void playerVsComputerMode()
{
	UINode* father = getNodeByName("selectPanel", playerSelectionWindow);
	//check if two players button is marked and change him to unmark
	UINode* markNodeBtn = getNodeByName("twoPlayersMark", playerSelectionWindow);
	if (markNodeBtn != NULL)
	{
		Window* win = (Window*)playerSelectionWindow->control;
		ImgButton* oldBtn = (ImgButton*)markNodeBtn->control;
		UINode* regularBtn = CreateButton(win->surface, oldBtn->x, oldBtn->y, "images/PlayersSelection/twoPlayers.bmp", twoPlayerMode, markNodeBtn->father, 0, "twoPlayers");
		replaceUINodeChild(father, regularBtn, "twoPlayersMark");
	}

	gameMode = 2;
	UINode* buttonNode = getNodeByName("playerVsComp", playerSelectionWindow);
	if (buttonNode == NULL)
	{
		presentUITree(playerSelectionWindow);
		return;
	}

	//change the node with new control and load him
	Window* win = (Window*)playerSelectionWindow->control;
	ImgButton* oldBtn = (ImgButton*)buttonNode->control;
	UINode* coloredBtnNode = CreateButton(win->surface, oldBtn->x, oldBtn->y, "images/PlayersSelection/playComH.bmp", NULL, buttonNode->father, 0, "playerVsCompMark");	
	replaceUINodeChild(father, coloredBtnNode, "playerVsComp");
	presentUITree(playerSelectionWindow);
}

void chooseNextWhite()
{
	Window* win;
	ImgButton* oldBtn;
	nextPlayer = WHITE;
	UINode* coloredBtnNode;
	UINode* father = getNodeByName("selectPanel", playerSelectionWindow);
	//check if comp players button is marked and change him to unmark
	UINode* markNodeBtn = getNodeByName("blackMark", playerSelectionWindow);
	if (markNodeBtn != NULL)
	{
		Window* win = (Window*)playerSelectionWindow->control;
		ImgButton* oldBtn = (ImgButton*)markNodeBtn->control;
		UINode* regularBtn = CreateButton(win->surface, oldBtn->x, oldBtn->y, "images/PlayersSelection/black.bmp", chooseNextBlack, markNodeBtn->father, 0, "black");
		replaceUINodeChild(father, regularBtn, "blackMark");
	}
	//update the bottom to be in highlight color
	UINode* buttonNode = getNodeByName("white", playerSelectionWindow);
	if (buttonNode == NULL)
	{
		presentUITree(playerSelectionWindow);
		return;
	}
	//change the node with new control and load him
	win = (Window*)playerSelectionWindow->control;
	oldBtn = (ImgButton*)buttonNode->control;
	coloredBtnNode = CreateButton(win->surface, oldBtn->x, oldBtn->y, "images/PlayersSelection/whiteMark.bmp", NULL, buttonNode->father, 0, "whiteMark");
	replaceUINodeChild(father, coloredBtnNode, "white");
	presentUITree(playerSelectionWindow);

}
void chooseNextBlack()
{
	nextPlayer = BLACK;
	UINode* father = getNodeByName("selectPanel", playerSelectionWindow);
	//check if comp players button is marked and change him to unmark
	UINode* markNodeBtn = getNodeByName("whiteMark", playerSelectionWindow);
	if (markNodeBtn != NULL)
	{
		Window* win = (Window*)playerSelectionWindow->control;
		ImgButton* oldBtn = (ImgButton*)markNodeBtn->control;
		UINode* regularBtn = CreateButton(win->surface, oldBtn->x, oldBtn->y, "images/PlayersSelection/white.bmp", chooseNextWhite, markNodeBtn->father, 0, "white");
		replaceUINodeChild(father, regularBtn, "whiteMark");
	}
	//update the bottom to be in highlight color
	UINode* buttonNode = getNodeByName("black", playerSelectionWindow);
	if (buttonNode == NULL)
	{
		presentUITree(playerSelectionWindow);
		return;
	}
	//change the node with new control and load him
	Window* win = (Window*)playerSelectionWindow->control;
	ImgButton* oldBtn = (ImgButton*)buttonNode->control;
	UINode* coloredBtnNode = CreateButton(win->surface, oldBtn->x, oldBtn->y, "images/PlayersSelection/blackMark.bmp", NULL, buttonNode->father, 0, "blackMark");
	replaceUINodeChild(father, coloredBtnNode, "black");
	presentUITree(playerSelectionWindow);
}
void openPlayerSelectionWindow(void* sourceBottomName)
{

	char* sourceBtnName = (char*)sourceBottomName;
	char* whiteBtnPath;
	char* blackBtnPath;
	char* twoplayersBtnPath = "images/PlayersSelection/twoPlayers.bmp";
	char* playervsCompBtnPath = "images/PlayersSelection/playComp.bmp";
	char* whiteBtnName = "whiteMark";
	char* blackBtnName = "black";
	char* twoplayersBtnName = "twoPlayers";
	char* playervsCompBtnName = "playerVsComp";
	//init resreved arguments
	if (nextPlayer == WHITE)
	{
		whiteBtnName = "whiteMark";
		blackBtnName = "black";
		whiteBtnPath = "images/PlayersSelection/whiteMark.bmp";
		blackBtnPath = "images/PlayersSelection/black.bmp";
	}
	else
	{
		whiteBtnName = "white";
		blackBtnName = "blackMark";
		whiteBtnPath = "images/PlayersSelection/white.bmp";
		blackBtnPath = "images/PlayersSelection/blackMark.bmp";
	}

	//init gamr mode:
	if (gameMode == 1)
	{
		playervsCompBtnName = "playerVsComp";
		twoplayersBtnName = "twoPlayersMark";
		twoplayersBtnPath = "images/PlayersSelection/twoplay.bmp";
		playervsCompBtnPath = "images/PlayersSelection/playComp.bmp";
	}
	else if (gameMode == 2)
	{
		playervsCompBtnName = "playerVsCompMark";
		twoplayersBtnName = "twoPlayers";
		playervsCompBtnPath = "images/PlayersSelection/playComH.bmp";
		twoplayersBtnPath = "images/PlayersSelection/twoPlayers.bmp";
	}
	//init or load the game configuration
	if (strcmp(sourceBtnName, "newGame") == 0)
	{
		//set default values:
		gameMode = 1;
		minimax_depth = 1;
		nextPlayer = WHITE;
		userColor = WHITE;
		init_board(board);
		playervsCompBtnName = "playerVsComp";
		twoplayersBtnName = "twoPlayersMark";
		whiteBtnName = "whiteMark";
		blackBtnName = "black";

		whiteBtnPath = "images/PlayersSelection/whiteMark.bmp";
		blackBtnPath = "images/PlayersSelection/black.bmp";
		twoplayersBtnPath = "images/PlayersSelection/twoplay.bmp";
		playervsCompBtnPath = "images/PlayersSelection/playComp.bmp";

	}
	else if (strcmp(sourceBtnName, "loadGame") == 0)
	{
		if (gameMode == 2)
		{
			playervsCompBtnName = "playerVsCompMark";
			twoplayersBtnName = "twoPlayers";
			playervsCompBtnPath = "images/PlayersSelection/playComH.bmp";
			twoplayersBtnPath = "images/PlayersSelection/twoPlayers.bmp";
		}
		else if (gameMode == 1)
		{
			playervsCompBtnName = "playerVsComp";
			twoplayersBtnName = "twoPlayersMark";
			twoplayersBtnPath = "images/PlayersSelection/twoplay.bmp";
			playervsCompBtnPath = "images/PlayersSelection/playComp.bmp";
		}
		if (nextPlayer == WHITE)
		{
			whiteBtnName = "whiteMark";
			blackBtnName = "black";
			whiteBtnPath = "images/PlayersSelection/whiteMark.bmp";
			blackBtnPath = "images/PlayersSelection/black.bmp";
		}
		else if (nextPlayer == BLACK)
		{
			whiteBtnName = "white";
			blackBtnName = "blackMark";
			whiteBtnPath = "images/PlayersSelection/white.bmp";
			blackBtnPath = "images/PlayersSelection/blackMark.bmp";
		}

	}

	//move to the next window
	//create window
	playerSelectionWindow = CreateWindow("Chess Players Selection", WIN_WIDTH, WIN_HEIGHT, 0, NULL, 255, 255, 255);
	Window* win = (Window*)playerSelectionWindow->control;
	UINode* selectPanel = CreatePanel(win->surface, 0, 0, WIN_WIDTH, WIN_HEIGHT, SDL_MapRGB(win->surface->format, 255, 255, 255), playerSelectionWindow, 0, "selectPanel");

	//create labels and buttons
	UINode* gameModeLabel = createLabel(win->surface, 50, 150, "images/PlayersSelection/gameMode.bmp", selectPanel, "gameMode");
	UINode* titleLabel = createLabel(win->surface, 220, 50, "images/PlayersSelection/title1.bmp", selectPanel, "title1");
	UINode* nextPlayerLabel = createLabel(win->surface, 50, 250, "images/PlayersSelection/nextPlayer.bmp", selectPanel, "nextPlayer");
	UINode* cancelBtn = CreateButton(win->surface, 50, 400, "images/PlayersSelection/cancel.bmp", cancelPlayerSelection, selectPanel, 0, "Cancel");
	UINode* BoardSettingsBtn = CreateButton(win->surface, 300, 400, "images/PlayersSelection/boardSettings.bmp", openBoardSettingWindow, selectPanel, 0, "boardSet");
	UINode* NextBtn = CreateButton(win->surface, 550, 400, "images/PlayersSelection/next.bmp", NextButtomClicked, selectPanel, 0, "next");

	UINode* twoPlayersBtn = CreateButton(win->surface, 250, 150, twoplayersBtnPath, twoPlayerMode, selectPanel, 0, twoplayersBtnName);
	UINode* compPlayersBtn = CreateButton(win->surface, 500, 150, playervsCompBtnPath, playerVsComputerMode, selectPanel, 0, playervsCompBtnName);
	UINode* whiteBtn = CreateButton(win->surface, 250, 250, whiteBtnPath, chooseNextWhite, selectPanel, 0, whiteBtnName);
	UINode* blackBtn = CreateButton(win->surface, 500, 250, blackBtnPath, chooseNextBlack, selectPanel, 0, blackBtnName);

	addChildToFather(playerSelectionWindow, selectPanel);
	addChildToFather(selectPanel, titleLabel);
	addChildToFather(selectPanel, gameModeLabel);
	addChildToFather(selectPanel, twoPlayersBtn);
	addChildToFather(selectPanel, compPlayersBtn);

	addChildToFather(selectPanel, nextPlayerLabel);
	addChildToFather(selectPanel, whiteBtn);
	addChildToFather(selectPanel, blackBtn);
	//addChildToFather(selectPanel, defaultColLabel);
	addChildToFather(selectPanel, cancelBtn);
	addChildToFather(selectPanel, BoardSettingsBtn);
	addChildToFather(selectPanel, NextBtn);

	presentUITree(playerSelectionWindow);
	ActiveWindow = playerSelectionWindow;
}

void QuitError()
{
	//free resurces

	if (mainWindow != NULL)
	{
		freeUINode(mainWindow);
		mainWindow = NULL;
	}
	if (gameWindow != NULL)
	{
		freeUINode(gameWindow);
		gameWindow = NULL;
	}

	if (settingWindow != NULL)
	{
		freeUINode(settingWindow);
		settingWindow = NULL;
	}

	if (playerSelectionWindow != NULL)
	{
		freeUINode(playerSelectionWindow);
		playerSelectionWindow = NULL;
	}
	SDL_Quit();
	exit(1);
}
void quitGame()
{
	//free resurces

	if (mainWindow != NULL)
	{
		freeUINode(mainWindow);
		mainWindow = NULL;
	}
		
	if (gameWindow != NULL)
	{
		freeUINode(gameWindow);
		gameWindow = NULL;
	}
		
	if (settingWindow != NULL)
	{
		freeUINode(settingWindow);
		settingWindow = NULL;
	}
		
	if (playerSelectionWindow != NULL)
	{
		freeUINode(playerSelectionWindow);
		playerSelectionWindow = NULL;
	}	
	shouldQuitEvents = 1;
	SDL_Quit();
	exit(0);
}


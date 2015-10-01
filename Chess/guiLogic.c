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
	UINode* loadGameBtn = CreateButton(win->surface, 315, 230, "images/LoadGame.bmp", loadGame, mainPanel, 0,"loadGame");
	UINode* quitGameBtn = CreateButton(win->surface, 315, 310, "images/Quit.bmp", quitGame, mainPanel, 0, "quit");
	UINode* logoBtn = CreateButton(win->surface, 100, 50, "images/chesslogo.bmp", NULL, mainPanel, 0,"logo");

	//adding children
	addChildToFather(mainWindow, mainPanel);
	addChildToFather(mainPanel, newGameBtn);
	addChildToFather(mainPanel, loadGameBtn);
	addChildToFather(mainPanel, quitGameBtn);
	addChildToFather(mainPanel, logoBtn);
	
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
	addBoardToPanel(gamePanel, win);

}

void loadGameFromSlot(char* sourceBtn)
{
	if (strcmp(sourceBtn, "slot1") == 0)
	{

	}
	if (strcmp(sourceBtn, "slot2") == 0)
	{

	}
}
void loadGame()
{
	//add 7 buttons of numbers
	//UINode* loadGameBtn = CreateButton(win->surface, 315, 230, "images/LoadGame.bmp", loadGame, mainPanel, 0, "loadGame");
	Window* win = (Window*)mainWindow->control;
	Panel* mainPanel = (Panel*)mainWindow->children[0]->control;
	UINode* slotNumBtn = createLabel(win->surface, 500, 50, "images/slotNum.bmp", mainWindow->children[0], "slotNum");
	UINode* slot1Btn = CreateButton(win->surface, 600, 100, "images/1.bmp", NULL, mainWindow->children[0], 0, "slot1");
	UINode* slot2Btn = CreateButton(win->surface, 600, 150, "images/2.bmp", NULL, mainWindow->children[0], 0, "slot2");
	UINode* slot3Btn = CreateButton(win->surface, 600, 200, "images/3.bmp", NULL, mainWindow->children[0], 0, "slot3");
	UINode* slot4Btn = CreateButton(win->surface, 600, 250, "images/4.bmp", NULL, mainWindow->children[0], 0, "slot4");
	UINode* slot5Btn = CreateButton(win->surface, 600, 300, "images/5.bmp", NULL, mainWindow->children[0], 0, "slot5");
	UINode* slot6Btn = CreateButton(win->surface, 600, 350, "images/6.bmp", NULL, mainWindow->children[0], 0, "slot6");
	UINode* slot7Btn = CreateButton(win->surface, 600, 400, "images/7.bmp", NULL, mainWindow->children[0], 0, "slot7");

	addChildToFather(mainWindow->children[0], slotNumBtn);
	addChildToFather(mainWindow->children[0], slot1Btn);
	addChildToFather(mainWindow->children[0], slot2Btn);
	addChildToFather(mainWindow->children[0], slot3Btn);
	addChildToFather(mainWindow->children[0], slot4Btn);
	addChildToFather(mainWindow->children[0], slot5Btn);
	addChildToFather(mainWindow->children[0], slot6Btn);
	addChildToFather(mainWindow->children[0], slot7Btn);

	presentUITree(mainWindow);

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
void chooseBlackColor()
{
	userColor = BLACK;
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
	shouldQuitsettingEvents = 1;
	freeUINode(settingWindow);
	openPlayerSelectionWindow("None");
}
void openSettingWindow()
{
	//create setting window
	shouldQuitSelectionEvents = 1;
	shouldQuitsettingEvents = 0;
	//clean resources
	settingWindow = CreateWindow("Chess Game AI Settings", WIN_WIDTH, WIN_HEIGHT, 0, NULL);
	Window* win = (Window*)settingWindow->control;
	UINode* settingPanel = CreatePanel(win->surface, 0, 0, WIN_WIDTH, WIN_HEIGHT, SDL_MapRGB(win->surface->format, 255, 255, 255), settingWindow, 0, "AISettingPanel");

	UINode* userColorLabel = createLabel(win->surface, 50, 150, "images/AISettings/userColor.bmp", settingPanel, "userColorLabel");
	UINode* titleLabel = createLabel(win->surface, 300, 50, "images/AISettings/title.bmp", settingPanel, "titleLabel");
	UINode* depthLabel = createLabel(win->surface, 50, 250, "images/AISettings/gameDepth.bmp", settingPanel, "depthLabel");

	UINode* whiteBtn = CreateButton(win->surface, 250, 150, "images/PlayersSelection/whiteMark.bmp", chooseWhiteColor, settingPanel, 0, "whiteMark");
	UINode* blackBtn = CreateButton(win->surface, 500, 150, "images/PlayersSelection/black.bmp", chooseBlackColor, settingPanel, 0, "black");
	UINode* depth1 = CreateButton(win->surface, 250, 250, "images/AISettings/1m.bmp", chooseDepth1, settingPanel, 0, "depth1Mark");
	UINode* depth2 = CreateButton(win->surface, 350, 250, "images/AISettings/2.bmp", chooseDepth2, settingPanel, 0, "depth2");
	UINode* depth3 = CreateButton(win->surface, 450, 250, "images/AISettings/3.bmp", chooseDepth3, settingPanel, 0, "depth3");
	UINode* depth4 = CreateButton(win->surface, 550, 250, "images/AISettings/4.bmp", chooseDepth4, settingPanel, 0, "depth4");
	UINode* depthBest = CreateButton(win->surface, 650, 250, "images/AISettings/best.bmp", chooseDepthBest, settingPanel, 0, "best");

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
	EventsLoopSettingWindow();



}
void NextButtomClicked()
{
	shouldQuitSelectionEvents = 1;
	free(playerSelectionWindow);
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
	lastChosenTool = -1;
}

void openBoardSettingWindow()
{

	shouldQuitMainEvents = 1;
	shouldQuitSelectionEvents = 1;
	boardSettingsWindow = CreateWindow("Chess Board Settings", WIN_WIDTH, WIN_HEIGHT, 0, NULL);
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
	
	init_board(board);
	drawBoard(board);
	presentUITree(boardSettingsWindow);
	EventsLoopboardSettingWindow();


}
void cancelPlayerSelection()
{
	//return to main window

	clear_board();
	nextPlayer = WHITE;
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
		return;
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
	//change the node with new control and load him
	Window* win = (Window*)playerSelectionWindow->control;
	ImgButton* oldBtn = (ImgButton*)buttonNode->control;
	UINode* coloredBtnNode = CreateButton(win->surface, oldBtn->x, oldBtn->y, "images/PlayersSelection/blackMark.bmp", NULL, buttonNode->father, 0, "blackMark");
	replaceUINodeChild(father, coloredBtnNode, "black");
	presentUITree(playerSelectionWindow);
}
void openPlayerSelectionWindow(void* sourceBottomName)
{
	//clear mainWindow tree
	shouldQuitMainEvents = 1;
	
	char* sourceBtnName = (char*)sourceBottomName;
	//init or load the game configuration
	if (strcmp(sourceBtnName, "newGame") == 0)
	{
		//set default values:
		minimax_depth = 1;
		nextPlayer = WHITE;
		userColor = WHITE;
		pawnPromotionTool = -1000;//queen
		init_board(board);
		freeUINode(mainWindow);
	}
	else if (strcmp(sourceBtnName, "loadGame") == 0)
	{
		freeUINode(mainWindow);
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
	UINode* cancelBtn = CreateButton(win->surface, 50, 400, "images/PlayersSelection/cancel.bmp", cancelPlayerSelection, selectPanel, 0, "Cancel");
	UINode* BoardSettingsBtn = CreateButton(win->surface, 300, 400, "images/PlayersSelection/boardSettings.bmp", openBoardSettingWindow, selectPanel, 0, "boardSet");
	UINode* NextBtn = CreateButton(win->surface, 550, 400, "images/PlayersSelection/next.bmp", NextButtomClicked, selectPanel, 0, "next");

	UINode* twoPlayersBtn = CreateButton(win->surface, 250, 150, "images/PlayersSelection/twoPlayers.bmp", twoPlayerMode, selectPanel, 0, "twoPlayers");
	UINode* compPlayersBtn = CreateButton(win->surface, 500, 150, "images/PlayersSelection/playComp.bmp", playerVsComputerMode, selectPanel, 0, "compPlayers");
	UINode* whiteBtn = CreateButton(win->surface, 250, 250, "images/PlayersSelection/whiteMark.bmp", chooseNextWhite, selectPanel, 0, "whiteMark");
	UINode* blackBtn = CreateButton(win->surface, 500, 250, "images/PlayersSelection/black.bmp", chooseNextBlack, selectPanel, 0, "black");

	//labels:
	//UINode* defaultColLabel = createLabel(win->surface, 450, 245, "images/PlayersSelection/whitet.bmp", selectPanel, "colorLabel");

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


#ifndef GUI_H
#define GUI_H
#include "SDL.h"
#include "SDL_video.h"
#include "fileHandle.h"
#include "ChessHelp.h"
#include "Chess.h"


#define WIN_HEIGHT 600
#define WIN_WIDTH 800
#define BUTTON 'b'
#define PANEL 'p'
#define WINDOW 'w'
#define LABEL 'l'


struct UINode* mainWindow;
struct UINode* settingWindow;
struct UINode* playerSelectionWindow;
struct UINode* boardSettingsWindow;
struct UINode* gameWindow;

int shouldQuitMainEvents;
int shouldQuitGameEvents;
int shouldQuitsettingEvents;
int shouldQuitAiSettingEvents;
//structures
struct ImgButton
{
	SDL_Surface * surface;
	int x;
	int y;
	char * filename;
	char type;
	char* name;
};
struct Label
{
	SDL_Surface * surface;
	int x;
	int y;
	char type;

};

//Panel contains multiple surfaces (up to 20)
struct Panel
{
	SDL_Surface * surface;
	int x;
	int y;
	char type;
	SDL_Rect* rect;
	int width;
	int height;
};

struct Window
{
	SDL_Surface * surface;
	char* title;
	int width;
	int height;
	char type;
};

struct UINode
{
	void* control;
	char type;//type of the control - button, panel, lable,window
	void(*Action)(void*);//this function will be the logic for the specific control
	int childsNumber;
	struct UINode** children;
	struct UINode* father;
}; 


typedef struct UINode UINode;
//typedef struct control control;
typedef struct ImgButton ImgButton;
typedef struct Panel Panel;
typedef struct Window Window;
typedef struct Label Label;

//functions
UINode* CreateAndAddUINode(void* control, int childsNumber, char type, UINode* father, void(*Action)(void*));
void addChildToFather(UINode* father, UINode* child);
//dfs scanning and presenting the tree
void presentUITree(UINode* root);
void freeUINode(UINode* root);

//create controls:
UINode* CreateWindow(char* title, int width, int height, int childsNumber, SDL_Rect* rect);
UINode* CreatePanel(SDL_Surface * surface, int x, int y, int width, int height, int color, UINode *father, int childsNumber);
UINode* CreateButton(SDL_Surface * surface, int x, int y, char * filename, 
	void(*Action)(void*), UINode *father, int childsNumber, char* name);
ImgButton createImgButton(int x, int y, char * filename, SDL_Surface * window);

void init();
int isButtonClicked(ImgButton btn, int clickedX, int clickedY);
SDL_Surface* loadImage(ImgButton btn, SDL_Surface * window);
void EventsLoopMainWindow();
void EventsLoopGameWindow();

//gui logic: create all 4 specific windows, game  buttons
void CreateMainWindow();

void startNewGame();
void quitGame();
void openSettingWindow();
void openPlayerSelectionWindow();

int main(int argc, char* args[]);


#endif GUI_H



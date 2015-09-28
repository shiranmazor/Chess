#ifndef GUI_H
#define GUI_H
#include "SDL.h"
#include "SDL_video.h"
#include "fileHandle.h"
#include "ChessHelp.h"

#define WIN_TITLE "Chess"
#define WIN_HEIGHT 600
#define WIN_WIDTH 800
#define BUTTON 'b'
#define PANEL 'p'
#define WINDOW 'w'
#define LABEL 'l'


struct UINode* mainWindow;
struct UINode* settingWindow;
struct UINode* AISettingWindow;
struct UINode* gameWindow;

//structures
struct ImgButton
{
	SDL_Surface * surface;
	int x;
	int y;
	char * filename;
	char type;
};
struct Label
{
	SDL_Surface * text;
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
UINode* CreateWindow(char* title, int with, int high);

int isButtonClicked(ImgButton btn, int clickedX, int clickedY);
void init();
SDL_Surface* loadImage(ImgButton btn, SDL_Surface * window);

//logic: create all 4 specific windows, game  buttons
void CreateMainWindow();

int main(int argc, char* args[]);


#endif GUI_H



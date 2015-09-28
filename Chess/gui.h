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

struct UINode* root;

//structures
struct ImgButton
{
	SDL_Surface * surface;
	int x;
	int y;
	char * filename;
};
struct Label
{
	SDL_Surface * text;
	int x;
	int y;
};

//Panel contains multiple surfaces (up to 20)
struct Panel
{
	SDL_Surface * surface;
	int x;
	int y;
	//SDL_Surface * buttonsArr[20];
};

struct Window
{
	SDL_Surface * surface;
	char* title;
	int with;
	int high;
	//SDL_Surface * PanelsArr[20];
};

struct UINode
{
	struct control* control;
	int childsNumber;
	struct UINode** children;
	struct UINode* father;
}; 

struct control
{
	SDL_Surface* surface;
	SDL_Rect* rect;
	//SDL_Rect* location_rect;
	char type;//type of the control - button, panel, lable,window
	void(*Action)(struct control*);//this function will be the logic for the specific control
}; 

typedef struct UINode UINode;
typedef struct control control;
typedef struct ImgButton ImgButton;
typedef struct Panel Panel;
typedef struct Window Window;
typedef struct Label Label;

//functions
UINode* CreateNewTree(control* window, int childsNumber);
UINode* addNewControlToTree(UINode* father, int childsNumber, control* control);
void addChildToFather(UINode* father, UINode* child);

//create controls:
control* CreateWindow(char* title, int with, int high, SDL_Rect* rect);

int isButtonClicked(ImgButton btn, int clickedX, int clickedY);
void init();
SDL_Surface* loadImage(ImgButton btn, SDL_Surface * window);

//logic: create all 4 specific windows, game  buttons
SDL_Surface *CreateMainWindow();

int main(int argc, char* args[]);


#endif GUI_H



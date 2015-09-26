#ifndef GUI_H
#define GUI_H
#include "SDL.h"
#include "fileHandle.h"
#define WIN_TITLE "Chess"
#define WIN_HEIGHT 600
#define WIN_WIDTH 800

typedef struct ImgButton ImgButton;
int isButtonClicked(ImgButton btn, int clickedX, int clickedY);
SDL_Surface * init();
SDL_Surface* loadImage(ImgButton btn, SDL_Surface * window);

int main(int argc, char* args[]);


#endif GUI_H



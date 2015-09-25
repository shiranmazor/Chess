#ifndef FILE_HANDLE_H
#define FILE_HANDLE_H
#include "Chess.h"

#define SLOTS_NUM  7
#define FILENAME "savedSlot"
#define FILEEXT ".xml"

typedef struct GameStatus GameStatus;
struct GameStatus
{
	int nextTurn;
	int gameMode;
	char difficulty[100]; 
	int userColor; 
	char board[BOARD_SIZE][BOARD_SIZE];
};

int isFileExist();
GameStatus readFile(int slotNumber);
char * getFilenameBySlotNumber(int slotNumber);

#endif FILE_HANDLE_H


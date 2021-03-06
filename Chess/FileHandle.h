#ifndef FILE_HANDLE_H
#define FILE_HANDLE_H

#ifndef CHESS_
#include "Chess.h"
#endif 

#define SLOTS_NUM  7
#define gameSlots SLOTS_NUM 
#define FILENAME "savedSlot"
#define FILEEXT ".xml"

typedef struct GameStatus GameStatus;
struct GameStatus
{
	int nextTurn;
	int gameMode;
	int difficulty; 
	int userColor; 
	char board[BOARD_SIZE][BOARD_SIZE];
};

GameStatus readFileWithSlotNumber(int slotNumber);
GameStatus readFileWithFilename(char * filename);
int saveFileWithFileName(GameStatus gameState, char * fullFileName);
int saveFileWithSlotNumber(GameStatus gameState, int slotNumber);
//int isFileExist(char * fullFileName);
char * getFilenameBySlotNumber(int slotNumber);
int fileExists(const char *fname);

#endif 


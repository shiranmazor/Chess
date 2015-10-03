#ifndef CONSOLE_H
#define CONSOLE_H

#ifndef CHESS_
#include "Chess.h"
#endif 

//#ifndef MINIMAX_H
#include "minimax.h"
//#endif MINIMAX_H

#ifndef CHESS_HELP_H
#include "ChessHelp.h"
#endif 

#ifndef FILE_HANDLE_H
#include "fileHandle.h"
#endif 

void settingState();
void executeSettingCmd(char* input);
void GameState();
void GameTwoPlayers();
void GameUserVsComputer();
int ComputerMove();
int UserMove(int userColor);

int getMoveScore(Move *move, int d, int playerColor);
void print_board(char board[BOARD_SIZE][BOARD_SIZE]);
void print_line();
void clear_board();
void init_board(char board[BOARD_SIZE][BOARD_SIZE]);
void remove_disc(char* input);
void set_disc(char* pos_input, char* color, char* type);
int checkNewBoardValidation(int color, char* Tool);
char* getPawnPromoteString(char tool);
int countKings();
void runConsole();

#endif 


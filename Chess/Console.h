#ifndef CONSOLE_H
#define CONSOLE_H

#ifndef CHESS_
#include "Chess.h"
#endif CHESS_

#include "ChessHelp.h"



void settingState();
void executeSettingCmd(char* input);
void GameState();
void GameTwoPlayers(int user1Color, int user2Color);
void GameUserVsComputer(int computerColor);

void print_board(char board[BOARD_SIZE][BOARD_SIZE]);
void print_line();
void clear_board();
void init_board(char board[BOARD_SIZE][BOARD_SIZE]);
void remove_disc(char* input);
void set_disc(char* pos_input, char* color, char* type);
int checkNewBoardValidation(int color, char* Tool);
int countKings();

#endif CONSOLE_H


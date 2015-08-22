#pragma once
#include "Chess.h"
#include "ChessHelp.h"

void settingState();
void executeSettingCmd(char* input);
void GameState();

int boardInitializeOk();
void print_board(char board[BOARD_SIZE][BOARD_SIZE]);
void print_line();
void clear_board();
void init_board(char board[BOARD_SIZE][BOARD_SIZE]);
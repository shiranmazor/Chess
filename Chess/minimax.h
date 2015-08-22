#pragma once
#include "Chess.h"
#include "ChessHelp.h"

int maxBoardNum = 1000000;
int minimax(char board[BOARD_SIZE][BOARD_SIZE], int depth, int isMaxplayer, Move** bestMove);
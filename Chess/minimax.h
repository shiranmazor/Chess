#pragma once
#include "Chess.h"
#include "ChessHelp.h"

int maxBoardNum = 1000000;
int minimax(char board[BOARD_SIZE][BOARD_SIZE], int depth, int computerColor, Move** bestMove, int alpha, int beta, int isMax);
int score(char board[BOARD_SIZE][BOARD_SIZE], int PlayerColor);
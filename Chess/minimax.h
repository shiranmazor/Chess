
#ifndef MINIMAX_H

#define MINIMAX_H

#ifndef CHESS_H
#include "Chess.h"
#endif CHESS_H

#ifndef CHESS_HELP_H
#include "ChessHelp.h"
#endif CHESS_HELP_H

int maxBoardNum = 1000000;
int minimax(char board[BOARD_SIZE][BOARD_SIZE], int depth, int computerColor,
	Move** bestMove, int alpha, int beta, int isMax, int boardCounter);
int score(char board[BOARD_SIZE][BOARD_SIZE], int PlayerColor);

#endif MINIMAX_H

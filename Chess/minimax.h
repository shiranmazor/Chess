
#ifndef MINIMAX_H
#define MINIMAX_H

#ifndef CHESS_H
#include "Chess.h"
#endif 

#ifndef CHESS_HELP_H
#include "ChessHelp.h"
#endif 

int minimax(char board[BOARD_SIZE][BOARD_SIZE], int depth,
	Move** bestMove, int alpha, int beta, int isMax, int boardCounter);
int score(char board[BOARD_SIZE][BOARD_SIZE], int PlayerColor);

#endif 

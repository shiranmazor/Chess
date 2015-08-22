#pragma once
#include "Chess.h"
#include "ChessHelp.h"

int minimax(char board[BOARD_SIZE][BOARD_SIZE], int depth, int isMaxplayer, Move** bestMove);
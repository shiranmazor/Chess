#include "minimax.h"

//recursive function for return the scoring result of the best move
int minimax(char board[BOARD_SIZE][BOARD_SIZE], int depth, int computerColor, Move** bestMove, 
	int alpha, int beta, int isMax, int boardCounter)
{
	//if depth  == -1 we are in best difficulty
	//get user and
	MoveNode* moves = getMoves(board, computerColor);

	//check if no moves or depth is 0
	if (moves == NULL || depth == 0 || boardCounter == 1000000)
	{
		//print_board(board);
		int res = score(board, computerColor);
		freeMoves(moves, NULL);
		//return res*(-1);
		return res;
	}
	else//############ let's generate minimax tree!
	{
		if (isMax == 1)//player is the computer
		{
			MoveNode* movesPointer = moves;
			while (movesPointer != NULL)
			{
				int newRes = 0;
				char newBoard[BOARD_SIZE][BOARD_SIZE];
				performMoveMinimax(board, newBoard, movesPointer->move);
				boardCounter++;
				//print_board(board);
				//print_board(newBoard);
				newRes = minimax(newBoard, depth - 1, computerColor, bestMove, alpha, beta, 0, boardCounter);
				
				//insert prunning:
				if (newRes > alpha)
				{
					alpha = newRes;
					if (depth == minimax_depth)//check if we are in first recursion
						*(bestMove) = movesPointer->move;

				}
				if (alpha > beta)
				{
					freeMoves(moves, *(bestMove));
					return beta; //pruning
				}
				movesPointer = movesPointer->next;
			}
			/*
			if (!*(bestMove) && depth == minimax_depth)//all the  moves are bad
			*(bestMove) = moves->move;
			*/

			freeMoves(moves, *(bestMove));
			return alpha;
		}
		else//player is the user:
		{
			MoveNode* movesPointer = moves;
			while (movesPointer != NULL)
			{
				int newRes = 0;
				char newBoard[BOARD_SIZE][BOARD_SIZE];
				performMoveMinimax(board, newBoard, movesPointer->move);
				boardCounter++;
				newRes = minimax(newBoard, depth - 1, computerColor, bestMove, alpha, beta, 1, boardCounter);
				if (alpha >= beta)
				{
					freeMoves(moves, *(bestMove));
					return alpha;
				}

				movesPointer = movesPointer->next;
			}

			freeMoves(moves, *(bestMove));
			return beta;
		}

	}
}

int score(char board[BOARD_SIZE][BOARD_SIZE], int PlayerColor)

{
	int opponentColor = BLACK;
	if (PlayerColor == BLACK)
		opponentColor = WHITE;
	int tie = 999;
	int result = 0;
	int winning = 1000;
	int loosing = -1000;
	
	int pawnScore = 1, knightBishopScore = 3, rookScore = 5;
	int queendScore = 9, kingScore = 400;
	if (checkForTie(board, PlayerColor) == 1)
	{
		return tie;
	}
	else if (isPlayerUnderMate(board, PlayerColor) == 1)
	{
		return loosing;
	}
	else if (isPlayerUnderMate(board, opponentColor) == 1)
	{
		return winning;
	}
	else
	{
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			for (int i = 0; i < BOARD_SIZE; i++)
			{
				if (PlayerColor == WHITE)
				{
					if (board[i][j] == WHITE_B || board[i][j] == WHITE_N)
						result = result + knightBishopScore;
					else if (board[i][j] == WHITE_P)
						result = result + pawnScore;
					else if (board[i][j] == WHITE_R)
						result = result + rookScore;
					else if (board[i][j] == WHITE_Q)
						result = result + queendScore;
					else if (board[i][j] == WHITE_K)
						result = result + kingScore;
				}
				else//player is black
				{
					if (board[i][j] == BLACK_B || board[i][j] == BLACK_N)
						result = result + knightBishopScore;
					else if (board[i][j] == BLACK_P)
						result = result + pawnScore;
					else if (board[i][j] == BLACK_R)
						result = result + rookScore;
					else if (board[i][j] == BLACK_Q)
						result = result + queendScore;
					else if (board[i][j] == BLACK_K)
						result = result + kingScore;
				}
			}
		}
		return result;
	}
}
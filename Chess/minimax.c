#include "minimax.h"

#define malloc(x) myMalloc(x)
#define free(x) myFree(x)
#define calloc(x,y) myCalloc(x,y)
#define realloc(x,y) myRealloc(x,y)

//recursive function for return the scoring result of the best move
int minimax(char board[BOARD_SIZE][BOARD_SIZE], int depth, Move** bestMove, 
	int alpha, int beta, int isMax, int boardCounter, int scoreDepth)
{
	//if depth  == -1 we are in best difficulty
	
	//int opponentColor = (playerColor == WHITE) ? BLACK : WHITE;
	MoveNode* moves;
	int newRes;
	int color = computerColor;
	if (isMax == 0)
		color = userColor;

	moves = getMoves(board, color);

	//check if no moves or depth is 0
	if (moves == NULL || depth == 0 || boardCounter == 1000000)
	{
		//print_board(board);
		int res = score(board, computerColor);
		freeMoves(moves, NULL);
		/*
		if (isMax == 0)
		res = res * (-1);
		*/

		return res;
	}
	else//############ let's generate minimax tree!
	{
		if (isMax == 1)//player is the computer
		{
			newRes = -10000;
			MoveNode* movesPointer = moves;
			while (movesPointer != NULL)
			{
				
				//char newBoard[BOARD_SIZE][BOARD_SIZE];
				performMoveMinimax(board, movesPointer->move);
				boardCounter++;
				//print_board(board);
				//print_board(newBoard);
				int temp = minimax(board, depth - 1, bestMove, alpha, beta, 0, boardCounter, depth - 1);
				UndoMove(board, movesPointer->move);

				newRes = max(newRes,temp);
				//insert prunning:
				if (newRes > alpha)
				{
					alpha = newRes;
					if (depth == minimax_depth || depth == scoreDepth)//check if we are in first recursion
						*(bestMove) = movesPointer->move;

				}
				//if (alpha >= beta)
				if (newRes >=beta)
				{
					freeMoves(moves, *(bestMove));
					return beta;//pruning
				}
				movesPointer = movesPointer->next;
			}

			freeMoves(moves, *(bestMove));
			return newRes;
		}
		else//player is the user:
		{
			newRes = 10000;
			MoveNode* movesPointer = moves;
			while (movesPointer != NULL)
			{
				//char newBoard[BOARD_SIZE][BOARD_SIZE];
				performMoveMinimax(board, movesPointer->move);
				
				boardCounter++;
				int temp = minimax(board, depth - 1, bestMove, alpha, beta, 1, boardCounter, depth - 1);
				UndoMove(board, movesPointer->move);
				newRes = min(newRes, temp);
				if (newRes < beta)
					beta = newRes;

				//if (alpha >= beta)
				if (newRes <= alpha)
				{
					freeMoves(moves, *(bestMove));
					return alpha;
				}

				movesPointer = movesPointer->next;
			}

			freeMoves(moves, *(bestMove));
			return newRes;
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
	int playerCounter = 0;
	int opponentCounter = 0;
	
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
				if (board[i][j] == WHITE_B || board[i][j] == WHITE_N)
				{
					if (PlayerColor == WHITE)
						playerCounter = playerCounter + knightBishopScore;
					else
						opponentCounter = opponentCounter + knightBishopScore;
				}
				else if (board[i][j] == WHITE_P)
				{
					if (PlayerColor == WHITE)
						playerCounter = playerCounter + pawnScore;
					else
						opponentCounter = opponentCounter + pawnScore;
				}
				else if (board[i][j] == WHITE_R)
				{
					if (PlayerColor == WHITE)
						playerCounter = playerCounter + rookScore;
					else
						opponentCounter = opponentCounter + rookScore;
				}
				else if (board[i][j] == WHITE_Q)
				{
					if (PlayerColor == WHITE)
						playerCounter = playerCounter + queendScore;
					else
						opponentCounter = opponentCounter + queendScore;
				}
				else if (board[i][j] == WHITE_K)
				{
					if (PlayerColor == WHITE)
						playerCounter = playerCounter + kingScore;
					else
						opponentCounter = opponentCounter + kingScore;
				}
				else if (board[i][j] == BLACK_B || board[i][j] == BLACK_N)
				{
					if (PlayerColor == BLACK)
						playerCounter = playerCounter + knightBishopScore;
					else
						opponentCounter = opponentCounter + knightBishopScore;
				}
				else if (board[i][j] == BLACK_P)
				{
					if (PlayerColor == BLACK)
						playerCounter = playerCounter + pawnScore;
					else
						opponentCounter = opponentCounter + pawnScore;
				}
				else if (board[i][j] == BLACK_R)
				{
					if (PlayerColor == BLACK)
						playerCounter = playerCounter + rookScore;
					else
						opponentCounter = opponentCounter + rookScore;
				}
				else if (board[i][j] == BLACK_Q)
				{
					if (PlayerColor == BLACK)
						playerCounter = playerCounter + queendScore;
					else
						opponentCounter = opponentCounter + queendScore;
				}
				else if (board[i][j] == BLACK_K)
				{
					if (PlayerColor == BLACK)
						playerCounter = playerCounter + kingScore;
					else
						opponentCounter = opponentCounter + kingScore;
				}
			}
		}
		//calculate the diff
		if (playerCounter == 0)
			result = loosing;
		else if (opponentCounter == 0)
			result = winning;
		else
			result= playerCounter- opponentCounter;
		return result;
	}
}


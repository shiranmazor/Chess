#include "minimax.h"

#define malloc(x) myMalloc(x)
#define free(x) myFree(x)
#define calloc(x,y) myCalloc(x,y)
#define realloc(x,y) myRealloc(x,y)

int countKings2(char board[BOARD_SIZE][BOARD_SIZE])
{
	int kingsCounter = 0;
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			if (board[i][j] == WHITE_K || board[i][j] == BLACK_K)
				kingsCounter++;
		}
	}
	return kingsCounter;
}
//recursive function for return the scoring result of the best move
int minimax(char board[BOARD_SIZE][BOARD_SIZE], int depth, Move** bestMove, 
	int alpha, int beta, int isMax, int boardCounter)
{
	//if depth  == -1 we are in best difficulty
	
	MoveNode* moves;
	int mate;
	int oponnentColor;
	int newRes;
	int kings;
	int color = computerColor;
	if (isMax == 0)
		color = userColor;
	
	
	/*
	kings = countKings2(board);
	if (kings < 2)
	mate = 1;
	*/
	

	oponnentColor = color == WHITE ? BLACK : WHITE;
	//if the next moves can lead to chessmate - eating the opponent king
	mate = isPlayerUnderMate(board, oponnentColor);
	moves = getMoves(board, color);
	
	//check if no moves or depth is 0
	if (moves == NULL || depth == 0 || boardCounter == 1000000 || mate ==1)
	{
		int res = score(board, computerColor);
		freeMoves(moves, NULL);

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
				
				performMoveMinimax(board, movesPointer->move);
				boardCounter++;
				int temp = minimax(board, depth - 1, bestMove, alpha, beta, 0, boardCounter);
				UndoMove(board, movesPointer->move);

				newRes = max(newRes,temp);
				//insert prunning:
				if (newRes > alpha)
				{
					alpha = newRes;
					if (depth == minimax_depth)//check if we are in first recursion
						*(bestMove) = movesPointer->move;

				}
				//if (alpha >= beta)
				if (newRes >=beta )
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
				int temp = minimax(board, depth - 1, bestMove, alpha, beta, 1, boardCounter);
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
	if (isPlayerUnderMate(board, opponentColor) == 1)
	{
		return winning;
	}
	else if (checkForTie(board, PlayerColor) == 1)
	{
		return tie;
	}
	else if (isPlayerUnderMate(board, PlayerColor) == 1)
	{
		return loosing;
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


#include "minimax.h"

//recursive function for return the scoring result of the best move
/*
int minimax(char board[BOARD_SIZE][BOARD_SIZE], int depth, int isMaxplayer, Move** bestMove)
{
char playerm;
char playerk;
char direction;

int player_color = WHITE;
//the computer is always max player
if (isMaxplayer == 1)
{
player_color = computer_color;
playerk = game_players.computer_k;
playerm = game_players.computer_m;
direction = game_players.computer_direction;
}
else
{
if (computer_color == WHITE)
player_color = BLACK;
playerk = game_players.user_k;
playerm = game_players.user_m;
direction = game_players.user_direction;
}
//get user and
MoveNode* moves = getMoves(board, playerm, playerk, direction);

//check if no moves or depth is 0
if (moves == NULL || depth == 0)
{
//print_board(board);
int res = score(board, player_color);
freeMoves(moves, NULL);
return res*(-1);
}
else//############ let's generate minimax tree!
{
if (isMaxplayer == 1)//player is the computer
{
int bestValue = -100;
MoveNode* movesPointer = moves;
while (movesPointer != NULL)
{
int newRes = 0;
char newBoard[BOARD_SIZE][BOARD_SIZE];
performMove(board, newBoard, *(movesPointer->move), direction);
//print_board(board);
//print_board(newBoard);
newRes = minimax(newBoard, depth - 1, 0, bestMove);


//check if we are in first recursion
if (depth == minimax_depth && bestValue < newRes)
{
bestValue = newRes;
*(bestMove) = movesPointer->move;

}
else if (bestValue < newRes)
bestValue = newRes;

movesPointer = movesPointer->next;
}

if (!*(bestMove) && depth == minimax_depth)//all the  moves are bad
*(bestMove) = moves->move;

freeMoves(moves, *(bestMove));
return bestValue;
}
else//player is the user:
{
int bestValue = 100;
MoveNode* movesPointer = moves;
while (movesPointer != NULL)
{
int newRes = 0;
char newBoard[BOARD_SIZE][BOARD_SIZE];
performMove(board, newBoard, *(movesPointer->move), direction);
//print_board(board);
//print_board(newBoard);

newRes = minimax(newBoard, depth - 1, 1, bestMove);
if (bestValue > newRes)
bestValue = newRes;


movesPointer = movesPointer->next;
}

freeMoves(moves, *(bestMove));
return bestValue;
}

}
freeMoves(moves, *(bestMove));
}

*/

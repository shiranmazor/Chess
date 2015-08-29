#ifndef CHESS_
#define CHESS_

#include<stdio.h>
#include "ChessHelp.h"
#include "FileHandle.h"

#define WHITE  1
#define BLACK  0

#define WHITE_P 'm'
#define WHITE_B 'b'
#define WHITE_N 'n'
#define WHITE_R 'r'
#define WHITE_Q 'q'
#define WHITE_K 'k'

#define BLACK_P 'M'
#define BLACK_B 'B'
#define BLACK_N 'N'
#define BLACK_R 'R'
#define BLACK_Q 'Q'
#define BLACK_K 'K'


#define EMPTY ' '

#define BOARD_SIZE 8

//global variables:
char board[BOARD_SIZE][BOARD_SIZE];
int gameMode = 1;//1- two players, 2- playey vs AI(computer)
int minimax_depth = 1; //default - 1, regular between 1-4, best -0
int userColor = WHITE;//relevent only in player vs AI
int startPlayer = WHITE;
//game structs:
typedef struct Pos Pos;
typedef struct PosNode PosNode;
typedef struct MoveNode MoveNode;
typedef struct Move Move;
struct PosNode
{
	Pos *pos;
	PosNode *next;
};

struct Pos
{
	//char x;
	int x;
	int y;
};

struct MoveNode
{
	Move *move;
	MoveNode *next;

};

struct Move
{
	Pos *currPos;
	int eat;
	PosNode *dest;
};


// board initialization
#define ENTER_SETTINGS "Enter game settings:\n" 
#define WRONG_GAME_MODE "Wrong game mode\n"
#define TWO_PLAYERS_GAME_MODE "Running game in 2 players mode\n"
#define PLAYER_VS_AI_GAME_MODE "Running game in player vs. AI mode\n"
#define WRONG_MINIMAX_DEPTH "Wrong value for minimax depth. The value should be between 1 to 4\n"
#define WRONG_FILE_NAME "Wrong file name\n"
#define WRONG_POSITION "Invalid position on the board\n"
#define NO_PIECE "Setting this piece creates an invalid board\n"  
#define WROND_BOARD_INITIALIZATION "Wrong board initialization\n"

#define ILLEGAL_COMMAND "Illegal command, please try again\n"
#define ILLEGAL_MOVE "Illegal move\n"

#define WRONG_ROOK_POSITION "Wrong position for a rook\n" 
#define ILLEGAL_CALTLING_MOVE "Illegal castling move\n"  

#define TIE "The game ends in a tie\n"
#define MATE_WHITE "Mate! white player wins the game\n"
#define MATE_BLACK "Mate! black player wins the game\n"
#define perror_message(func_name) (fprintf(stderr, "Error: standard function %s has failed\n", func_name));
#define print_message(message) (printf("%s", message));


//functions declarations:

void freeMove(Move *move);
void freeMoveWithoutDest(MoveNode *moveNode);
void freeMoves(MoveNode *moveNodeHead, Move* notDelete);
void freeMoveNode(MoveNode *moveNode);
int isValidPos(Pos *pos);
int isValidIndexes(int i, int j);
Pos * formatPos(char* pos_input);

char* getStringFormatMove(Move move);
char* getStringFormatPos(Pos* pos);
int checkifPlayerWins(int player_color);
int performMove(Move move);
MoveNode * getMoves(char board[BOARD_SIZE][BOARD_SIZE]);
void addMoveNodeToList(MoveNode **movesList, MoveNode * moveNode);
MoveNode *createMoveNode(Pos pos, Pos destPos);
MoveNode *getRookMoves(Pos pos, char board[BOARD_SIZE][BOARD_SIZE], int playerColor);
int checkForTie(int playerColor);
int isPlayerUnderCheck(int playerColor);
int isPlayerUnderMate(int playerColor);
int isPlayerStuck(int playerColor);
int isPlayerLose(int playerColor);
Pos* getKingPos(int playerColor);

//check validation help functions:
int checkRookThreat(int oponnentColor, Pos *kingPos);
int checkBishopThreat(int oponnentColor, Pos *kingPos);
int checkKnightThreat(int oponnentColor, Pos *kingPos);
int checkPawnThreat(int oponnentColor, Pos *kingPos);
int checkQueenThreat(int oponnentColor, Pos *kingPos);
int checkKingThreat(int oponnentColor, Pos *kingPos);


#endif CHESS_;
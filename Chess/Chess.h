#ifndef CHESS_
#define CHESS_

#include<stdio.h>

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

struct Move{
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

#define perror_message(func_name) (perror("Error: standard function %s has failed", func_name));
#define print_message(message) (printf("%s", message));


//functions declarations:
void print_board(char board[BOARD_SIZE][BOARD_SIZE]);
void print_line();
void clear_board();
void init_board(char board[BOARD_SIZE][BOARD_SIZE]);
void freeMove(Move *move);
void freeMoveWithoutDest(MoveNode *moveNode);
void freeMoves(MoveNode *moveNodeHead, Move* notDelete);
void freeMoveNode(MoveNode *moveNode);
int isValidPos(Pos *pos);
Pos * formatPos(char* pos_input);
int boardInitializeOk();


char* getStringFormatMove(Move move);
char* getStringFormatPos(Pos* pos);
int checkifPlayerWins(int player_color);
int performMove(Move move);
MoveNode * getMoves(char board[BOARD_SIZE][BOARD_SIZE]);
void addMoveNodeToList(MoveNode **movesList, MoveNode **last, MoveNode * moveNode);
MoveNode *createMoveNode(Pos pos, Pos destPos, int eat);
#endif CHESS_
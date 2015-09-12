#include "Console.h"


void print_line()
{
	int i;
	printf("  |");
	for (i = 1; i < BOARD_SIZE * 4; i++){
		printf("-");
	}
	printf("|\n");
}

void print_board(char board[BOARD_SIZE][BOARD_SIZE])
{
	int i, j;
	print_line();
	for (j = BOARD_SIZE - 1; j >= 0; j--)
	{
		printf((j < 9 ? " %d" : "%d"), j + 1);
		for (i = 0; i < BOARD_SIZE; i++){
			printf("| %c ", board[i][j]);
		}
		printf("|\n");
		print_line();
	}
	printf("   ");
	for (j = 0; j < BOARD_SIZE; j++){
		printf(" %c  ", (char)('a' + j));
	}
	printf("\n");
}

void clear_board()
{
	int i, j;
	for (i = 0; i < BOARD_SIZE; i++)
	{
		for (j = 0; j < BOARD_SIZE; j++)
		{
			if (board[i][j] != EMPTY)
				board[i][j] = EMPTY;
		}
	}
}


void init_board(char board[BOARD_SIZE][BOARD_SIZE])
{
	int i, j;

	//init first and last line with - j=0, and 7
	//with bishp,rook,knight, queen, kingr
	board[0][0] = WHITE_R;
	board[1][0] = WHITE_N;
	board[2][0] = WHITE_B;
	board[3][0] = WHITE_K;
	board[4][0] = WHITE_Q;
	board[5][0] = WHITE_B;
	board[6][0] = WHITE_N;
	board[7][0] = WHITE_R;
	//line =7, black player
	board[0][7] = BLACK_R;
	board[1][7] = BLACK_N;
	board[2][7] = BLACK_B;
	board[3][7] = BLACK_K;
	board[4][7] = BLACK_Q;
	board[5][7] = BLACK_B;
	board[6][7] = BLACK_N;
	board[7][7] = BLACK_R;

	//init pawns
	for (int k = 0; k < 2; k++)
	{
		if (k == 0)
			j = 1;
		else
			j = 6;
		for (i = 0; i < BOARD_SIZE; i++)
		{
			if (j == 1)
				board[i][j] = WHITE_P;
			if (j == 6)
				board[i][j] = BLACK_P;

		}
	}
	
}

void settingState()
{
	char input[50];
	while (1)
	{
		if (printf("%s", ENTER_SETTINGS) < 0)
		{
			perror_message("settingState");
			exit(0);
		}
		scanf("%s", input);
		reduceSpaces(input);
		if (strcmp(input, "quit") != 0 && strcmp(input, "start") != 0)
			executeSettingCmd(input);
		else
		{
			if (strcmp(input, "start") == 0)
			{
				//check if board is initialize:
				int ok = boardInitializeOk();
				if (ok == 0)
				{
					printf("%s", WROND_BOARD_INITIALIZATION);
				}
				else
				{
					//call game state on the board
					GameState(board);
				}

			}
			else if (strcmp(input, "quit") == 0)
			{
				//TODO:clean all memory
				exit(0);
			}
		}
	}
}

void executeSettingCmd(char* input)
{
	//trim all spaces from start and end:
	input = trimwhitespace(input);
	char **arr = NULL;
	int arr_len = split(input, ' ', &arr);
	if (strstr(input, "game_mode"))
	{
		//arr len in 2:
		int num = atoi(arr[1]);
		if (num != 1 || num != 2)
		{
			printf("%s", WRONG_GAME_MODE);
		}
		else
		{
			gameMode = num;
			if (gameMode == 1) 
				printf("%s", TWO_PLAYERS_GAME_MODE);
			else
				printf("%s", PLAYER_VS_AI_GAME_MODE);
		}
	}
	else if (strstr(input, "difficulty"))
	{
		if (gameMode != 2)
		{
			//ilegal command
			printf("%s", ILLEGAL_COMMAND);
		}
		else if (strstr(input, "depth"))
		{
			int depth = atoi(arr[2]);
			if (depth <1 || depth > 4)
				printf("%s", WRONG_MINIMAX_DEPTH);
			else
				minimax_depth = depth;
		}
		else if (strstr(input, "best"))
		{
			minimax_depth = -1;
		}
	}
	else if (strstr(input, "user_color"))
	{
		if (gameMode != 2)
		{
			//ilegal command
			printf("%s", ILLEGAL_COMMAND);
		}
		else
		{
			if (strcmp(arr[1], "white") == 0)
				userColor = WHITE;
			else if (strcmp(arr[1], "black") == 0)
				userColor = BLACK;
		}
	}
	else if (strstr(input, "load"))
	{
		int filePathLen = strlen(arr[1]);
		char* filePath = arr[1];

	}
	else if (strstr(input, "clear"))
	{
		//clear the board
		clear_board();
	}
	else if (strstr(input, "next_player"))
	{
		if (strcmp(arr[1], "black"))
			startPlayer = BLACK;
		//else white player is already defined as default
	}
	else if (strstr(input, "rm"))
	{
		remove_disc(arr[1]);
	}
	else if (strstr(input, "set"))
	{
		set_disc(arr[1], arr[2], arr[3]);
	}
	else if (strstr(input, "print"))
	{
		print_board(board);
	}
	else if (strstr(input, "quit"))
	{
		exit(0);
	}
	else if (strstr(input, "start"))
	{
		
		//check board init
		int kings = countKings();
		if (kings != 2)
		{
			printf("%s", WROND_BOARD_INITIALIZATION);
		}
		else if (checkForTie(board, startPlayer) == 1)
		{
			printf("%s", TIE);
			exit(0);
		}
		else if (isPlayerUnderMate(board, startPlayer) == 1)
		{
			if (startPlayer == WHITE)
				printf("%s", MATE_WHITE);
			else
				printf("%s", MATE_BLACK);
			exit(0);
		}
		else
		{
			//moving to game state:
			GameState();
		}
		
	}
}

int countKings()
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

int checkNewBoardValidation(int color, char* Tool)
{
	//define counters
	int rooks, bishops, knights, queens, kings, pawn = 0;
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			if (userColor == WHITE)
			{
				switch (board[i][j])
				{
				case WHITE_B:
					bishops++;
					break;
				case WHITE_K:
					kings++;
					break;
				case WHITE_N:
					knights++;
					break;
				case WHITE_P:
					pawn++;
				case WHITE_Q:
					queens++;
					break;
				case WHITE_R:
					rooks++;
					break;
				default:
					break;
				}
			}
			else
			{
				switch (board[i][j])
				{
				case BLACK_B:
					bishops++;
					break;
				case BLACK_K:
					kings++;
					break;
				case BLACK_N:
					knights++;
					break;
				case BLACK_P:
					pawn++;
				case BLACK_Q:
					queens++;
					break;
				case BLACK_R:
					rooks++;
					break;
				default:
					break;
				}
			}
			
		}
	}
	//check if current tool is valid
	if (strcmp(Tool, "king") == 0)
	{
		if (kings < 1)
			return 1;
		else
			return 0;
	}
	if (strcmp(Tool, "queen") == 0)
	{
		if (queens < 1)
			return 1;
		else
			return 0;
	}
	if (strcmp(Tool, "rook") == 0)
	{
		if (rooks < 2)
			return 1;
		else
			return 0;
	}
	if (strcmp(Tool, "knight") == 0)
	{
		if (knights < 2)
			return 1;
		else
			return 0;
	}
	if (strcmp(Tool, "bishop") == 0)
	{
		if (bishops < 2)
			return 1;
		else
			return 0;
	}
	if (strcmp(Tool, "pawn") == 0)
	{
		if (pawn < 8)
			return 1;
		else
			return 0;
	}
	return 0;
}
void set_disc(char* pos_input, char* color, char* type)
{
	int col;
	if (strcmp(color, "black") == 0)
		col = BLACK;
	else
		col = WHITE;

	Pos *pos = formatPos(pos_input);
	if (pos == NULL)
	{
		return;
	}
	else
	{
		if (!checkNewBoardValidation(col, type))
		{
			printf("%s", NO_PIECE);
			free(pos);
			return;
		}
		if (col == BLACK)
		{
			if (strcmp(type, "king") == 0)
				board[pos->x][pos->y] = BLACK_K;
			else if (strcmp(type, "queen") == 0)
				board[pos->x][pos->y] = BLACK_Q;
			else if (strcmp(type, "rook") == 0)
				board[pos->x][pos->y] = BLACK_R;
			else if (strcmp(type, "bishop") == 0)
				board[pos->x][pos->y] = BLACK_B;
			else if (strcmp(type, "knight") == 0)
				board[pos->x][pos->y] = BLACK_N;
			else if (strcmp(type, "pawn") == 0)
				board[pos->x][pos->y] = BLACK_P;
		}
		else if (col == WHITE)
		{
			if (strcmp(type, "king") == 0)
				board[pos->x][pos->y] = WHITE_K;
			else if (strcmp(type, "queen") == 0)
				board[pos->x][pos->y] = WHITE_Q;
			else if (strcmp(type, "rook") == 0)
				board[pos->x][pos->y] = WHITE_R;
			else if (strcmp(type, "bishop") == 0)
				board[pos->x][pos->y] = WHITE_B;
			else if (strcmp(type, "knight") == 0)
				board[pos->x][pos->y] = WHITE_N;
			else if (strcmp(type, "pawn") == 0)
				board[pos->x][pos->y] = WHITE_P;
		}
	}
	free(pos);
}

void remove_disc(char* input)
{
	//input is <x,y>
	Pos *pos = formatPos(input);

	if (pos)
	{
		int x_int = pos->x;
		board[x_int][pos->y] = EMPTY;
	}
	free(pos);
}

void GameState()
{
	int resComputer = 0;
	int resUser1 = 0;
	int resUser2 = 0;
	if (gameMode == 1)//player vs player
	{

	}
	else if (gameMode == 2)
	{

	}
}

int boardInitializeOk()
{
	return 0;
}
int main()
{
	gameMode = 1;
	minimax_depth = 1;
	userColor = WHITE;
	startPlayer = WHITE;
	init_board(board);
	print_board(board);
	settingState();
	return 0;
}


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

/*
initialize the board for starting chess gamse, white player is always down
*/
void init_board(char board[BOARD_SIZE][BOARD_SIZE])
{
	int i, j;

	//init first and last line with - j=0, and 7
	//with bishp,rook,knight, queen, king
	//line=0 white player
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
			minimax_depth = 0;
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
		//call file handle
	}
}
int main()
{
	init_board(board);
	print_board(board);
	return 0;
}
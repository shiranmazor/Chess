#include "Console.h"
#include "fileHandle.h"

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
	
	for (int f = 0; f < BOARD_SIZE; f++)
	{
		for (int s = 0; s < BOARD_SIZE; s++)
			board[f][s] = EMPTY;
	}

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
		printf("%s", ENTER_SETTINGS);
		scanf("%s", input);
		reduceSpaces(input);
		if (strcmp(input, "quit") != 0 && strcmp(input, "start") != 0)
			executeSettingCmd(input);
		else
		{
			if (strcmp(input, "start") == 0)
			{
				//check if board is initialize:
				int kings = countKings();
				if (kings != 2)
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
		//check if file exist! Todo:
		GameStatus gStatus = readFile(2);
		//load game
		gameMode = gStatus.gameMode;
		strcpy(board, gStatus.board);
		userColor = gStatus.userColor;
		//minimax_depth = gStatus.difficulty;
		nextPlayer = gStatus.nextTurn;
		print_board(board);


	}
	else if (strstr(input, "clear"))
	{
		//clear the board
		clear_board();
	}
	else if (strstr(input, "next_player"))
	{
		if (strcmp(arr[1], "black"))
			nextPlayer = BLACK;
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
		else if (checkForTie(board, nextPlayer) == 1)
		{
			printf("%s", TIE);
			exit(0);
		}
		else if (isPlayerUnderMate(board, nextPlayer) == 1)
		{
			if (nextPlayer == WHITE)
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
	
	/*
	int resComputer = 0;
	int resUser1 = 0;
	int resUser2 = 0;
	*/	
		
	if (gameMode == 1)//player vs player
	{
		GameTwoPlayers();
	}

	else if (gameMode == 2)
	{
		//player vs computer
		int computerColor = BLACK;
		if (userColor == BLACK)
			computerColor == WHITE;
		GameUserVsComputer(computerColor);

	}
	
}

void GameUserVsComputer(int computerColor)
{
	int insideGameLoop = 1;
	int winningPlayerColor;
	while (insideGameLoop)
	{

		//check if computer or user are playing:
		if (nextPlayer == computerColor)
		{
			//play computer move
			ComputerMove(computerColor);
			nextPlayer = userColor;
		}
		else
		{
			//user move:

			UserMove(userColor);
			nextPlayer = computerColor;

			
		}
		
	}

}
void GameTwoPlayers()
{

}

/*return the computer results after playing a move - 1-win, 0-not win yet*/
int ComputerMove(int computerColor)
{

}

/*return the user result after performing  the move 1-win, 0-not win yet*/
int UserMove(int userColor)
{
	char input[50];
	int MoveDone = 0;
	while (MoveDone == 0)//didn't perform any move, continuing loop
	{
		if (userColor == WHITE)
			printf("%s", "WHITE player - enter your move:\n");
		else
			printf("%s", "BLACK player - enter your move:\n");
		scanf("%s", input);
		reduceSpaces(input);
		if (StartsWith(input, "move"))
		{
			Move *move = parseMoveCommand(input);//Invalid position or color on the board- move==NULL
			if (move == NULL)
				continue;
			else
			{
				//check if the move is legal
				if (isMoveLegal(move, userColor) == 0)
				{
					printf("%s", ILLEGAL_MOVE);
					freeMove(move);
					continue;
				}
				else
				{
					//perform move
					performUserMove(move);
					MoveDone = 1;
					freeMove(move);
				}
			}
		}
		else if (StartsWith(input, "get_moves"))
		{
			char **arr = NULL;
			int arr_len = split(input, ' ', &arr);
			Pos* pos = formatPos(arr[1]);
			if (pos == NULL)
				continue;
			if (getColorByPos(pos->x, pos->y) != userColor)
			{
				printf("%s", WRONG_POS_COLOR);
				free(pos);
				continue;
			}
			//else
			MoveNode *moves = getMove(board, *pos, userColor);
			if (moves != NULL)
			{
				printMoves(moves);
			}
			free(arr);
			freeMoves(moves, NULL);
		}
		else if (StartsWith(input, "get_best_moves"))
		{
			char **arr = NULL;
			int arr_len = split(input, ' ', &arr);
			int d = atoi(arr[1]);
			free(arr);
			//get all move and check their score by minimax
			MoveNode* highestMoves = NULL;
			MoveNode* moves = getMoves(board, userColor);
			if (moves == NULL)
				continue;
			MoveNode* movesPointer = moves;
			int maxRes = 0;
			while (movesPointer != NULL)
			{
				int res = getMoveScore(movesPointer->move, d, userColor);
				if (res > maxRes)
				{
					if (highestMoves != NULL)
					{
						freeMoves(highestMoves, NULL);
						highestMoves = NULL;
					}
					//replace all elements in the list
					MoveNode * moveNode = createMoveNode(*(movesPointer->move->currPos), *(movesPointer->move->dest->pos));
					addMoveNodeToList(&highestMoves, moveNode);
				}
				else if (res == maxRes)
				{
					MoveNode * moveNode = createMoveNode(*(movesPointer->move->currPos), *(movesPointer->move->dest->pos));
					addMoveNodeToList(&highestMoves, moveNode);
				}
				movesPointer = movesPointer->next;
			}
			printMoves(highestMoves);
		}
		else if (StartsWith(input, "get_score"))
		{
			char **arr = NULL;
			int arr_len = split(input, ' ', &arr);
			int d = atoi(arr[1]);
			free(arr);
			//get_score d move <x,y> to <i,j> x
			str_cut(input, 0, 12);
			Move* move = parseMoveCommand(input);
			int res = getMoveScore(move, d, userColor);
			printf("%d", res);

		}
		else if (StartsWith(input, "save"))
		{
			//create gamseState struct:
			GameStatus status;
			strcpy(status.board, board);
			status.userColor = userColor;
			status.gameMode = gameMode;
			status.nextTurn = nextPlayer;
			//status.difficulty=minimax_depth;
			//Todo Change!!
			saveFile(status, 2);

		}
		else if (StartsWith(input, "quit"))
		{
			exit(0);
		}
	}
	//outside move loop, move has compelted:
	print_board(board);
	//check mate or a tie
	if (checkForTie(board, userColor))
	{
		printf("%s", TIE);
		exit(0);
	}
	if (isPlayerUnderMate(board, WHITE) == 1)
	{
		printf("%s", MATE_BLACK);
		exit(0);
	}
	else if (isPlayerUnderMate(board, BLACK) ==1)
	{
		printf("%s", MATE_WHITE);
		exit(0);
	}
	int opponentColor = (userColor == BLACK) ? WHITE : BLACK;
	if (isPlayerUnderCheck(board, opponentColor) == 1)
	{
		printf("%s", "Check!\n");
	}

	return 0;
}

int getMoveScore(Move *move, int d, int userColor)
{
	if (isMoveLegal(move, userColor) == 0)
	{
		printf("%s", ILLEGAL_MOVE);
		return NULL;
	}
	int opponentColor = (userColor == BLACK) ? WHITE : BLACK;
	char newBoard[BOARD_SIZE][BOARD_SIZE];
	copyBoard(board, newBoard);
	performMoveMinimax(board, newBoard, move);
	//newBoard is changes - run minimax
	Move* bestMove = NULL;
	int res = minimax(newBoard, d - 1, opponentColor, &bestMove, -9999, 9999, 1, 0);
	freeMove(bestMove);
	return res;
}

void getMovesUnitTests()
{
	MoveNode * movesList = getMoves(board, 0);
	//markMoves(board, movesList);
	//print_board(board);

	movesList = getMoves(board, 1);

/*	for (int i = 0; i < 8; i++)
	{
		assert(movesList->move->currPos->x == i);
		assert(movesList->move->currPos->y == 1);
		assert(movesList->move->dest->pos->x == i);
		assert(movesList->move->dest->pos->y == 2);
		movesList = movesList->next;
	}
	assert(movesList == NULL);
	*/
	clear_board();

	board[4][4] = WHITE_R;
	
	movesList = getMoves(board, WHITE);

	/*
		move <4,4> to <4,3>
		move <4,4> to <4,2>
		move <4,4> to <4,1>
		move <4,4> to <4,0>
		move <4,4> to <3,4>
		move <4,4> to <2,4>
		move <4,4> to <1,4>
		move <4,4> to <0,4>
		move <4,4> to <5,4>
		move <4,4> to <6,4>
		move <4,4> to <7,4>
		move <4,4> to <4,5>
		move <4,4> to <4,6>
		move <4,4> to <4,7>
	*/

	board[4][4] = WHITE_B;
	movesList = getMoves(board, WHITE);
	//printMoves(movesList);

	//board[4][4] = WHITE_K;
	//movesList = getMoves(board, WHITE);
	//printMoves(movesList);
	//markMoves(board,movesList);
	//print_board(board);
	init_board(board);
	board[1][1] = BLACK_N;
	movesList = getMoves(board, BLACK);
	//printMoves(movesList);
	markMoves(board, movesList);
	//print_board(board);


	clear_board();

	board[4][4] = WHITE_P;
	board[5][5] = WHITE_P;
	board[4][5] = WHITE_P;
	print_board(board);
	movesList = getMoves(board, WHITE);
	markMoves(board, movesList);
	print_board(board);
	clear_board();

	board[4][4] = WHITE_B;
	board[5][5] = WHITE_P;
	board[4][5] = WHITE_P;
	board[2][2] = BLACK_Q;
	print_board(board);
	movesList = getMoves(board, WHITE);
	markMoves(board, movesList);
	print_board(board);
	clear_board();

	board[4][4] = WHITE_K;
	board[5][5] = WHITE_P;
	board[4][5] = WHITE_P;
	board[3][3] = BLACK_B;
	print_board(board);
	movesList = getMoves(board, WHITE);
	markMoves(board, movesList);
	print_board(board);
	clear_board();

	board[4][4] = WHITE_N;
	board[5][5] = WHITE_P;
	board[4][5] = WHITE_P;
	

	print_board(board);
	movesList = getMoves(board, WHITE);
	markMoves(board, movesList);
	print_board(board);
	clear_board();

	board[4][4] = WHITE_Q;
	board[5][5] = WHITE_P;
	board[4][5] = WHITE_P;
	print_board(board);
	movesList = getMoves(board, WHITE);
	markMoves(board, movesList);
	print_board(board);
	clear_board();

	board[4][4] = WHITE_R;
	board[5][5] = WHITE_P;
	board[4][5] = WHITE_P;
	print_board(board);
	movesList = getMoves(board, WHITE);
	markMoves(board, movesList);
	print_board(board);
	clear_board();

	movesList = getMoves(board, BLACK);
	assert(movesList == NULL);

	clear_board();

	board[4][4] = BLACK_P;
	board[5][5] = BLACK_P;
	board[4][5] = BLACK_P;
	print_board(board);
	movesList = getMoves(board, BLACK);
	markMoves(board, movesList);
	print_board(board);
	clear_board();

	board[4][4] = BLACK_B;
	board[5][5] = BLACK_P;
	board[4][5] = BLACK_P;
	print_board(board);
	movesList = getMoves(board, BLACK);
	markMoves(board, movesList);
	print_board(board);
	clear_board();

	board[4][4] = BLACK_K;
	board[5][5] = BLACK_P;
	board[4][5] = BLACK_P;
	print_board(board);
	movesList = getMoves(board, BLACK);
	markMoves(board, movesList);
	print_board(board);
	clear_board();

	board[4][4] = BLACK_N;
	board[5][5] = BLACK_P;
	board[4][5] = BLACK_P;
	print_board(board);
	movesList = getMoves(board, BLACK);
	markMoves(board, movesList);
	print_board(board);
	clear_board();

	board[4][4] = BLACK_Q;
	board[5][5] = BLACK_P;
	board[4][5] = BLACK_P;
	print_board(board);
	movesList = getMoves(board, BLACK);
	markMoves(board, movesList);
	print_board(board);
	clear_board();

	board[4][4] = BLACK_R;
	board[5][5] = BLACK_P;
	board[4][5] = BLACK_P;
	print_board(board);
	movesList = getMoves(board, BLACK);
	markMoves(board, movesList);
	print_board(board);
	clear_board();

	board[4][4] = BLACK_P;
	board[4][3] = WHITE_N;
	board[5][3] = WHITE_B;
	print_board(board);
	movesList = getMoves(board, BLACK);
	markMoves(board, movesList);
	print_board(board);
	clear_board();
}

void markMoves(char board[BOARD_SIZE][BOARD_SIZE], MoveNode * movesList)
{
	while (movesList != NULL)
	{
		board[movesList->move->dest->pos->x][movesList->move->dest->pos->y] = 'X';
		movesList = movesList->next;
	}
}

void printMoves(MoveNode *movesList)
{
	while (movesList != NULL)
	{
		printf("move <%d,%d> to <%d,%d>\n", movesList->move->currPos->x, movesList->move->currPos->y, movesList->move->dest->pos->x, movesList->move->dest->pos->y);
		movesList = movesList->next;
	}
}
int main()
{
	//first initializetion
	gameMode = 1;
	minimax_depth = 1;
	userColor = WHITE;
	nextPlayer = WHITE;
	pawnPromotionTool = -1000;//queen
	init_board(board);
	print_board(board);


	GameStatus gameState;
	gameState.nextTurn = WHITE;
	gameState.userColor = BLACK;
	gameState.difficulty = -1;
	gameState.gameMode = 0;
	memcpy(gameState.board, board, sizeof(board));
	saveFile(gameState, 1);
	GameStatus gs2 = readFile(1);
	GameStatus gs = readFile(3);
	saveFile(gs, 4);
	getMovesUnitTests();
	settingState();
	
	

	return 0;
}


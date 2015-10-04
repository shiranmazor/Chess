#include "Console.h"


#define malloc(x) myMalloc(x)
#define free(x) myFree(x)
#define calloc(x,y) myCalloc(x,y)
#define realloc(x,y) myRealloc(x,y)


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
		fgets(input, 50, stdin);
		reduceSpaces(input);
		strcpy(input, str_replace(input, "\n", ""));
		if (strcmp(input, "quit") != 0 && strcmp(input, "start") != 0)
			executeSettingCmd(input);
		else
		{
			if (strcmp(input, "start") == 0)
			{
				// check board init
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
	split(input, ' ', &arr);
	if (strstr(input, "game_mode"))
	{
		//arr len in 2:
		int num = atoi(arr[1]);
		if (num != 1 && num != 2)
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
			minimax_depth = 4;
		}
		else
		{
			printf("%s", ILLEGAL_COMMAND);
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
			{
				userColor = WHITE;
				computerColor = BLACK;
			}
			else if (strcmp(arr[1], "black") == 0)
			{
				userColor = BLACK;
				computerColor = WHITE;
			}
				
		}
	}
	else if (strstr(input, "load"))
	{
		clear_board();
		char* filePath = arr[1];
		//check if file exist! Todo:
		GameStatus gStatus = readFileWithFilename(filePath);
		if (gStatus.nextTurn != -1)
		{
			//load game
			gameMode = gStatus.gameMode;
			copyBoard(gStatus.board, board);
			//strcpy(board, gStatus.board);
			userColor = gStatus.userColor;
			minimax_depth = gStatus.difficulty;
			nextPlayer = gStatus.nextTurn;
			print_board(board);
		}
		
	}
	else if (strstr(input, "clear"))
	{
		//clear the board
		clear_board();
	}
	else if (strstr(input, "next_player"))
	{
		if (strcmp(arr[1], "black") == 0)
			nextPlayer = BLACK;
		else
			nextPlayer = WHITE;
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
	else
	{
		printf("%s", ILLEGAL_COMMAND);
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

/*return 1 in case board is valid*/
int checkNewBoardValidation(int color, char* Tool)
{
	//define counters
	int rooks =0, bishops=0, knights=0, queens=0, kings=0, pawn = 0;
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			if (color == WHITE)
			{
				if (board[i][j] == WHITE_B)
					bishops++;
				else if (board[i][j] == WHITE_K)
					kings++;
				else if (board[i][j] == WHITE_N)
					knights++;
				else if (board[i][j] == WHITE_P)
					pawn++;
				else if (board[i][j] == WHITE_Q)
					queens++;
				else if (board[i][j] == WHITE_R)
					rooks++;
			}
			else
			{
				if( board[i][j] == BLACK_B)
					bishops++;
				else if (board[i][j] == BLACK_K)
					kings++;
				else if (board[i][j] == BLACK_N)
					knights++;
				else if (board[i][j] == BLACK_P)
					pawn++;
				else if (board[i][j] == BLACK_Q)
					queens++;
				else if (board[i][j] == BLACK_R)
					rooks++;
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
	if (gameMode == 1)//player vs player
	{
		GameTwoPlayers();
	}

	else if (gameMode == 2)
	{
		//player vs computer
		computerColor = (userColor == BLACK) ?WHITE:BLACK;
		GameUserVsComputer(computerColor);

	}
	
}

void GameUserVsComputer()
{
	int insideGameLoop = 1;
	while (insideGameLoop)
	{
		//check if computer or user are playing:
		if (nextPlayer == computerColor)
		{
			//play computer move
			ComputerMove();
		}
		else
		{
			//user move:
			UserMove(userColor);
		}

	}
}
void GameTwoPlayers()
{
	int insideGameLoop = 1;
	while (insideGameLoop)
	{
		UserMove(nextPlayer);
	}
}

/*return the computer results after playing a move - 1-win, 0-not win yet*/
int ComputerMove()
{
	//call minimax algorithm
	Move* computerMove = NULL;
	int opponentColor = (computerColor == BLACK) ? WHITE : BLACK;
	minimax(board, minimax_depth, &computerMove, -9999, 9999, 1, 0);
	//perforam chosen  move
	performUserMove(computerMove);
	//check if promotion has happend and print move
	if (computerMove->movePromotePawn == 1 && guiMode == 0)
	{
		char* moveStr = getStringFormatMove(*computerMove);
		moveStr = str_replace(moveStr, "\n", "");
		printf("%s%s %s\n", "Computer: move ", moveStr, getPawnPromoteString(computerMove->pawnPromotionTool));
		free(moveStr);
	}
	else if (guiMode == 0)
	{
		char* moveStr = getStringFormatMove(*computerMove);
		printf("%s%s", "Computer: move ", moveStr);
		free(moveStr);
	}
	if (guiMode == 0)
		print_board(board);
	freeMove(computerMove);
	if (isPlayerStuck(opponentColor))
	{
		if (checkForTie(board, opponentColor))
		{
			printf("%s", TIE);
			exit(0);
		}
		if (isPlayerUnderMate(board, opponentColor) == 1)
		{
			if (opponentColor == BLACK)
				printf("%s", MATE_WHITE);
			else
				printf("%s", MATE_BLACK);
			exit(0);
		}
	}
	
	if (isPlayerUnderCheck(board, opponentColor) == 1)
	{
		printf("%s", "Check!\n");
	}

	nextPlayer = (computerColor == WHITE) ? BLACK : WHITE;
	return 0;
}

Move * get_best_move(int playerColor, int depth)
{
	Move* highestMove = NULL;
	MoveNode* moves = getMoves(board, playerColor);
	MoveNode* movesPointer = moves;
	int maxRes = -10000;
	while (movesPointer != NULL)
	{
		int res = getMoveScore(movesPointer->move, depth, playerColor);
		if (res > maxRes)
		{
			maxRes = res;
			highestMove = movesPointer->move;
		}
		movesPointer = movesPointer->next;
	}
	freeMoves(moves, highestMove);
	return highestMove;
}
/*return the user result after performing  the move 1-win, 0-not win yet*/
int UserMove(int playerColor)
{
	char input[50];
	int MoveDone = 0;
	while (MoveDone == 0)//didn't perform any move, continuing loop
	{
		if (playerColor == WHITE)
			printf("%s", "WHITE player - enter your move:\n");
		else
			printf("%s", "BLACK player - enter your move:\n");
		fgets(input, 50, stdin);
		reduceSpaces(input);
		strcpy(input, str_replace(input, "\n", ""));
		if (StartsWith(input, "move"))
		{
			Move *move = parseMoveCommand(input, playerColor);//Invalid position or color on the board- move==NULL
			if (move == NULL)
				continue;
			else
			{
				//check if the move is legal
				if (isMoveLegal(move, playerColor) == 0)
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
			split(input, ' ', &arr);
			Pos* pos = formatPos(arr[1]);
			if (pos == NULL)
				continue;
			if (getColorByPos(pos->x, pos->y) != playerColor)
			{
				printf("%s", WRONG_POS_COLOR);
				free(pos);
				continue;
			}
			//else
			MoveNode *moves = getMove(board, *pos, playerColor);
			if (moves != NULL)
			{
				printGameMoves(moves);
			}
			free(arr);
			freeMoves(moves, NULL);
		}
		else if (StartsWith(input, "get_best_moves"))
		{
			char **arr = NULL;
			split(input, ' ', &arr);
			int d = atoi(arr[1]);
			free(arr);
			//get all move and check their score by minimax
			MoveNode* highestMoves = NULL;
			MoveNode* moves = getMoves(board, playerColor);
			if (moves == NULL)
				continue;
			MoveNode* movesPointer = moves;
			int maxRes = -10000;
			while (movesPointer != NULL)
			{
				int res = getMoveScore(movesPointer->move, d, playerColor);
				if (res > maxRes)
				{
					maxRes = res;
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
			printGameMoves(highestMoves);
		}
		else if (StartsWith(input, "get_score"))
		{
			char **arr = NULL;
			split(input, ' ', &arr);
			int d = atoi(arr[1]);
			free(arr);
			//get_score d move <x,y> to <i,j> x
			str_cut(input, 0, 12);
			Move* move = parseMoveCommand(input, playerColor);
			int res = getMoveScore(move, d, playerColor);
			if (res != -1000)
			printf("%d\n", res);

		}
		else if (StartsWith(input, "save"))
		{
			char **arr = NULL;
			split(input, ' ', &arr);
			//create gamseState struct:
			GameStatus status;
			copyBoard(board,status.board);
			//strcpy(status.board, board);
			status.userColor = userColor;
			status.gameMode = gameMode;
			status.nextTurn = nextPlayer;
			status.difficulty=minimax_depth;
			saveFileWithFileName(status, arr[1]);
			free(arr);

		}
		else if (StartsWith(input, "quit"))
		{
			exit(0);
		}
	}
	//outside move loop, move has compelted:
	print_board(board);
	int opponentColor = (playerColor == BLACK) ? WHITE : BLACK;
	//check mate or a tie
	if (checkForTie(board, opponentColor))
	{
		printf("%s", TIE);
		exit(0);
	}
	if (isPlayerUnderMate(board, opponentColor) == 1)
	{
		if (opponentColor == BLACK)
			printf("%s", MATE_WHITE);
		else
			printf("%s", MATE_BLACK);
		exit(0);
	}
	if (isPlayerUnderCheck(board, opponentColor) == 1)
	{
		printf("%s", "Check!\n");
	}
	nextPlayer = opponentColor;
	return 0;
}

int getMoveScore(Move *move, int d, int playerColor)
{
	int opponentColor = (playerColor == BLACK) ? WHITE : BLACK;
	performMoveMinimax(board, move);
	//newBoard is changes - run minimax
	Move* bestMove = NULL;
	int res;
	if (d == 1)
	{
		//get newBoard score for the player:
		res = score(board, playerColor);
	}
	else
	{
		int oldComputerColor = computerColor;
		computerColor = playerColor;
		userColor = opponentColor;
		res = minimax(board, d - 1, &bestMove, -9999, 9999, 0, 0);
		freeMove(bestMove);
		computerColor = oldComputerColor;
	}
	UndoMove(board, move);
	freeMove(bestMove);
	return res;
}

void markMoves(char board[BOARD_SIZE][BOARD_SIZE], MoveNode * movesList)
{
	while (movesList != NULL)
	{
		board[movesList->move->dest->pos->x][movesList->move->dest->pos->y] = 'X';
		movesList = movesList->next;
	}
}

char* getPawnPromoteString(char tool)
{
	if (tool == WHITE_B || tool == BLACK_B)
		return "bishop";
	else if (tool == WHITE_N || tool == BLACK_N)
		return "knight";
	else if (tool == WHITE_Q || tool == BLACK_Q)
		return "queen";
	else if (tool == WHITE_R || tool == BLACK_R)
		return "rook";
	return "queen";
}
void printGameMoves(MoveNode *movesList)
{
	while (movesList != NULL)
	{
		char* moveStr = getStringFormatMove(*(movesList->move));
		if (movesList->move->movePromotePawn == 1)
		{
			moveStr = str_replace(moveStr, "\n","");
			printf("%s %s\n", moveStr, getPawnPromoteString(movesList->move->pawnPromotionTool));
		}
		else
			printf("%s", moveStr);
		free(moveStr);
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


void testMateTieCheck()
{
	minimax_depth = 1;
	userColor = WHITE;
	nextPlayer = WHITE;
	init_board(board);
	print_board(board);
	gameMode = 2;
	nextPlayer = WHITE;
	//check
	clear_board();
	nextPlayer = WHITE;
	set_disc("<d,6>", "black", "king");
	set_disc("<g,3>", "white", "bishop");
	set_disc("<d,1>", "white", "king");
	set_disc("<c,2>", "black", "knight");
	set_disc("<a,1>", "white", "rook");
	//print_board(board);
	int res = isPlayerUnderCheck(board, BLACK);
	assert(res == 1);
	//mate:
	set_disc("<d,5>", "black", "rook");
	set_disc("<d,7>", "black", "pawn");
	set_disc("<c,6>", "black", "bishop");
	set_disc("<e,6>", "black", "rook");
	//print_board(board);
	res = isPlayerUnderMate(board, BLACK);
	assert(res == 0);
	print_board(board);
	int res2 = res = isPlayerUnderMate(board, WHITE);
	assert(res2 == 0);
	set_disc("<e,6>", "black", "pawn");
	set_disc("<e,5>", "black", "pawn");
	set_disc("<c,4>", "white", "knight");
	print_board(board);
	res = isPlayerUnderMate(board, BLACK);
	assert(res = 1);

}

void testMinimax_moves()
{
	clear_board();
	init_board(board);
	remove_disc("<b,2>");
	remove_disc("<b,7>");
	remove_disc("<c,2>");
	set_disc("<b,3>", "white", "pawn");
	set_disc("<c,3>", "black", "pawn");
	set_disc("<h,4>", "white", "pawn");
}
void test_config_for_best_move1()
{
	//get_best_moves 4 = move <d,7> to <d,5>
	clear_board();
	set_disc("<d,1>", "white", "king");
	set_disc("<e,1>", "white", "rook");
	set_disc("<c,2>", "white", "pawn");
	set_disc("<f,2>", "black", "queen");
	set_disc("<g,2>", "white", "pawn");
	set_disc("<c,3>", "white", "pawn");
	set_disc("<e,3>", "black", "bishop");
	set_disc("<h,3>", "white", "pawn");

	set_disc("<d,5>", "black", "pawn");
	set_disc("<f,5>", "black", "pawn");
	set_disc("<c,5>", "black", "king");

	set_disc("<e,6>", "black", "rook");
	set_disc("<f,6>", "black", "pawn");
	set_disc("<g,6>", "black", "pawn");
	set_disc("<h,6>", "black", "pawn");

	set_disc("<a,7>", "black", "pawn");
	set_disc("<b,7>", "white", "queen");
	set_disc("<d,7>", "white", "rook");
	gameMode = 1;
	nextPlayer = WHITE;
	print_board(board);
}

void test_config_for_best_move2()
{
	//get_score 4 <e, 2> to <g, 1> = -12
	/*
	get_score 3 <e,2> to <g,1>
	-9
	get_score 2 <e,2> to <g,1>
	-9
	get_score 1 <e,2> to <g,1>
	-6
	*/
	clear_board();
	set_disc("<a,2>", "white", "pawn");
	set_disc("<b,2>", "white", "pawn");
	set_disc("<c,2>", "white", "pawn");
	set_disc("<d,2>", "white", "pawn");
	set_disc("<e,2>", "white", "knight");
	set_disc("<h,2>", "white", "pawn");

	set_disc("<a,1>", "white", "rook");
	set_disc("<b,1>", "white", "knight");
	set_disc("<c,1>", "white", "bishop");
	set_disc("<d,1>", "white", "queen");
	set_disc("<e,1>", "white", "king");
	set_disc("<h,1>", "black", "queen");

	set_disc("<c,4>", "white", "bishop");
	set_disc("<f,4>", "white", "pawn");

	set_disc("<g,6>", "black", "pawn");
	set_disc("<h,6>", "black", "pawn");
	set_disc("<a,7>", "black", "pawn");
	set_disc("<b,7>", "black", "pawn");
	set_disc("<c,7>", "black", "pawn");
	set_disc("<d,7>", "black", "pawn");
	set_disc("<f,7>", "black", "pawn");

	set_disc("<a,8>", "black", "rook");
	set_disc("<b,8>", "black", "knight");
	set_disc("<c,8>", "black", "bishop");
	set_disc("<e,8>", "black", "king");
	set_disc("<f,8>", "black", "bishop");
	set_disc("<g,8>", "black", "knight");
	set_disc("<h,8>", "black", "rook");

	gameMode = 1;
	minimax_depth = 4;
	nextPlayer = WHITE;
	print_board(board);
}
void runConsole()
{
	objectsInMemory = 0;
	isLastMovePromotePawn = 0;

	//first initializetion	
	gameMode = 1;
	minimax_depth = 1;
	userColor = WHITE;
	nextPlayer = WHITE;
	init_board(board);
	print_board(board);
	//test_config_for_best_move2();
	settingState();

}


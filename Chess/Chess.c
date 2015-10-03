#include "Chess.h"

#define malloc(x) myMalloc(x)
#define free(x) myFree(x)
#define calloc(x,y) myCalloc(x,y)
#define realloc(x,y) myRealloc(x,y)



void freeMove(Move *move)
{
	if (move == NULL)
		return;
	free(move->currPos);
	PosNode *node = move->dest;
	while (node != NULL)
	{
		PosNode *toFree = node;
		node = node->next;
		free(toFree->pos);
		free(toFree);

	}
	free(move);
}

void freeMoveWithoutDest(MoveNode *moveNode)
{
	if (moveNode->move)
		free(moveNode->move->currPos);
	free(moveNode->move);
	free(moveNode);
}

void freeMoves(MoveNode *moveNodeHead, Move* notDelete)
{
	while (moveNodeHead != NULL)
	{
		MoveNode* currNode = moveNodeHead;
		Move* move = moveNodeHead->move;
		if (move != notDelete)
			freeMove(move);

		moveNodeHead = moveNodeHead->next;
		free(currNode);
	}
}

void freeMoveNode(MoveNode *moveNode)
{
	if (moveNode != NULL)
		freeMove(moveNode->move);
	free(moveNode);
}

MoveNode * getLastNodeInList(MoveNode ** list)
{
	MoveNode * last = (*(list));
	if (last == NULL)
		return last;

	while (last->next)
		last = last->next;

	return last;

}

void addMoveNodeToList(MoveNode **movesList, MoveNode * moveNode)
{
	if (!*(movesList)) 
	{
		(*(movesList)) = moveNode;
	}
	else
	{
		MoveNode *last = getLastNodeInList(movesList);
		MoveNode **lastPtr = &last;
		(*(lastPtr))->next = moveNode;
	}
}

int isValidPos(Pos *pos)
{
	if (pos->x >= BOARD_SIZE || pos->x < 0 || pos->y >= BOARD_SIZE || pos->y < 0)
		return 0;
	return 1;
}

int isValidIndexes(int i, int j)
{
	if (i < 0 || i >= BOARD_SIZE || j < 0 || j >= BOARD_SIZE)
		return 0;
	return 1;
}

int getOpponentColor(int userColor)
{
	return userColor == 1 ? 0 : 1;
}

int getColor(char tool)
{
	if (tool == EMPTY)
		return -1;

	if (tool == WHITE_P || tool == WHITE_N || tool == WHITE_B || tool == WHITE_Q || tool == WHITE_R || tool == WHITE_K)
		return WHITE;

	return BLACK;

}

MoveNode *getPawnMoves(Pos pos, char board[BOARD_SIZE][BOARD_SIZE], int userColor)
{
	MoveNode * movesList = NULL;

	if (userColor == WHITE)
	{
		Pos upRight;
		upRight.x = pos.x + 1;
		upRight.y = pos.y + 1;

		if (isValidPos(&upRight) && getColor(board[upRight.x][upRight.y]) == getOpponentColor(userColor))
		{
			MoveNode * moveNode = createMoveNode(pos, upRight);
			addMoveNodeToList(&movesList, moveNode);
		}

		Pos upLeft;
		upLeft.x = pos.x - 1;
		upLeft.y = pos.y + 1;

		if (isValidPos(&upLeft) && getColor(board[upLeft.x][upLeft.y]) == getOpponentColor(userColor))
		{
			MoveNode * moveNode = createMoveNode(pos, upLeft);
			addMoveNodeToList(&movesList, moveNode);
		}

		Pos upCenter;
		upCenter.x = pos.x;
		upCenter.y = pos.y + 1;

		if (isValidPos(&upCenter) && board[upCenter.x][upCenter.y] == EMPTY)
		{
			MoveNode * moveNode = createMoveNode(pos, upCenter);
			addMoveNodeToList(&movesList, moveNode);
		}
	}
	else
	{
		//black - down
		Pos downRight;
		downRight.x = pos.x + 1;
		downRight.y = pos.y - 1;

		if (isValidPos(&downRight) && getColor(board[downRight.x][downRight.y]) == getOpponentColor(userColor))
		{
			MoveNode * moveNode = createMoveNode(pos, downRight);
			addMoveNodeToList(&movesList, moveNode);
		}

		Pos downLeft;
		downLeft.x = pos.x - 1;
		downLeft.y = pos.y - 1;

		if (isValidPos(&downLeft) && getColor(board[downLeft.x][downLeft.y]) == getOpponentColor(userColor))
		{
			MoveNode * moveNode = createMoveNode(pos, downLeft);
			addMoveNodeToList(&movesList, moveNode);
		}

		Pos downCenter;
		downCenter.x = pos.x;
		downCenter.y = pos.y - 1;

		if (isValidPos(&downCenter) && board[downCenter.x][downCenter.y] == EMPTY)
		{
			MoveNode * moveNode = createMoveNode(pos, downCenter);
			addMoveNodeToList(&movesList, moveNode);
		}

	}

	return movesList;
}


void getDiagAdjPositions(Pos pos, Pos** adj)
{
	for (int a = 0; a < 4; a++)
	{
		adj[a] = malloc(sizeof(Pos));
		if (adj[a] == NULL)
		{
			free(adj);
			perror_message("getManMoves");
			exit(0);
		}
	}
	//down:
	adj[0]->x = pos.x - 1;
	adj[0]->y = pos.y - 1;

	adj[1]->x = pos.x + 1;
	adj[1]->y = pos.y - 1;

	//up:
	adj[2]->x = pos.x - 1;
	adj[2]->y = pos.y + 1;

	adj[3]->x = pos.x + 1;
	adj[3]->y = pos.y + 1;

	for (int i = 0; i < 4; i++)
	{
		if (!isValidPos(adj[i]))
		{
			free(adj[i]);
			adj[i] = NULL;;
		}
	}
}


void getStraightAdjPositions(Pos pos, Pos** adj)
{
	for (int a = 0; a < 4; a++)
	{
		adj[a] = malloc(sizeof(Pos));
		if (adj[a] == NULL)
		{
			free(adj);
			perror_message("getManMoves");
			exit(0);
		}
	}
	//down:
	adj[0]->x = pos.x;
	adj[0]->y = pos.y - 1;

	//left:
	adj[1]->x = pos.x - 1;
	adj[1]->y = pos.y;

	//right:
	adj[2]->x = pos.x + 1;
	adj[2]->y = pos.y;

	//up:
	adj[3]->x = pos.x;
	adj[3]->y = pos.y + 1;

	for (int i = 0; i < 4; i++)
	{
		if (!isValidPos(adj[i]))
		{
			free(adj[i]);
			adj[i] = NULL;;
		}
	}
}


void getAdjPositions(Pos pos, Pos** adj)
{
	for (int a = 0; a < 8; a++)
	{
		adj[a] = malloc(sizeof(Pos));
		if (adj[a] == NULL)
		{
			free(adj);
			perror_message("getManMoves");
			exit(0);
		}
	}
	//down:
	adj[0]->x = pos.x - 1;
	adj[0]->y = pos.y - 1;

	adj[1]->x = pos.x + 1;
	adj[1]->y = pos.y - 1;

	adj[2]->x = pos.x;
	adj[2]->y = pos.y - 1;

	//up:
	adj[3]->x = pos.x - 1;
	adj[3]->y = pos.y + 1;

	adj[4]->x = pos.x + 1;
	adj[4]->y = pos.y + 1;

	adj[5]->x = pos.x;
	adj[5]->y = pos.y + 1;

	//left and right
	adj[6]->x = pos.x - 1;
	adj[6]->y = pos.y;

	adj[7]->x = pos.x + 1;
	adj[7]->y = pos.y;


	for (int i = 0; i < 8; i++)
	{
		if (!isValidPos(adj[i]))
		{
			free(adj[i]);
			adj[i] = NULL;;
		}
	}
}


void getKnightPositions(Pos pos, Pos** adj)
{
	for (int a = 0; a < 8; a++)
	{
		adj[a] = malloc(sizeof(Pos));
		if (adj[a] == NULL)
		{
			free(adj);
			perror_message("getManMoves");
			exit(0);
		}
	}

	//up left:
	adj[0]->x = pos.x - 2;
	adj[0]->y = pos.y + 1;

	adj[1]->x = pos.x - 1;
	adj[1]->y = pos.y + 2;
	
	//up right:
	adj[2]->x = pos.x + 2;
	adj[2]->y = pos.y + 1;

	adj[3]->x = pos.x + 1;
	adj[3]->y = pos.y + 2;

	//down left
	adj[4]->x = pos.x - 2;
	adj[4]->y = pos.y - 1;

	adj[5]->x = pos.x - 1 ;
	adj[5]->y = pos.y - 2;

	//down right
	adj[6]->x = pos.x + 2;
	adj[6]->y = pos.y - 1;

	adj[7]->x = pos.x + 1;
	adj[7]->y = pos.y - 2;


	for (int i = 0; i < 8; i++)
	{
		if (!isValidPos(adj[i]))
		{
			free(adj[i]);
			adj[i] = NULL;;
		}
	}
}

MoveNode *getBishopMoves(Pos pos, char board[BOARD_SIZE][BOARD_SIZE], int playerColor)
{
	MoveNode * movesList = NULL;
	Pos** adj = malloc(4 * sizeof(Pos*));
	getDiagAdjPositions(pos, adj);

	for (int i = 0; i < 4; i++)
	{
		if (adj[i] != NULL)
		{
			Pos  nextPosOnSameDirection;
			nextPosOnSameDirection.x = adj[i]->x;
			nextPosOnSameDirection.y = adj[i]->y;
			int xDiff = nextPosOnSameDirection.x - pos.x;
			int yDiff = nextPosOnSameDirection.y - pos.y;


			while (1)
			{
				char nextToolOnTheSamePath = board[nextPosOnSameDirection.x][nextPosOnSameDirection.y];

				if (getColor(nextToolOnTheSamePath) == playerColor)
					break;

				MoveNode *moveNode = createMoveNode(pos, nextPosOnSameDirection);
				addMoveNodeToList(&movesList, moveNode);

				if (getColor(nextToolOnTheSamePath) == getOpponentColor(playerColor)) 
					break;
				
				nextPosOnSameDirection.x = nextPosOnSameDirection.x + xDiff;
				nextPosOnSameDirection.y = nextPosOnSameDirection.y + yDiff;

				if (!isValidPos(&nextPosOnSameDirection)) 
					break;

				

			}
		}
	}
	//todo free adj array, in all functions who uses adj
	return movesList;
}

MoveNode *getKingMoves(Pos pos, char board[BOARD_SIZE][BOARD_SIZE], int playerColor)
{
	MoveNode * movesList = NULL;
	Pos** adj = malloc(8 * sizeof(Pos*));
	getAdjPositions(pos, adj);

	for (int i = 0; i < 8; i++)
	{
		if (adj[i] != NULL)
		{
			char nextToolOnTheSamePath = board[adj[i]->x][adj[i]->y];
			if (getColor(nextToolOnTheSamePath) == playerColor)
				continue;

			MoveNode *moveNode = createMoveNode(pos, *(adj[i]));
			addMoveNodeToList(&movesList, moveNode);			
		}
	}

	return movesList;
}

MoveNode *getQueenMoves(Pos pos, char board[BOARD_SIZE][BOARD_SIZE], int playerColor)
{
	//combine rook and bishop
	MoveNode * movesList = getBishopMoves(pos, board, playerColor);
	addMoveNodeToList(&movesList, getRookMoves(pos, board, playerColor));

	return movesList;
}

MoveNode *getRookMoves(Pos pos, char board[BOARD_SIZE][BOARD_SIZE], int playerColor)
{
	MoveNode * movesList = NULL;
	Pos** adj = malloc(4 * sizeof(Pos*));
	getStraightAdjPositions(pos, adj);

	for (int i = 0; i < 4; i++)
	{
		if (adj[i] != NULL)
		{
			Pos  nextPosOnSameDirection;
			nextPosOnSameDirection.x = adj[i]->x;
			nextPosOnSameDirection.y = adj[i]->y;
			int xDiff = nextPosOnSameDirection.x - pos.x;
			int yDiff = nextPosOnSameDirection.y - pos.y;

			while (1)
			{
				char nextToolOnTheSamePath = board[nextPosOnSameDirection.x][nextPosOnSameDirection.y];

				if (getColor(nextToolOnTheSamePath) == playerColor)
					break;

				MoveNode *moveNode = createMoveNode(pos, nextPosOnSameDirection);
				addMoveNodeToList(&movesList, moveNode);

				if (getColor(nextToolOnTheSamePath) == getOpponentColor(playerColor)) //we can eat it, but that's it for this direction
					break;

				nextPosOnSameDirection.x = nextPosOnSameDirection.x + xDiff;
				nextPosOnSameDirection.y = nextPosOnSameDirection.y + yDiff;

				if (!isValidPos(&nextPosOnSameDirection)) 
					break;
			}
		}
	}
	return movesList;
}

MoveNode *getKnightMoves(Pos pos, char board[BOARD_SIZE][BOARD_SIZE], int playerColor)
{
	MoveNode * movesList = NULL;
	Pos** adj = malloc(8 * sizeof(Pos*));
	getKnightPositions(pos, adj);

	for (int i = 0; i < 8; i++)
	{
		if (adj!= NULL && adj[i] != NULL)
		{
			char nextToolOnTheSamePath = board[adj[i]->x][adj[i]->y];
			if (getColor(nextToolOnTheSamePath) == playerColor)
				continue;

			MoveNode *moveNode = createMoveNode(pos, *(adj[i]));
			addMoveNodeToList(&movesList, moveNode);
		}
	}
	
	return movesList;
}

MoveNode * getMove(char board[BOARD_SIZE][BOARD_SIZE], Pos pos, int playerColor)
{
	MoveNode *movesList = NULL;
	char currentTool = board[pos.x][pos.y];

	char currentToolLowered = tolower(currentTool);

	switch (currentToolLowered)
	{
		case WHITE_P:
			movesList = getPawnMoves(pos, board, playerColor);
			break;
		case WHITE_B:
			movesList = getBishopMoves(pos, board, playerColor);
			break;
		case WHITE_K:
			movesList = getKingMoves(pos, board, playerColor);
			break;
		case WHITE_N:
			movesList = getKnightMoves(pos, board, playerColor);
			break;
		case WHITE_Q:
			movesList = getQueenMoves(pos, board, playerColor);
			break;
		case WHITE_R:
			movesList = getRookMoves(pos, board, playerColor);
			break;
		default:
			return NULL;
	}

	//check if player under check and then remove any moves that keeps the check status
	if (isPlayerUnderCheck(board, playerColor))
	{
		MoveNode * moveNode = movesList;
		MoveNode * prev = NULL;
		while (moveNode != NULL)
		{
			performMoveMinimax(board, moveNode->move);
			if (isPlayerUnderCheck(board, playerColor))
			{
				if (prev == NULL)
				{
					movesList = moveNode->next;
				}
				else
				{
					prev->next = moveNode->next;
				}
			}
			else
			{
				prev = moveNode;
			}
			UndoMove(board, moveNode->move);
			moveNode = moveNode->next;
		}
	}
	else//board is not under check but checking 
	{
		MoveNode * moveNode = movesList;
		MoveNode * prev = NULL;
		while (moveNode != NULL)
		{
			performMoveMinimax(board, moveNode->move);
			if (isPlayerUnderCheck(board, playerColor))
			{
				if (prev == NULL)
				{
					movesList = moveNode->next;
				}
				else
				{
					prev->next = moveNode->next;
				}
			}
			else
			{
				prev = moveNode;
			}
			UndoMove(board, moveNode->move);
			moveNode = moveNode->next;
		}
	}

	return movesList;
}

MoveNode * getMoves(char board[BOARD_SIZE][BOARD_SIZE], int playerColor)
{
	MoveNode *firstMoveNode = NULL;
	MoveNode *lastNode = NULL;

	int i, j;
	
	for (i = 0; i < BOARD_SIZE; i++)
	{
		for (j = 0; j < BOARD_SIZE; j++)
		{
			Pos pos;
			pos.x = i;
			pos.y = j;
			
			char currentTool = board[pos.x][pos.y];

			if (getColor(currentTool) != playerColor)
				continue;

			MoveNode * movesList = getMove(board, pos, playerColor);

			if (movesList != NULL)
				addMoveNodeToList(&firstMoveNode, movesList);
		}
	}

	
	return firstMoveNode;
}

MoveNode *createMoveNode(Pos pos, Pos destPos)
{
	MoveNode *moveNode = malloc(sizeof(MoveNode));
	if (moveNode == NULL)
	{
		perror_message("createMoveNode");
		exit(0);
	}
	Move *move = malloc(sizeof(Move));
	if (move == NULL)
	{
		perror_message("createMoveNode");
		exit(0);
	}
	move->currPos = malloc(sizeof(Pos));
	if (move->currPos == NULL)
	{
		perror_message("createMoveNode");
		exit(0);
	}
	move->currPos->x = pos.x;
	move->currPos->y = pos.y;
	move->dest = malloc(sizeof(PosNode));
	if (move->dest == NULL)
	{
		perror_message("createMoveNode");
		exit(0);
	}
	move->dest->pos = malloc(sizeof(Pos));
	if (move->dest->pos == NULL)
	{
		perror_message("createMoveNode");
		exit(0);
	}
	move->dest->pos->x = destPos.x;
	move->dest->pos->y = destPos.y;
	move->dest->next = NULL;
	//move->eat = eat;
	moveNode->move = move;
	moveNode->next = NULL;

	return moveNode;
}

Pos * formatPos(char* pos_input)
{
	char **arr = NULL;
	Pos* pos = malloc(sizeof(Pos));
	if (pos == NULL)
	{
		free(pos_input);
		perror_message("formatPos");
		exit(0);
	}
	pos_input = replace(pos_input, '<', "");
	char * toFree = pos_input;
	pos_input = replace(pos_input, '>', "");
	int arr_len = split(pos_input, ',', &arr);
	free(pos_input);
	free(toFree);
	pos->x = getIntValue(arr[0][0]);
	pos->y = atoi(arr[1]) - 1;
	freeArray(arr, arr_len);


	if (!isValidPos(pos))
	{
		if (printf("%s", WRONG_POSITION) < 0)
		{
			perror_message("formatPos");
			exit(0);
		}
		free(pos);
		return NULL;
	}

	return pos;
}
void print_line2()
{
	int i;
	printf("  |");
	for (i = 1; i < BOARD_SIZE * 4; i++){
		printf("-");
	}
	printf("|\n");
}

void print_board2(char board[BOARD_SIZE][BOARD_SIZE])
{
	int i, j;
	print_line2();
	for (j = BOARD_SIZE - 1; j >= 0; j--)
	{
		printf((j < 9 ? " %d" : "%d"), j + 1);
		for (i = 0; i < BOARD_SIZE; i++){
			printf("| %c ", board[i][j]);
		}
		printf("|\n");
		print_line2();
	}
	printf("   ");
	for (j = 0; j < BOARD_SIZE; j++){
		printf(" %c  ", (char)('a' + j));
	}
	printf("\n");
}

int isPlayerUnderMate(char board[BOARD_SIZE][BOARD_SIZE], int playerColor)
{
	int isMate = 1;
	char boardCopy[BOARD_SIZE][BOARD_SIZE];
	MoveNode * moves = getMoves(board, playerColor);
	MoveNode* movesPointer = moves;
	if (isPlayerUnderCheck(board, playerColor) == 0)
		return 0;

	if (moves == NULL)
		return 1;
	while (movesPointer != NULL)
	{
		performMoveMinimax(board, movesPointer->move);
		if (isPlayerUnderCheck(board, playerColor) == 0)
		{
			UndoMove(board, movesPointer->move);
			isMate = 0;
			break;
		}
		else
			UndoMove(board, movesPointer->move);
		movesPointer = movesPointer->next;
	}
	freeMoves(moves, NULL);
	return isMate;

	return 0;
}

char* getStringFormatMove(Move move)
{
	char* res = (char *)malloc(sizeof(char)* 1024);
	if (res == NULL)
	{
		perror_message("getStringFormatMove");
		exit(0);
	}
	Pos* curr = move.currPos;
	char* curr_str = getStringFormatPos(curr);
	strcpy(res, curr_str);
	free(curr_str);
	strcat(res, " to ");
	PosNode* posList = move.dest;
	while (posList != NULL)
	{
		curr = posList->pos;
		curr_str = getStringFormatPos(curr);
		strcat(res, curr_str);

		free(curr_str);
		posList = posList->next;
	}
	strcat(res, "\n");
	strcat(res, "\0");
	return res;
}

char* getStringFormatPos(Pos* pos)
{
	char* res = (char *)malloc(sizeof(char)* 512);
	if (res == NULL)
	{
		perror_message("getStringFormatPos");
		exit(0);
	}
	res[0] = '<';
	char x_char = pos->x + 'a';
	res[1] = x_char;
	res[2] = ',';
	int y_num = pos->y + 1;
	char y_char = y_num + '0';
	res[3] = y_char;
	res[4] = '>';
	res[5] = '\0';
	return res;

}

void copyBoard(char board[BOARD_SIZE][BOARD_SIZE], char newBoard[BOARD_SIZE][BOARD_SIZE])
{
	//copy board to move board:
	for (int j = 0; j <BOARD_SIZE; j++)
	{
		for (int i = 0; i < BOARD_SIZE; i++)
		{
			newBoard[i][j] = board[i][j];
		}
	}
}


int checkForTie(char board[BOARD_SIZE][BOARD_SIZE], int playerColor)
{
	if (isPlayerStuck(playerColor) == 1 && isPlayerUnderCheck(board,playerColor) == 0)
		return 1;
	return 0;
}
/*undo move on board after perform  move minimax*/
void UndoMove(char board[BOARD_SIZE][BOARD_SIZE], Move* move)
{
	Pos* curr = move->currPos;
	Pos* nextPos = move->dest->pos;
	char Player = board[nextPos->x][nextPos->y];
	
	//check if last move did promote
	if (move->movePromotePawn == 1)
	{
		int color = getColorByPos(nextPos->x, nextPos->y);
		if (color == WHITE)
			board[curr->x][curr->y] = WHITE_P;
		else
			board[curr->x][curr->y] = BLACK_P;
	}
	else
		board[curr->x][curr->y] = Player;
	//check if the move was eat
	if (move->eat == 1)
	{
		board[nextPos->x][nextPos->y] = move->eatTool;
	}
	else
		board[nextPos->x][nextPos->y] = EMPTY;
}
void performUserMove(Move *move)
{
	Pos* curr = move->currPos;
	Pos* nextPos = move->dest->pos;
	char Player = board[curr->x][curr->y];
	board[nextPos->x][nextPos->y] = Player;
	board[curr->x][curr->y] = EMPTY;
	//check promotion in case of pawn:
	if (Player == WHITE_P)
		move->movePromotePawn = checkAndPerformPromotion(board, nextPos, WHITE);
	else if (Player == BLACK_P)
		move->movePromotePawn = checkAndPerformPromotion(board, nextPos, BLACK);
	else
		move->movePromotePawn = 0;
}

void performMoveMinimax(char board[BOARD_SIZE][BOARD_SIZE], Move *move)
{
	Pos* curr = move->currPos;
	Pos* nextPos = move->dest->pos;
	char Player = board[curr->x][curr->y];
	pawnPromotionTool = EMPTY;
	//check if move is eating
	if (board[nextPos->x][nextPos->y] != EMPTY)
	{
		move->eat = 1;
		move->eatTool= board[nextPos->x][nextPos->y];
	}
	else
	{
		move->eat = 0;
		move->eatTool = EMPTY;
	}

	board[curr->x][curr->y] = EMPTY;
	board[nextPos->x][nextPos->y] = Player;

	//check promotion in case of pawn:
	if (Player == WHITE_P)
		move->movePromotePawn = checkAndPerformPromotion(board, nextPos, WHITE);
	else if (Player == BLACK_P)
		move->movePromotePawn = checkAndPerformPromotion(board, nextPos, BLACK);
	else
		move->movePromotePawn = 0;
}
int getColorByPos(int x, int y)
{
	if (board[x][y] == EMPTY)
		return -1;
	if (board[x][y] >= 'A' && board[x][y] <= 'Z')
		return BLACK;
	else
		return WHITE;
}
int countPlayers(char board[BOARD_SIZE][BOARD_SIZE])
{
	int playersCount = 0;
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			if (board[i][j] != EMPTY)
				playersCount++;
		}
	}
	return playersCount;
}

/*
This function assumes the command is a valid move command, parses it and return a move object
move <x,y> to <i,j> x
x- optional for promotion command if entered update pawnPromotionTool with specific tool
otherwise pawnPromotionTool = queen value
next player - the color of thr current player
*/
Move * parseMoveCommand(char *command)
{
	trimwhitespace(command);

	//remove double spaces
	char *toFree = NULL;
	int cnt = 0;
	while (strstr(command, "  "))
	{
		cnt++;
		toFree = command;
		command = str_replace(command, "  ", " ");
		if (command != NULL && cnt>1)
			free(toFree);
	}

	Move *move = (Move*)malloc(sizeof(Move));
	char** arr = NULL;
	int arrLen = split(command, ' ', &arr);
	//default value
	pawnPromotionTool = EMPTY;
	if (arrLen == 5)//we got promotion
	{
		if (userColor == WHITE)
		{
			if (strcmp(arr[4], "queen") == 0)
				pawnPromotionTool = WHITE_Q;
			else if (strcmp(arr[4], "bishop") == 0)
				pawnPromotionTool = WHITE_B;
			else if (strcmp(arr[4], "rook") == 0)
				pawnPromotionTool = WHITE_R;
			else if (strcmp(arr[4], "knight") == 0)
				pawnPromotionTool = WHITE_N;
		}
		else
		{
			if (strcmp(arr[4], "queen") == 0)
				pawnPromotionTool = BLACK_Q;
			else if (strcmp(arr[4], "bishop") == 0)
				pawnPromotionTool = BLACK_B;
			else if (strcmp(arr[4], "rook") == 0)
				pawnPromotionTool = BLACK_R;
			else if (strcmp(arr[4], "knight") == 0)
				pawnPromotionTool = BLACK_N;
		}
		
	}

	move->currPos = formatPos(arr[1]);
	if (!move->currPos)//position was invalid
		return NULL;
	//check if position <x,y> does not contain a piece of the user's color:
	if (nextPlayer != getColorByPos(move->currPos->x, move->currPos->y))
	{
		printf("%s", WRONG_POS_COLOR);
		return NULL;
	}

	char **destArr = NULL;
	int destArrLen = split(arr[3], '>', &destArr);

	PosNode *lastPos = NULL;
	for (int i = 0; i < destArrLen - 1; i++)
	{
		PosNode *posToAdd = malloc(sizeof(PosNode));
		posToAdd->pos = formatPos(destArr[i]);
		if (posToAdd->pos == NULL)
			return NULL;
		posToAdd->next = NULL;

		if (i == 0)
		{
			move->dest = posToAdd;
			lastPos = move->dest;
		}
		else
		{
			lastPos->next = posToAdd;
			lastPos = lastPos->next;
		}
	}
	freeArray(arr, arrLen);
	freeArray(destArr, destArrLen);
	return move;
}



/* check if pwan need promotion (he is at the end) and promot to nextPromotionTool , default is the queen*/
int checkAndPerformPromotion(char board[BOARD_SIZE][BOARD_SIZE], Pos* currPawnPos, int playerColor)
{
	int promot = 0;
	if (playerColor == WHITE && currPawnPos->y == BOARD_SIZE -1)//need promote
	{
		if (pawnPromotionTool == EMPTY)//default to queent
		{
			board[currPawnPos->x][currPawnPos->y] = WHITE_Q;
		}
		else
		{
			board[currPawnPos->x][currPawnPos->y] = pawnPromotionTool;
		}
		promot = 1;
	}
	else if (playerColor == BLACK && currPawnPos->y == 0)//need promote
	{
		if (pawnPromotionTool == EMPTY)//default to queent
		{
			board[currPawnPos->x][currPawnPos->y] = BLACK_Q;
		}
		else
		{
			board[currPawnPos->x][currPawnPos->y] = pawnPromotionTool;
		}
		promot = 1;
	}
	return promot;
}

int isMoveLegal(Move *move, int userColor)
{
	int legal = 0;
	char currType = board[move->currPos->x][move->currPos->y];
	if (userColor == BLACK)
	{
		if (currType == BLACK_B)
			legal = isBishopMoveLegal(move, userColor);
		else if (currType == BLACK_N)
			legal = isKnightMoveLegal(move, userColor);
		else if (currType == BLACK_K)
			legal = isKingMoveLegal(move, userColor);
		else if (currType == BLACK_P)
			legal = isPawnMoveLegal(move, userColor);
		else if (currType == BLACK_Q)
			legal = isQueenMoveLegal(move, userColor);
		else if (currType == BLACK_R)
			legal = isRookMoveLegal(move, userColor);
	}
	else
	{
		if (currType == WHITE_B)
			legal = isBishopMoveLegal(move, userColor);
		else if (currType == WHITE_N)
			legal = isKnightMoveLegal(move, userColor);
		else if (currType == WHITE_K)
			legal = isKingMoveLegal(move, userColor);
		else if (currType == WHITE_P)
			legal = isPawnMoveLegal(move, userColor);
		else if (currType == WHITE_Q)
			legal = isQueenMoveLegal(move, userColor);
		else if (currType == WHITE_R)
			legal = isRookMoveLegal(move, userColor);

	}
	if (legal == 0)//no need to test check
		return 0;
	else
	{
		//a player cannot insert himself to check state
		char boardCopy[BOARD_SIZE][BOARD_SIZE];
		performMoveMinimax(board, move);
		if (isPlayerUnderCheck(board, userColor) == 1)
		{
			legal = 0;
		}
		UndoMove(board, move);
	}
	return legal;
}

int isPawnMoveLegal(Move *move, int useColor)
{
	//move one step forward or eat diagonally
	Pos *curr = move->currPos;
	Pos* next = move->dest->pos;
	if (next->y != curr->y + 1)
		return 0;
	if (curr->x == next->x)//same colum do not eat
	{
		//check if not oocupied
		if (board[next->x][next->y] != EMPTY)
			return 0;
		return 1;
	}
	else //eating move
	{
		if (board[next->x][next->y] == EMPTY)
			return 0;
		else if (next->x != curr->x - 1 && next->x != curr->x + 1)
			return 0;
		else
		{
			if (userColor == getColorByPos(next->x, next->y))
				return 0;
			else return 1;
		}

	}
}
int isRookMoveLegal(Move *move, int useColor)
{
	Pos *curr = move->currPos;
	Pos* next = move->dest->pos;
	//cannot capture of it's own color
	if (getColorByPos(next->x, next->y) == useColor)
		return 0;
	//check if the next is on the same column or line
	if (curr->x != next->x && curr->y != next->y)
		return 0;
	if (curr->x == next->x)//move in column, up or down
	{
		int i = curr->x;
		int start = min(curr->y, next->y);
		int end = max(curr->y, next->y);
		for (int j = start; j < end; j++)
		{
			if (next->x == i && next->y == j)
				break;
			if (board[i][j] != EMPTY)
				return 0;
		}
	}
	else if (curr->y == next->y)//move in line, right or left
	{
		int j= curr->y;
		int start = min(curr->x, next->x);
		int end = max(curr->x, next->x);
		for (int i = start; i < end; i++)
		{
			if (next->x == i && next->y == j)
				break;
			if (board[i][j] != EMPTY)
				return 0;
		}
	}
	return 1;
}
int isKnightMoveLegal(Move *move, int useColor)
{
	Pos *curr = move->currPos;
	Pos* next = move->dest->pos;
	//cannot capture of it's own color
	if (getColorByPos(next->x, next->y) == useColor)
		return 0;
	//checking 8 places
	int x = curr->x;
	int y = curr->y;
	if ((next->x == x - 2 && next->y == y + 1) || (next->x == x - 1 && next->y == y + 2) || (next->x == x + 2 && next->y == y + 1)
		|| (next->x == x + 1 && next->y == y + 2) || (next->x == x - 2 && next->y == y - 1) ||
		(next->x == x - 1 && next->y == y - 2) || (next->x == x + 2 && next->y == y - 1)
		|| (next->x == x + 1 && next->y == y - 2))
	{
			return 1;
	}

}
int isBishopMoveLegal(Move *move, int useColor)
{
	Pos *curr = move->currPos;
	Pos* next = move->dest->pos;
	//cannot capture of it's own color
	if (getColorByPos(next->x, next->y) == useColor)
		return 0;
	//move diagonally, cannot leap over other pieces
	//check if on digaonal
	//0-not, 1- left up, 2- left down,3-right up, 4- right down
	int foundOnDiagonal = 0;
	for (int k = 1; k < BOARD_SIZE; k++)
	{
		if (next->x == curr->x + k && next->y == curr->y + k)
			foundOnDiagonal = 3;
		else if (next->x == curr->x - k && next->y == curr->y + k)
			foundOnDiagonal = 1;
		else if (next->x == curr->x - k && next->y == curr->y - k)
			foundOnDiagonal = 2;
		else if (next->x == curr->x + k && next->y == curr->y - k)
			foundOnDiagonal = 4;
	}
	if (foundOnDiagonal == 0)
		return 0;
	if (foundOnDiagonal == 1)
	{
		for (int i = curr->x-1, j = curr->y+1; i > next->x && j < next->y; i--, j++)
		{
			if (next->x == i && next->y == j)
				break;
			if (board[i][j] != EMPTY)
				return 0;
		}
	}
	else if (foundOnDiagonal == 2)
	{
		for (int i = curr->x - 1, j = curr->y - 1; i > next->x && j > next->y; i--, j--)
		{
			if (next->x == i && next->y == j)
				break;
			if (board[i][j] != EMPTY)
				return 0;
		}
	}
	else if (foundOnDiagonal == 3)
	{
		for (int i = curr->x + 1, j = curr->y + 1; i < next->x && j < next->y; i++, j++)
		{
			if (next->x == i && next->y == j)
				break;
			if (board[i][j] != EMPTY)
				return 0;
		}
	}
	else if (foundOnDiagonal == 4)
	{
		for (int i = curr->x + 1, j = curr->y - 1; i < next->x && j > next->y; i++, j--)
		{
			if (next->x == i && next->y == j)
				break;
			if (board[i][j] != EMPTY)
				return 0;
		}
	}
	return 1;
}

int isQueenMoveLegal(Move *move, int useColor)
{
	Pos *curr = move->currPos;
	Pos* next = move->dest->pos;
	//cannot capture of it's own color
	if (getColorByPos(next->x, next->y) == useColor)
		return 0;
	//check diagonal:
	int foundOnDiagonal = 0;
	for (int k = 1; k < BOARD_SIZE; k++)
	{
		if (next->x == curr->x + k && next->y == curr->y + k)
			foundOnDiagonal = 1;
		else if (next->x == curr->x - k && next->y == curr->y + k)
			foundOnDiagonal = 1;
		else if (next->x == curr->x - k && next->y == curr->y - k)
			foundOnDiagonal = 1;
		else if (next->x == curr->x + k && next->y == curr->y - k)
			foundOnDiagonal = 1;
	}
	if (foundOnDiagonal = 1)
		return 1;
	else
	{
		if (curr->x == next->x || curr->y == next->y)
			return 1;
	}
	return  0;
}
int isKingMoveLegal(Move *move, int useColor)
{
	Pos *curr = move->currPos;
	Pos* next = move->dest->pos;
	int x = curr->x;
	int y = curr->y;

	//cannot capture of it's own color
	if (getColorByPos(next->x, next->y) == useColor)
		return 0;
	
	if ((next->x == x - 1 && next->y == y) || (next->x == x + 1 && next->y == y) ||
		(next->x == x && next->y == y + 1) || (next->x == x && next->y == y-1))
		return 1; 
	if ((next->x == x - 1 && next->y == y+1) || (next->x == x + 1 && next->y == y+1) ||
		(next->x == x-1 && next->y == y - 1) || (next->x == x+1 && next->y == y - 1))
		return 1;
	return 0;

}

int isPlayerStuck(int playerColor)
{
	MoveNode *moveList = getMoves(board, playerColor);
	if (moveList == NULL)
		return 1;
	else
		return 0;
}


int checkRookThreat(char board[BOARD_SIZE][BOARD_SIZE],int oponnentColor, Pos *kingPos)
{
	int i = kingPos->x;
	int j = kingPos->y;
	int isThreat = 0;
	if (oponnentColor == BLACK)
	{
		while (j < 7)//scan up
		{
			if (board[i][j + 1] == EMPTY)
				j++;
			else if (board[i][j + 1] != BLACK_R)
				break;
			else if (board[i][j + 1] == BLACK_R)
			{
				isThreat = 1;
				return isThreat;
			}	
		}
		j = kingPos->y;
		while (j > 0)//scan down
		{
			if (board[i][j - 1] == EMPTY)
				j--;
			else if (board[i][j - 1] != BLACK_R)
				break;
			else if (board[i][j - 1] == BLACK_R)
			{
				isThreat = 1;
				return isThreat;
			}
		}
		j = kingPos->y;
		while (i  < 7)//scan right
		{
			if (board[i+1][j] == EMPTY)
				i++;
			else if (board[i+1][j] != BLACK_R)
				break;
			else if (board[i+1][j] == BLACK_R)
			{
				isThreat = 1;
				return isThreat;
			}
		}
		i = kingPos->x;
		while (i > 0)//scan left
		{
			if (board[i-1][j] == EMPTY)
				i--;
			else if (board[i - 1][j] != BLACK_R)
				break;
			else if (board[i - 1][j] == BLACK_R)
			{
				isThreat = 1;
				return isThreat;
			}
		}
	}
	else
	{
		while (j < 7)//scan up
		{
			if (board[i][j + 1] == EMPTY)
				j++;
			else if (board[i][j + 1] != WHITE_R)
				break;
			else if (board[i][j + 1] == WHITE_R)
			{
				isThreat = 1;
				return isThreat;
			}
		}
		j = kingPos->y;
		while (j > 0)//scan down
		{
			if (board[i][j - 1] == EMPTY)
				j--;
			else if (board[i][j - 1] != WHITE_R)
				break;
			else if (board[i][j - 1] == WHITE_R)
			{
				isThreat = 1;
				return isThreat;
			}
		}
		j = kingPos->y;
		while (i  < 7)//scan right
		{
			if (board[i + 1][j] == EMPTY)
				i++;
			else if (board[i + 1][j] != WHITE_R)
				break;
			else if (board[i + 1][j] == WHITE_R)
			{
				isThreat = 1;
				return isThreat;
			}
		}
		i = kingPos->x;
		while (i > 0)//scan left
		{
			if (board[i - 1][j] == EMPTY)
				i--;
			else if (board[i - 1][j] != WHITE_R)
				break;
			else if (board[i - 1][j] == WHITE_R)
			{
				isThreat = 1;
				return isThreat;
			}
		}
	}
	return 0;
	
}

int checkBishopThreat(char board[BOARD_SIZE][BOARD_SIZE],int oponnentColor, Pos *kingPos)
{
	
	int isThreat = 0;
	if (oponnentColor == BLACK)
	{
		int i = kingPos->x;
		int j = kingPos->y;
		while (i >0 && j < 7)//scan up left
		{
			if (board[i - 1][j + 1] == EMPTY)
			{
				i--;
				j++;
			}
			else if (board[i - 1][j + 1] != BLACK_B)
				break;
			else if (board[i - 1][j + 1] == BLACK_B)
			{
				isThreat = 1;
				return isThreat;
			}
		}
		i = kingPos->x;
		j = kingPos->y;
		while (i < 7 && j < 7)//scan up right
		{
			if (board[i + 1][j + 1] == EMPTY)
			{
				i++;
				j++;
			}
			else if (board[i + 1][j + 1] != BLACK_B)
				break;
			else if (board[i + 1][j + 1] == BLACK_B)
			{
				isThreat = 1;
				return isThreat;
			}
		}
		i = kingPos->x;
		j = kingPos->y;
		while (i >0 && j > 0)//scan down left
		{
			if (board[i - 1][j - 1] == EMPTY)
			{
				i--;
				j--;
			}
			else if (board[i - 1][j - 1] != BLACK_B)
				break;
			else if (board[i - 1][j - 1] == BLACK_B)
			{
				isThreat = 1;
				return isThreat;
			}
		}
		i = kingPos->x;
		j = kingPos->y;
		while (i < 7 && j > 0)//scan down right
		{
			if (board[i + 1][j - 1] == EMPTY)
			{
				i++;
				j--;
			}
			else if (board[i + 1][j - 1] != BLACK_B)
				break;
			else if (board[i + 1][j - 1] == BLACK_B)
			{
				isThreat = 1;
				return isThreat;
			}
		}
	}
	else
	{
		int i = kingPos->x;
		int j = kingPos->y;
		while (i >0 && j < 7)//scan up left
		{
			if (board[i - 1][j + 1] == EMPTY)
			{
				i--;
				j++;
			}
			else if (board[i - 1][j + 1] != WHITE_B)
				break;
			else if (board[i - 1][j + 1] == WHITE_B)
			{
				isThreat = 1;
				return isThreat;
			}
		}
		i = kingPos->x;
		j = kingPos->y;
		while (i < 7 && j < 7)//scan up right
		{
			if (board[i + 1][j + 1] == EMPTY)
			{
				i++;
				j++;
			}
			else if (board[i + 1][j + 1] != WHITE_B)
				break;
			else if (board[i + 1][j + 1] == WHITE_B)
			{
				isThreat = 1;
				return isThreat;
			}
		}
		i = kingPos->x;
		j = kingPos->y;
		while (i >0 && j > 0)//scan down left
		{
			if (board[i - 1][j - 1] == EMPTY)
			{
				i--;
				j--;
			}
			else if (board[i - 1][j - 1] != WHITE_B)
				break;
			else if (board[i - 1][j - 1] == WHITE_B)
			{
				isThreat = 1;
				return isThreat;
			}
		}
		i = kingPos->x;
		j = kingPos->y;
		while (i < 7 && j > 0)//scan down right
		{
			if (board[i + 1][j - 1] == EMPTY)
			{
				i++;
				j--;
			}
			else if (board[i + 1][j - 1] != WHITE_B)
				break;
			else if (board[i + 1][j - 1] == WHITE_B)
			{
				isThreat = 1;
				return isThreat;
			}
		}
	}
	return 0;
}


int checkQueenThreat(char board[BOARD_SIZE][BOARD_SIZE],int oponnentColor, Pos *kingPos)
{
	int i = kingPos->x;
	int j = kingPos->y;
	int isThreat = 0;
	if (oponnentColor == BLACK)
	{
		while (j < 7)//scan up
		{
			if (board[i][j + 1] == EMPTY)
				j++;
			else if (board[i][j + 1] != BLACK_Q)
				break;
			else if (board[i][j + 1] == BLACK_Q)
			{
				isThreat = 1;
				return isThreat;
			}
		}
		j = kingPos->y;
		while (j > 0)//scan down
		{
			if (board[i][j - 1] == EMPTY)
				j--;
			else if (board[i][j - 1] != BLACK_Q)
				break;
			else if (board[i][j - 1] == BLACK_Q)
			{
				isThreat = 1;
				return isThreat;
			}
		}
		j = kingPos->y;
		while (i  < 7)//scan right
		{
			if (board[i + 1][j] == EMPTY)
				i++;
			else if (board[i + 1][j] != BLACK_Q)
				break;
			else if (board[i + 1][j] == BLACK_Q)
			{
				isThreat = 1;
				return isThreat;
			}
		}
		i = kingPos->x;
		while (i > 0)//scan left
		{
			if (board[i - 1][j] == EMPTY)
				i--;
			else if (board[i - 1][j] != BLACK_Q)
				break;
			else if (board[i - 1][j] == BLACK_Q)
			{
				isThreat = 1;
				return isThreat;
			}
		}

		i = kingPos->x;
		j = kingPos->y;
		while (i >0 && j < 7)//scan up left
		{
			if (board[i - 1][j + 1] == EMPTY)
			{
				i--;
				j++;
			}
			else if (board[i - 1][j + 1] != BLACK_Q)
				break;
			else if (board[i - 1][j + 1] == BLACK_Q)
			{
				isThreat = 1;
				return isThreat;
			}
		}
		i = kingPos->x;
		j = kingPos->y;
		while (i < 7 && j < 7)//scan up right
		{
			if (board[i + 1][j + 1] == EMPTY)
			{
				i++;
				j++;
			}
			else if (board[i + 1][j + 1] != BLACK_Q)
				break;
			else if (board[i + 1][j + 1] == BLACK_Q)
			{
				isThreat = 1;
				return isThreat;
			}
		}
		i = kingPos->x;
		j = kingPos->y;
		while (i >0 && j > 0)//scan down left
		{
			if (board[i - 1][j - 1] == EMPTY)
			{
				i--;
				j--;
			}
			else if (board[i - 1][j - 1] != BLACK_Q)
				break;
			else if (board[i - 1][j - 1] == BLACK_Q)
			{
				isThreat = 1;
				return isThreat;
			}
		}
		i = kingPos->x;
		j = kingPos->y;
		while (i < 7 && j > 0)//scan down right
		{
			if (board[i + 1][j - 1] == EMPTY)
			{
				i++;
				j--;
			}
			else if (board[i + 1][j - 1] != BLACK_Q)
				break;
			else if (board[i + 1][j - 1] == BLACK_Q)
			{
				isThreat = 1;
				return isThreat;
			}
		}
	}
	else
	{
		while (j < 7)//scan up
		{
			if (board[i][j + 1] == EMPTY)
				j++;
			else if (board[i][j + 1] != WHITE_Q)
				break;
			else if (board[i][j + 1] == WHITE_Q)
			{
				isThreat = 1;
				return isThreat;
			}
		}
		j = kingPos->y;
		while (j > 0)//scan down
		{
			if (board[i][j - 1] == EMPTY)
				j--;
			else if (board[i][j - 1] != WHITE_Q)
				break;
			else if (board[i][j - 1] == WHITE_Q)
			{
				isThreat = 1;
				return isThreat;
			}
		}
		j = kingPos->y;
		while (i  < 7)//scan right
		{
			if (board[i + 1][j] == EMPTY)
				i++;
			else if (board[i + 1][j] != WHITE_Q)
				break;
			else if (board[i + 1][j] == WHITE_Q)
			{
				isThreat = 1;
				return isThreat;
			}
		}
		i = kingPos->x;
		while (i > 0)//scan left
		{
			if (board[i - 1][j] == EMPTY)
				i--;
			else if (board[i - 1][j] != WHITE_Q)
				break;
			else if (board[i - 1][j] == WHITE_Q)
			{
				isThreat = 1;
				return isThreat;
			}
		}
		//check diagonals:
		i = kingPos->x;
		j = kingPos->y;
		while (i >0 && j < 7)//scan up left
		{
			if (board[i - 1][j + 1] == EMPTY)
			{
				i--;
				j++;
			}
			else if (board[i - 1][j + 1] != WHITE_Q)
				break;
			else if (board[i - 1][j + 1] == WHITE_Q)
			{
				isThreat = 1;
				return isThreat;
			}
		}
		i = kingPos->x;
		j = kingPos->y;
		while (i < 7 && j < 7)//scan up right
		{
			if (board[i + 1][j + 1] == EMPTY)
			{
				i++;
				j++;
			}
			else if (board[i + 1][j + 1] != WHITE_Q)
				break;
			else if (board[i + 1][j + 1] == WHITE_Q)
			{
				isThreat = 1;
				return isThreat;
			}
		}
		i = kingPos->x;
		j = kingPos->y;
		while (i >0 && j > 0)//scan down left
		{
			if (board[i - 1][j - 1] == EMPTY)
			{
				i--;
				j--;
			}
			else if (board[i - 1][j - 1] != WHITE_Q)
				break;
			else if (board[i - 1][j - 1] == WHITE_Q)
			{
				isThreat = 1;
				return isThreat;
			}
		}
		i = kingPos->x;
		j = kingPos->y;
		while (i < 7 && j > 0)
		{
			if (board[i + 1][j - 1] == EMPTY)
			{
				i++;
				j--;
			}
			else if (board[i + 1][j - 1] != WHITE_Q)
				break;
			else if (board[i + 1][j - 1] == WHITE_Q)
			{
				isThreat = 1;
				return isThreat;
			}
		}
	}
	return 0;
}


int checkKnightThreat(char board[BOARD_SIZE][BOARD_SIZE], int oponnentColor, Pos *kingPos)
{
	int i = kingPos->x;
	int j = kingPos->y;
	if (oponnentColor == BLACK)
	{
		if (isValidIndexes(i - 2, j + 1) == 1 && board[i - 2][j + 1] == BLACK_N)
			return 1;
		else if (isValidIndexes(i - 1, j + 2) == 1 && board[i - 1][j + 2] == BLACK_N)
			return 1;
		else if (isValidIndexes(i + 2, j + 1) == 1 && board[i + 2][j + 1] == BLACK_N)
			return 1;
		else if (isValidIndexes(i + 1, j + 2) == 1 && board[i + 1][j + 2] == BLACK_N)
			return 1;
		else if (isValidIndexes(i - 2, j - 1) == 1 && board[i - 2][j - 1] == BLACK_N)
			return 1;
		else if (isValidIndexes(i - 1, j - 2) == 1 && board[i - 1][j - 2] == BLACK_N)
			return 1;
		else if (isValidIndexes(i + 2, j - 1) == 1 && board[i + 2][j - 1] == BLACK_N)
			return 1;
		else if (isValidIndexes(i + 1, j -2) == 1 && board[i + 1][j - 2] == BLACK_N)
			return 1;
	}
	else
	{
		if (isValidIndexes(i - 2, j + 1) == 1 && board[i - 2][j + 1] == WHITE_N)
			return 1;
		else if (isValidIndexes(i - 1, j + 2) == 1 && board[i - 1][j + 2] == WHITE_N)
			return 1;
		else if (isValidIndexes(i + 2, j + 1) == 1 && board[i + 2][j + 1] == WHITE_N)
			return 1;
		else if (isValidIndexes(i + 1, j + 2) == 1 && board[i + 1][j + 2] == WHITE_N)
			return 1;
		else if (isValidIndexes(i - 2, j - 1) == 1 && board[i - 2][j - 1] == WHITE_N)
			return 1;
		else if (isValidIndexes(i - 1, j - 2) == 1 && board[i - 1][j - 2] == WHITE_N)
			return 1;
		else if (isValidIndexes(i + 2, j - 1) == 1 && board[i + 2][j - 1] == WHITE_N)
			return 1;
		else if (isValidIndexes(i + 1, j - 2) == 1 && board[i + 1][j - 2] == WHITE_N)
			return 1;
	}
	return 0;
}


Pos* getKingPos(int playerColor)
{
	Pos* pos = malloc(sizeof(Pos));
	if (pos == NULL)
	{
		perror_message("getKingPos");
		exit(0);
	}
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			if (playerColor == WHITE && board[i][j] == WHITE_K)
			{
				pos->x = i;
				pos->y = j;
				return pos;
			}
			else if (playerColor == BLACK && board[i][j] == BLACK_K)
			{
				pos->x = i;
				pos->y = j;
				return pos;
			}

		}
	}
	free(pos);
	return NULL;
}


int checkPawnThreat(char board[BOARD_SIZE][BOARD_SIZE],int oponnentColor, Pos *kingPos)
{
	int i = kingPos->x;
	int j = kingPos->y;
	if (oponnentColor == BLACK)
	{
		if (isValidIndexes(i + 1, j + 1) && board[i + 1][j + 1] == BLACK_P)
			return 1;
		else if (isValidIndexes(i - 1, j + 1) && board[i - 1][j + 1] == BLACK_P)
			return 1;
		else if (isValidIndexes(i - 1, j - 1) && board[i - 1][j - 1] == BLACK_P)
			return 1;
		else if (isValidIndexes(i + 1, j - 1) && board[i + 1][j - 1] == BLACK_P)
			return 1;
	}
	else
	{
		if(isValidIndexes(i + 1, j + 1) && board[i + 1][j + 1] == WHITE_P)
			return 1;
		else if (isValidIndexes(i - 1, j + 1) && board[i - 1][j + 1] == WHITE_P)
			return 1;
		else if (isValidIndexes(i - 1, j - 1) && board[i - 1][j - 1] == WHITE_P)
			return 1;
		else if (isValidIndexes(i + 1, j - 1) && board[i + 1][j - 1] == WHITE_P)
			return 1;
	}
	return 0;

}


int checkKingThreat(char board[BOARD_SIZE][BOARD_SIZE], int oponnentColor, Pos *kingPos)
{
	int i = kingPos->x;
	int j = kingPos->y;
	if (oponnentColor == BLACK)
	{
		if (isValidIndexes(i + 1, j + 1) && board[i + 1][j + 1] == BLACK_K)
			return 1;
		else if (isValidIndexes(i - 1, j + 1) && board[i - 1][j + 1] == BLACK_K)
			return 1;
		else if (isValidIndexes(i - 1, j - 1) && board[i - 1][j - 1] == BLACK_K)
			return 1;
		else if (isValidIndexes(i + 1, j - 1) && board[i + 1][j - 1] == BLACK_K)
			return 1;
		else if (isValidIndexes(i + 1, j) && board[i + 1][j] == BLACK_K)
			return 1;
		else if (isValidIndexes(i - 1, j) && board[i - 1][j] == BLACK_K)
			return 1;
		else if (isValidIndexes(i, j - 1) && board[i][j - 1] == BLACK_K)
			return 1;
		else if (isValidIndexes(i , j + 1) && board[i][j + 1] == BLACK_K)
			return 1;

	}
	else
	{
		if (isValidIndexes(i + 1, j + 1) && board[i + 1][j + 1] == WHITE_K)
			return 1;
		else if (isValidIndexes(i - 1, j + 1) && board[i - 1][j + 1] == WHITE_K)
			return 1;
		else if (isValidIndexes(i - 1, j - 1) && board[i - 1][j - 1] == WHITE_K)
			return 1;
		else if (isValidIndexes(i + 1, j - 1) && board[i + 1][j - 1] == WHITE_K)
			return 1;
		else if (isValidIndexes(i + 1, j) && board[i + 1][j] == WHITE_K)
			return 1;
		else if (isValidIndexes(i - 1, j) && board[i - 1][j] == WHITE_K)
			return 1;
		else if (isValidIndexes(i, j - 1) && board[i][j - 1] == WHITE_K)
			return 1;
		else if (isValidIndexes(i, j + 1) && board[i][j + 1] == WHITE_K)
			return 1;
	}
	return 0;
}


int isPlayerUnderCheck(char board[BOARD_SIZE][BOARD_SIZE], int playerColor)
{
	int stop;
	Pos *kingPos = getKingPos(playerColor);
	if (kingPos == NULL)
	{
		stop = 1;
		return 0;
	}
		
	int opponentColor = getOpponentColor(playerColor);

	if (checkRookThreat(board, opponentColor, kingPos) == 1 || checkBishopThreat(board,opponentColor, kingPos) == 1 ||
		checkKingThreat(board,opponentColor, kingPos) == 1
		|| checkKnightThreat(board, opponentColor, kingPos) == 1 || checkPawnThreat(board,opponentColor, kingPos) == 1 ||
		checkQueenThreat(board,opponentColor, kingPos) == 1)
	{
		free(kingPos);
		return 1;
	}
	free(kingPos);
	return 0;
		
}




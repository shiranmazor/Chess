#include "Chess.h"

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



//returns false if pos is outside the board
int isValidPos(Pos *pos)
{
	if (pos->x >= BOARD_SIZE || pos->x < 0 || pos->y >= BOARD_SIZE || pos->y < 0)
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
		//white - up

		Pos upRight;
		upRight.x = pos.x + 1;
		upRight.y = pos.y + 1;

		if (isValidPos(&upRight) && getColor(board[upRight.x, upRight.y]) ==  getOpponnetColor(userColor))
		{
			MoveNode * moveNode = createMoveNode(pos, upRight,1);
			addMoveNodeToList(&movesList, moveNode);
		}

		Pos upLeft;
		upLeft.x = pos.x - 1;
		upLeft.y = pos.y + 1;

		if (isValidPos(&upLeft) && getColor(board[upLeft.x, upLeft.y]) == getOpponnetColor(userColor))
		{
			MoveNode * moveNode = createMoveNode(pos, upLeft, 1);
			addMoveNodeToList(&movesList, moveNode);
		}

		Pos upCenter;
		upCenter.x = pos.x;
		upCenter.y = pos.y + 1;

		if (isValidPos(&upCenter) && board[upCenter.x, upCenter.y] == EMPTY)
		{
			MoveNode * moveNode = createMoveNode(pos, upCenter, 0);
			addMoveNodeToList(&movesList, moveNode);
		}
	}
	else
	{
		//black - down
		Pos downRight;
		downRight.x = pos.x + 1;
		downRight.y = pos.y - 1;

		if (isValidPos(&downRight) && getColor(board[downRight.x, downRight.y]) == getOpponnetColor(userColor))
		{
			MoveNode * moveNode = createMoveNode(pos, downRight, 1);
			addMoveNodeToList(&movesList, moveNode);
		}

		Pos downLeft;
		downLeft.x = pos.x - 1;
		downLeft.y = pos.y - 1;

		if (isValidPos(&downLeft) && getColor(board[downLeft.x, downLeft.y]) == getOpponnetColor(userColor))
		{
			MoveNode * moveNode = createMoveNode(pos, downLeft, 1);
			addMoveNodeToList(&movesList, moveNode);
		}

		Pos downCenter;
		downCenter.x = pos.x;
		downCenter.y = pos.y - 1;

		if (isValidPos(&downCenter) && board[downCenter.x, downCenter.y] == EMPTY)
		{
			MoveNode * moveNode = createMoveNode(pos, downCenter, 0);
			addMoveNodeToList(&movesList, moveNode);
		}

	}

	return movesList;
}

//returns all valid diagonial adjacent positions
void getDiagAdjPositions(Pos pos, Pos** adj)
{
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

//returns all valid straight adjacent positions
void getStraightAdjPositions(Pos pos, Pos** adj)
{
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


//returns all valid adjacent positions
void getAdjPositions(Pos pos, Pos** adj)
{
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
	//return adj;
}

//returns all valid kinght next positions
void getKnightPositions(Pos pos, Pos** adj)
{
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

			while (1)
			{
				char nextToolOnTheSamePath = board[nextPosOnSameDirection.x][nextPosOnSameDirection.y];

				if (getColor(nextToolOnTheSamePath) == playerColor)
					break;

				MoveNode *moveNode = createMoveNode(pos, *(adj[i]), 0);
				addMoveNodeToList(&movesList, moveNode);

				if (getColor(nextToolOnTheSamePath) == getOpponentColor(playerColor)) //we can eat it, but that's it for this direction
					break;

				int xDiff = nextPosOnSameDirection.x - pos.x;
				int yDiff = nextPosOnSameDirection.y - pos.y;
				
				nextPosOnSameDirection.x = nextPosOnSameDirection.x + xDiff;
				nextPosOnSameDirection.y = nextPosOnSameDirection.y + yDiff;

				if (!isValidPos(&nextPosOnSameDirection)) //we reached end of board
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
	getgAdjPositions(pos, adj);

	for (int i = 0; i < 8; i++)
	{
		if (adj[i] != NULL)
		{
			char nextToolOnTheSamePath = board[adj[i]->x][adj[i]->y];
			if (getColor(nextToolOnTheSamePath) == playerColor)
				break;

			MoveNode *moveNode = createMoveNode(pos, *(adj[i]), 0);
			addMoveNodeToList(&movesList, moveNode);			
		}
	}

	return movesList;
}

MoveNode * getLastNodeInList(MoveNode * list)
{
	MoveNode * last = list;
	if (last == NULL)
		return last;

	while (last->next)
		last = last->next;

	return last;
	
}

MoveNode *getQueenMoves(Pos pos, char board[BOARD_SIZE][BOARD_SIZE], int playerColor)
{
	//combine rook and bishop
	MoveNode * movesList = getBishopMoves(pos, board[BOARD_SIZE][BOARD_SIZE], playerColor);
	addMoveNodeToList(&movesList, getRookMoves(pos, board[BOARD_SIZE][BOARD_SIZE], playerColor));

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

			while (1)
			{
				char nextToolOnTheSamePath = board[nextPosOnSameDirection.x][nextPosOnSameDirection.y];

				if (getColor(nextToolOnTheSamePath) == playerColor)
					break;

				MoveNode *moveNode = createMoveNode(pos, *(adj[i]));
				addMoveNodeToList(&movesList, moveNode);

				if (getColor(nextToolOnTheSamePath) == getOpponentColor(playerColor)) //we can eat it, but that's it for this direction
					break;

				int xDiff = nextPosOnSameDirection.x - pos.x;
				int yDiff = nextPosOnSameDirection.y - pos.y;

				nextPosOnSameDirection.x = nextPosOnSameDirection.x + xDiff;
				nextPosOnSameDirection.y = nextPosOnSameDirection.y + yDiff;

				if (!isValidPos(&nextPosOnSameDirection)) //we reached end of board
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
	getStraightAdjPositions(pos, adj);

	for (int i = 0; i < 8; i++)
	{
		if (adj[i] != NULL)
		{
			char nextToolOnTheSamePath = board[adj[i]->x][adj[i]->y];
			if (getColor(nextToolOnTheSamePath) == playerColor)
				break;

			MoveNode *moveNode = createMoveNode(pos, *(adj[i]), 0);
			addMoveNodeToList(&movesList, moveNode);
		}
	}
}

//return all user moves
MoveNode * getMoves(char board[BOARD_SIZE][BOARD_SIZE], int playerColor)
{
	MoveNode *firstMoveNode = NULL;
	MoveNode *lastNode = NULL;

	int i, j;
	int maxEats = -1;
	for (i = 0; i < BOARD_SIZE; i++)
	{
		for (j = 0; j < BOARD_SIZE; j++)
		{
			Pos pos;
			pos.x = i;
			pos.y = j;

			MoveNode *movesList = NULL;
			char currentTool = board[i][j];
			
			if (getColor(currentTool) != playerColor)
				continue;

			char currentToolLowered = tolower(board[i][j]);

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
			}

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


void addMoveNodeToList(MoveNode **movesList, MoveNode * moveNode)
{
	if (!*(movesList)) //empty list
	{
		*(movesList) = moveNode;
		//*(last) = *(movesList);
	}
	else
	{
		MoveNode *last = getLastNodeInList(&movesList);
		MoveNode **lastPtr = &last;
		(*(lastPtr))->next = moveNode;
		//*(lastPtr) = (*(lastPtr))->next;
	}
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


	if (pos->x < 0 || pos->x > 9 || pos->y<0 || pos->y>9 || ((pos->x + pos->y) % 2 != 0))
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

int boardInitializeOk()
{
	//Todo:implement
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

int checkifPlayerWins(int player_color)
{
	//todo:implement
	return 0;
}

int performMove(Move move)
{
	//todo:implement
	return 0;
}
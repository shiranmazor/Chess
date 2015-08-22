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

//return all user moves
MoveNode * getMoves(char board[BOARD_SIZE][BOARD_SIZE])
{
	//Todo: implement
	return NULL;
}

MoveNode *createMoveNode(Pos pos, Pos destPos, int eat)
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
	move->eat = eat;
	moveNode->move = move;
	moveNode->next = NULL;

	return moveNode;
}


void addMoveNodeToList(MoveNode **movesList, MoveNode **last, MoveNode * moveNode)
{
	if (!*(movesList)) //empty list
	{
		*(movesList) = moveNode;
		*(last) = *(movesList);
	}
	else
	{
		(*(last))->next = moveNode;
		*(last) = (*(last))->next;
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
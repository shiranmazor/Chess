#include "fileHandle.h"
#include "string.h"
#include "stdio.h"

#define malloc(x) myMalloc(x)
#define free(x) myFree(x)
#define calloc(x,y) myCalloc(x,y)
#define realloc(x,y) myRealloc(x,y)

int fileExists(const char *fname)
{
	FILE *file;
	file = fopen(fname, "r");
	if (file != NULL)
	{
		fclose(file);
		return 1;
	}
	return 0;
}
char * getFilenameBySlotNumber(int slotNumber)
{
	char slotStr[4];
	sprintf(slotStr, "%d", slotNumber);

	char * fullFileName = malloc(sizeof(char)*100);
	sprintf(fullFileName, "%s%s%s", FILENAME, slotStr, FILEEXT);

	return fullFileName;
}

int saveFileWithFileName(GameStatus gameState, char * fullFileName)
{
	FILE *f = fopen(fullFileName, "w");
	
	if (f == NULL)
	{
		printf("%s",WRONG_FILE_NAME);
		return 1;
	}

	fprintf(f, "%s", "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
	fprintf(f, "%s", "<game>\n");
	fprintf(f, "\t<next_turn>%s</next_turn>\n", gameState.nextTurn == WHITE ? "White" : "Black");
	fprintf(f, "\t<game_mode>%d</game_mode>\n", gameState.gameMode);
	char difficulty[5];
	if (gameState.difficulty == -1)
		sprintf(difficulty, "%s", "BEST");
	else
		sprintf(difficulty, "%d", gameState.difficulty);

	fprintf(f, "\t<difficulty>%s</difficulty>\n", difficulty);

	char * userColorStr;
	if (gameState.userColor == BLACK)
		userColorStr = "Black";

	if (gameState.userColor == WHITE)
		userColorStr = "White";

	fprintf(f, "\t<user_color>%s</user_color>\n", userColorStr);
	fprintf(f, "%s", "\t<board>\n");


	for (int i = BOARD_SIZE - 1; i >= 0; i--)
	{
		fprintf(f, "\t\t<row_%d>", i + 1);
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			if (gameState.board[j][i] == EMPTY)
			{
				fputc('_', f);
			}
			else
			{
				fputc(gameState.board[j][i], f);
			}
		}
		fprintf(f, "</row_%d>\n", i + 1);
	}
	fprintf(f, "%s", "\t</board>\n");
	fprintf(f, "%s", "</game>");

	fclose(f);
	return 0;
}

int saveFileWithSlotNumber(GameStatus gameState, int slotNumber)
{
	if (slotNumber > SLOTS_NUM)
	{
		perror("not existed slot");
		return 1;
	}
	
	char * fullFileName = getFilenameBySlotNumber(slotNumber);
	
	int result =  saveFileWithFileName(gameState,fullFileName);
	free(fullFileName);
	return result;
}

GameStatus readFileWithFilename(char * filename)
{
	FILE * f = fopen(filename, "r");
	GameStatus gameState = { .nextTurn = 0, .gameMode = 0, .userColor = WHITE };
	if (f == NULL)
	{
		printf("%s",WRONG_FILE_NAME);
		gameState.nextTurn = -1;
		return gameState;
	}

	//free(filename);

	char nextTurn[100];
	fscanf(f, "<?xml version=\"1.0\" encoding=\"UTF - 8\"?>\n<game>\n\t<next_turn>%[^<]</next_turn>\n", nextTurn);
  	gameState.nextTurn = strcmp(nextTurn, "Black") ? WHITE : BLACK;
	fscanf(f, "\t<game_mode>%d</game_mode>\n", &gameState.gameMode);
	char diff[100];
	diff[0] = '1';
	diff[1] = 0;
	int currentOffset = ftell(f);
	if (!fscanf(f, "\t<difficulty>%[^<]</difficulty>\n", diff))
	{
		fseek(f, currentOffset, SEEK_SET);
		fscanf(f, "\t<difficulty></difficulty>\n");
	}
	gameState.difficulty = atoi(diff);
	char userColorStr[100];
	currentOffset = ftell(f);
	userColorStr[0] = 0;
	if (!fscanf(f, "\t<user_color>%[^<]</user_color>\n", userColorStr))
	{
		fseek(f, currentOffset, SEEK_SET);
		fscanf(f, "\t<user_color></user_color>\n");
	}

	gameState.userColor = strcmp(userColorStr, "Black") ? WHITE : BLACK;

	fscanf(f, "\t<board>\n");
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		int rowIndex = 0;
		int rowIndex2 = 0;
		char row[9];
		fscanf(f, "\t\t<row_%d>%[^<]</row_%d>\n", &rowIndex, row, &rowIndex2);
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			if (row[j] == '_')
				gameState.board[j][rowIndex - 1] = EMPTY;
			else
				gameState.board[j][rowIndex - 1] = row[j];
		}
	}

	return gameState;
}

GameStatus readFileWithSlotNumber(int slotNumber)
{
	char * fullFileName = getFilenameBySlotNumber(slotNumber);
	GameStatus gs =  readFileWithFilename(fullFileName);
	free(fullFileName);
	return gs;
}
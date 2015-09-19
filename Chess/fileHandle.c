#include "Chess.h"

#include "string.h"
#include "stdio.h"

int isFile()
{
	return 0;
}

char * getFilenameBySlotNumber(int slotNumber)
{
	char slotStr[2];
	sprintf(slotStr, "%d", slotNumber);

	char * fullFileName = malloc(sizeof(char)*100);
	sprintf(fullFileName, "%s%s%s", FILENAME, slotStr, FILEEXT);

	return fullFileName;
}

int saveFile(GameStatus gameState, int slotNumber)
{
	if (slotNumber > SLOTS_NUM)
	{
		perror("not existed slot");
		return 1;
	}
	
	const char * fullFileName = getFilenameBySlotNumber(slotNumber);
	//char * fullFileName = strcat(strcat(FILENAME, slotStr), FILEEXT);
	FILE *f = fopen(fullFileName, "w");
	free(fullFileName);
	if (f == NULL)
	{
		perror("IO error");
		return 1;
	}
	
	fprintf(f, "%s", "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
	fprintf(f, "%s", "<game>\n");
	fprintf(f, "\t<next_turn>%d</next_turn>\n", gameState.nextTurn);
	fprintf(f, "\t<game_mode>%d</game_mode>\n", gameState.gameMode);
	fprintf(f, "\t<difficulty>%s</difficulty>\n", gameState.difficulty);

	char * userColorStr;
	if (gameState.userColor == BLACK)
		userColorStr = "BLACK";
	
	if (gameState.userColor == WHITE)
		userColorStr = "WHITE";
	
	fprintf(f, "\t<user_color>%s</user_color>\n", userColorStr);
	fprintf(f, "%s", "\t<board>\n");


	for (int i = BOARD_SIZE - 1; i >= 0; i--)
	{
		fprintf(f, "\t\t<row_%d>", i+1);
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			if (board[j][i] == EMPTY)
			{
				fputc('_', f);
			}
			else
			{
				fputc(board[j][i],f);
			}
		}
		fprintf(f, "</row_%d>\n", i+1);
	}
	fprintf(f, "%s", "\t</board>\n");
	fprintf(f, "%s", "</game>");

	fclose(f);
	return 0;
}

GameStatus readFile(int slotNumber)
{
	char * fullFileName = getFilenameBySlotNumber(slotNumber);
	FILE * f = fopen(fullFileName, "r");
	free(fullFileName);

	GameStatus gameState = { .nextTurn = 0, .gameMode = 0,  .userColor = 0};
	//fscanf(f)
	
	fscanf(f, "<?xml version=\"1.0\" encoding=\"UTF - 8\"?>\n<game>\n\t<next_turn>%d</next_turn>\n", &gameState.nextTurn);
	fscanf(f, "\t<game_mode>%d</game_mode>\n", &gameState.gameMode);
	char diff[100];
	fscanf(f, "\t<difficulty>%[^<]</difficulty>\n", diff);
	strcpy(gameState.difficulty, diff);
	char userColorStr[100];
	fscanf(f, "\t<user_color>%[^<]</user_color>\n", userColorStr);
	
	gameState.userColor = strcmp(userColorStr, "BLACK") ? WHITE :  BLACK ;
	
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		int rowIndex = 0;
		char row[8];
		fscanf(f, "<row_%d>%s<row_", &rowIndex, &row);
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			gameState.board[j][rowIndex] = row[i];
		}
	}

	return gameState;
}
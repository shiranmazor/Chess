#include "Console.h"
#include "gui.h"

int main(int argc, char* args[])
{
	if (strcmp(args[1], "console") == 0)
		runConsole();
	else if (strcmp(args[1], "gui") == 0)
		RunGui();
	else
		runConsole();
}
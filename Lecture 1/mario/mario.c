#include <stdbool.h>
#include <stdio.h>

typedef unsigned int uint;

uint maxHeight;

static uint GetHeight();
static void PrintChar(char characterToPrint, uint timesToPrint);
static void PrintPyramid(uint maxHeight);

int main(void)
{
	maxHeight = GetHeight();

	PrintPyramid(maxHeight);
}

static uint GetHeight()
{
	int sscanResult;
	char inputLine[3];

	do
	{
		printf_s("Enter a number between 1 and 8.\n");
		printf_s("Height: ");
		fgets(inputLine, 3, stdin);
		/* 1st char is our height, 2nd is a new line (\n),
		3rd is the null char (\0), which is added automatically
		by fgets() as strings in C should be null-terminated.
		Which means if we read only 2 chars, the last char will be \0
		and any input will be discarded by the next if statement */

		sscanResult = sscanf_s(inputLine, "%d", &maxHeight);

		int inputLength = strlen(inputLine);
		if (inputLine[inputLength - 1] == '\n')
		{
			inputLine[inputLength - 1] = 0;
		}
		else
		{
			scanf_s("%*[^\n]");
			scanf_s("%*c"); //clear up to newline
			continue;
		}
	}
	while (sscanResult != 1 || maxHeight < 1 || maxHeight > 8);

	return maxHeight;
}

static void PrintChar(char characterToPrint, uint timesToPrint)
{
	if (timesToPrint == 0)
	{
		return;
	}
	for (uint i = 0; i < timesToPrint; i++)
	{
		printf("%c", characterToPrint);
	}
}

static void PrintPyramid(uint maxHeight)
{
	for (uint currentHeight = 0; currentHeight < maxHeight; currentHeight++)
	{
		PrintChar(' ', maxHeight - currentHeight - 1);
		/* print (maxHeight - currentHeight - 1) spaces,
		e.g. if height == 4 and it's a first iteration,
		we need to print 3 spaces; this is made to align our pyramid */
		PrintChar('#', currentHeight + 1);
		printf("  ");
		PrintChar('#', currentHeight + 1);
		printf("\n");
	}
}
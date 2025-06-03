#include <stdio.h>

static void PrintChar(char character, unsigned int times);

int main(void)
{
	unsigned int height;
	int result;
	char line[3];

	do
	{
		printf_s("Enter a number between 1 and 8.\n");
		printf_s("Height: ");
		fgets(line, 3, stdin);
		/* 1st char is our height, 2nd is a new line (\n),
		3rd is the null char (\0), which is added automatically
		by fgets() as strings in C should be null-terminated.
		Which means if we read only 2 chars, the last char will be \0
		and any input will be discarded by the next if statement */

		result = sscanf_s(line, "%d", &height);

		int len = strlen(line);
		if (line[len - 1] == '\n')
		{
			line[len - 1] = 0;
		}
		else
		{
			scanf_s("%*[^\n]");
			scanf_s("%*c"); //clear up to newline
		}
	} while (result != 1 || height < 1 || height > 8);

	for (unsigned int currentHeight = 0; currentHeight < height; currentHeight++)
	{
		PrintChar(' ', height - currentHeight - 1);
		/* print (height - currentHeight) spaces,
		e.g. if height == 4 and it's a first iteration,
		we need to print 3 spaces; this is made to align our pyramid */
		PrintChar('#', currentHeight + 1);
		printf("  ");
		PrintChar('#', currentHeight + 1);
		printf("\n");
	}
}

static void PrintChar(char character, unsigned int times)
{
	if (times == 0)
	{
		return;
	}
	for (unsigned int i = 0; i < times; i++)
	{
		printf("%c", character);
	}
}
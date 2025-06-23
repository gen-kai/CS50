#include <ctype.h>
#include <stdio.h>
#include <string.h>

typedef unsigned int u_int;

#define WORD_MAX_SIZE 31
// 30 actual symbols, and a '\0' byte

u_int PromptForWord(char *enteredWord);
u_int CountScore(char *playerWord, u_int wordLength);

int main(void)
{
	char firstPlayerWord[WORD_MAX_SIZE], secondPlayerWord[WORD_MAX_SIZE];
	u_int firstPlayerWordLength, secondPlayerWordLength;
	u_int firstPlayerScore, secondPlayerScore;

	printf("Player 1: ");
	firstPlayerWordLength = PromptForWord(firstPlayerWord);

	printf("Player 2: ");
	secondPlayerWordLength = PromptForWord(secondPlayerWord);

	printf_s("%s %d\n%s %d", firstPlayerWord, firstPlayerWordLength, secondPlayerWord, secondPlayerWordLength);

	firstPlayerScore = CountScore(firstPlayerWord, firstPlayerWordLength);
	secondPlayerScore = CountScore(secondPlayerWord, secondPlayerWordLength);

	if (firstPlayerScore > secondPlayerScore)
	{
		printf("Player 1 wins!");
	}
	else if (firstPlayerScore < secondPlayerScore)
	{
		printf("Player 2 wins!");
	}
	else
	{
		printf("Tie!");
	}
}

u_int PromptForWord(char *enteredWord)
{
	char inputLine[WORD_MAX_SIZE];

	fgets(inputLine, WORD_MAX_SIZE, stdin);

	u_int inputLineLength = (u_int) strlen(inputLine);

	if (inputLine[inputLineLength - 1] == '\n')
	{
		inputLine[inputLineLength - 1] = 0;
	}
	else
	{
		scanf_s("%*[^\n]");
		scanf_s("%*c"); // clear up to new line
	}

	sprintf_s(enteredWord, WORD_MAX_SIZE, "%s", inputLine);

	for (u_int iterator = 0; enteredWord[iterator] != '\0'; iterator++)
	{
		enteredWord[iterator] = toupper(enteredWord[iterator]);
	}

	return (u_int) strlen(enteredWord);
}

u_int CountScore(char *playerWord, u_int wordLength)
{
	u_int playerScore = 0;

	for (u_int iterator = 0; iterator < wordLength; iterator++)
	{
		if (!(playerWord[iterator] >= 'A' && playerWord[iterator] <= 'Z'))
		{
			continue;
		}

		switch (playerWord[iterator])
		{
			case 'A':
				playerScore += 1;
				break;
			case 'B':
				playerScore += 3;
				break;
			case 'C':
				playerScore += 3;
				break;
			case 'D':
				playerScore += 2;
				break;
			case 'E':
				playerScore += 1;
				break;
			case 'F':
				playerScore += 4;
				break;
			case 'G':
				playerScore += 2;
				break;
			case 'H':
				playerScore += 4;
				break;
			case 'I':
				playerScore += 1;
				break;
			case 'J':
				playerScore += 8;
				break;
			case 'K':
				playerScore += 5;
				break;
			case 'L':
				playerScore += 1;
				break;
			case 'M':
				playerScore += 3;
				break;
			case 'N':
				playerScore += 1;
				break;
			case 'O':
				playerScore += 1;
				break;
			case 'P':
				playerScore += 3;
				break;
			case 'Q':
				playerScore += 10;
				break;
			case 'R':
				playerScore += 1;
				break;
			case 'S':
				playerScore += 1;
				break;
			case 'T':
				playerScore += 1;
				break;
			case 'U':
				playerScore += 1;
				break;
			case 'V':
				playerScore += 4;
				break;
			case 'W':
				playerScore += 4;
				break;
			case 'X':
				playerScore += 8;
				break;
			case 'Y':
				playerScore += 4;
				break;
			case 'Z':
				playerScore += 10;
				break;
		}
	}

	return playerScore;
}
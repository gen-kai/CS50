#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned int uint;

#define KEY_MAX_SIZE 28
#define TEXT_MAX_SIZE 2001
#define ALPHABET_SIZE 26

bool IsOnlyLetters(char *enteredKey, uint enteredKeyLength);
void ConvertToUpper(char *enteredKey, uint enteredKeyLength);
uint PromptForText(char *enteredText);
bool AreLettersUnique(char *enteredKey, uint enteredKeyLength);
void EncryptText(char *enteredText, uint enteredTextLength, char *enteredKey, char *encryptedText);

int main(int argc, char *argv[])
{
	char *enteredKey = malloc(KEY_MAX_SIZE * sizeof(char));
	uint enteredKeyLength;

	char enteredText[TEXT_MAX_SIZE];
	uint enteredTextLength;

	char encryptedText[TEXT_MAX_SIZE];

	if (argc != 2)
	{
		printf("Usage: substitution <26-letter key>\n");
		return 1;
	}

	memcpy(enteredKey, argv[1], KEY_MAX_SIZE);
	enteredKeyLength = (uint) strlen(enteredKey);

	if (enteredKeyLength != 26)
	{
		printf("Wrong key length!\n");

		free(enteredKey);
		enteredKey = NULL;

		return 1;
	}

	if (!IsOnlyLetters(enteredKey, enteredKeyLength))
	{
		printf("Key must contain only letters!\n");

		free(enteredKey);
		enteredKey = NULL;

		return 1;
	}

	ConvertToUpper(enteredKey, enteredKeyLength);

	if (!AreLettersUnique(enteredKey, enteredKeyLength))
	{
		printf("All letters in the key must be unique!\n");
		
		free(enteredKey);
		enteredKey = NULL;

		return 1;
	}

	enteredTextLength = PromptForText(enteredText);

	EncryptText(enteredText, enteredTextLength, enteredKey, encryptedText);

	free(enteredKey);
	enteredKey = NULL;

	printf("ciphertext: %s\n", encryptedText);
}

bool IsOnlyLetters(char *enteredKey, uint enteredKeyLength)
{
	for (uint iterator = 0; iterator < enteredKeyLength; iterator++)
	{
		if (!isalpha(enteredKey[iterator]))
		{
			return false;
		}
	}

	return true;
}

void ConvertToUpper(char *enteredKey, uint enteredKeyLength)
{
	for (uint iterator = 0; iterator < enteredKeyLength; iterator++)
	{
		enteredKey[iterator] = toupper(enteredKey[iterator]);
	}
}

uint PromptForText(char *enteredText)
{
	char inputLine[TEXT_MAX_SIZE];

	printf("plaintext: ");

	fgets(inputLine, TEXT_MAX_SIZE, stdin);

	uint inputLineLength = (uint) strlen(inputLine);

	if (inputLine[inputLineLength - 1] == '\n')
	{
		inputLine[inputLineLength - 1] = 0;
	}
	else
	{
		scanf_s("%*[^\n]");
		scanf_s("%*c"); // clear up to new line
	}

	sprintf_s(enteredText, TEXT_MAX_SIZE, "%s", inputLine);

	return (uint)strlen(enteredText);
}

bool AreLettersUnique(char *enteredKey, uint enteredKeyLength)
{
	uint letterOccurances[ALPHABET_SIZE] = {0};

	for (uint iterator = 0; iterator < enteredKeyLength; iterator++)
	{
		char currentLetter = enteredKey[iterator];

		if (isalpha(currentLetter))
		{
			letterOccurances[currentLetter - 'A'] += 1;
		}
	}

	for (uint iterator = 0; iterator < ALPHABET_SIZE; iterator++)
	{
		if (letterOccurances[iterator] != 1)
		{
			return false;
		}
	}

	return true;
}

void EncryptText(char *enteredText, uint enteredTextLength, char *enteredKey, char *encryptedText)
{
	for (uint iterator = 0; iterator < enteredTextLength; iterator++)
	{
		if (isalpha(enteredText[iterator]))
		{
			if (isupper(enteredText[iterator]))
			{
				encryptedText[iterator] = enteredKey[enteredText[iterator] - 'A'];
			}
			else
			{
				encryptedText[iterator] = tolower(enteredKey[enteredText[iterator] - 'a']);
			}
		}
		else
		{
			encryptedText[iterator] = enteredText[iterator];
		}
	}

	encryptedText[enteredTextLength] = '\0';
}
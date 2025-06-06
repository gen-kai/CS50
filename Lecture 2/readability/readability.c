#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

typedef unsigned int uint;

#define TEXT_MAX_SIZE 2001

uint PromptForText(char *enteredText);

bool IsEndOfWord(char stringChar);
bool IsEndOfSentence(char stringChar);

uint CountWords(char *enteredText, uint enteredTextLength);
uint CountSentences(char *enteredText, uint enteredTextLength);
uint CountLetters(char *enteredText, uint enteredTextLength);
float CalculateLettersPerHundredWords(uint letterCount, uint wordCount);
float CalculateSentencesPerHundredWords(uint wordCount, uint sentenceCount);
float CalculateTextComplexity(float lettersPerHundredWords, float sentencesPerHundredWords);

int main(void)
{
	char enteredText[TEXT_MAX_SIZE];
	uint enteredTextLength;
	uint letterCount, wordCount, sentenceCount;
	float lettersPerHundredWords, sentencesPerHundredWords;
	float textComplexity;

	enteredTextLength = PromptForText(enteredText);

	letterCount = CountLetters(enteredText, enteredTextLength);
	wordCount = CountWords(enteredText, enteredTextLength);
	sentenceCount = CountSentences(enteredText, enteredTextLength);

	lettersPerHundredWords = CalculateLettersPerHundredWords(letterCount, wordCount);
	sentencesPerHundredWords = CalculateSentencesPerHundredWords(wordCount, sentenceCount);

	textComplexity = CalculateTextComplexity(lettersPerHundredWords, sentencesPerHundredWords);

	if (textComplexity < 1.0)
	{
		printf("Before Grade 1\n");
	}
	else if (textComplexity > 16.0)
	{
		printf("Grade 16+\n");
	}
	else
	{
		printf("Grade %d\n", (uint) round(textComplexity));
	}
};

uint PromptForText(char *enteredText)
{
	char inputLine[TEXT_MAX_SIZE];

	printf("Text: ");

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

	return (uint) strlen(enteredText);
}

bool IsEndOfWord(char stringChar)
{
	bool isLetter = isalpha(stringChar);
	bool isComma = stringChar == ',';
	bool isSemiColon = stringChar == ';';
	bool isColon = stringChar == ':';
	bool isPeriod = stringChar == '.';
	bool isExclamationMark = stringChar == '!';
	bool isQuestionMark = stringChar == '?';
	bool isDoubleQuote = stringChar == '\"';

	return isLetter || isComma || isSemiColon || isColon || isPeriod || isExclamationMark || isQuestionMark || isDoubleQuote;
}

bool IsEndOfSentence(char stringChar)
{
	bool isPeriod = stringChar == '.';
	bool isExclamationMark = stringChar == '!';
	bool isQuestionMark = stringChar == '?';

	return isPeriod || isExclamationMark || isQuestionMark;
}

uint CountLetters(char *enteredText, uint enteredTextLength)
{
	uint letterCount = 0;

	for (uint iterator = 0; iterator < enteredTextLength; iterator++)
	{
		if (isalpha(enteredText[iterator]))
		{
			letterCount += 1;
		}
	}

	return letterCount;
}

uint CountWords(char *enteredText, uint enteredTextLength)
{
	uint wordCount = 0;

	for (uint iterator = 0; iterator < enteredTextLength - 1; iterator++)
	{
		if(IsEndOfWord(enteredText[iterator]) && enteredText[iterator + 1] == ' ')
		// "a ", ", ", ". ", "! ", "? ", "\" "
		{
			wordCount += 1;
		}
	}

	return wordCount + 1;
	// because this method won't count the last word
}

uint CountSentences(char *enteredText, uint enteredTextLength)
{
	uint sentenceCount = 0;

	for (uint iterator = 0; iterator < enteredTextLength - 1; iterator++)
	{
		if (IsEndOfSentence(enteredText[iterator]) && enteredText[iterator + 1] == ' ')
		// ". ", "! ", "? "
		{
			sentenceCount += 1;
		}
	}

	return sentenceCount + 1;
	// because this method won't count the last sentence
}

float CalculateLettersPerHundredWords(uint letterCount, uint wordCount)
{
	return (float) letterCount / (float) wordCount * 100.0;
}

float CalculateSentencesPerHundredWords(uint wordCount, uint sentenceCount)
{
	return (float)sentenceCount / (float) wordCount * 100.0;
}

float CalculateTextComplexity(float lettersPerHundredWords, float sentencesPerHundredWords)
{
	return (0.0588 * lettersPerHundredWords) - (0.296 * sentencesPerHundredWords) - 15.8;
}
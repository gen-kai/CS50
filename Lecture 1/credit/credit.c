#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned int uint;

#define NUMBER_LINE_MAX_SIZE 18
/* 17th is a new line (\n), if 16 chars are entered; 18th is the null char (\0),
which is added automatically by fgets() as strings in C should be null-terminated.
Which means if we read only 16 chars, the last char will be \0
and any input will be discarded by the next if statement */

static const uint AMEX_LENGTH = 15;
static const uint MASTERCARD_LENGTH = 16;
static const uint VISA_LENGTH_1 = 13;
static const uint VISA_LENGTH_2 = 16;

static const uint AMEX_FIRST_DIGIT = '3';
static const uint AMEX_SECOND_DIGIT = '7';

static const uint MASTERCARD_FIRST_DIGIT = '5';
static const uint MASTERCARD_SECOND_DIGIT[5] = { '1', '2', '3', '4', '5' };

static const uint VISA_FIRST_DIGIT = '4';

static bool IsDigitsOnly(char *stringToCheck);
static bool IsChecksumValid(char string[]);
static bool IsAmex(uint numberOfDigits, char cardNumber[]);
static bool IsMastercard(uint numberOfDigits, char cardNumber[]);
static bool IsVisa(uint numberOfDigits, char cardNumber[]);

int main(void)
{
	char inputLine[NUMBER_LINE_MAX_SIZE] = { 0 };
	bool isLessThan17;
	uint numberOfDigits = 0;
	char cardNumber[NUMBER_LINE_MAX_SIZE] = { 0 };

	printf("Enter a credit card number: \n");
	fgets(inputLine, NUMBER_LINE_MAX_SIZE, stdin);
	
	isLessThan17 = false;
	int inputLineLength = strlen(inputLine);
	if (inputLine[inputLineLength - 1] == '\n')
	{
		isLessThan17 = true;
		inputLine[inputLineLength - 1] = 0;
	}

	if (!isLessThan17) // if the input is equal or more than 17
	{
		scanf_s("%*[^\n]");
		scanf_s("%*c"); // clear up to newline
	}

	numberOfDigits = sprintf_s(cardNumber, NUMBER_LINE_MAX_SIZE, "%s", inputLine);

	if (!IsDigitsOnly(inputLine))
	{
		printf("INVALID\n");
		return;
	}

	if (IsAmex(numberOfDigits, cardNumber) && IsChecksumValid(cardNumber))
	{
		printf("AMEX\n");
	}
	else if (IsMastercard(numberOfDigits, cardNumber) && IsChecksumValid(cardNumber))
	{
		printf("MASTERCARD\n");
	}
	else if (IsVisa(numberOfDigits, cardNumber) && IsChecksumValid(cardNumber))
	{
		printf("VISA\n");
	}
	else
	{
		printf("INVALID\n");
	}
}

static bool IsDigitsOnly(char* stringToCheck)
{
	while (*stringToCheck)
	{
		if (*stringToCheck < '0' || *stringToCheck > '9')
		{
			return false;
		}
		stringToCheck++;
	}

	return true;
}

static bool IsChecksumValid(char stringToCheck[])
{
	uint stringLength = 0, stringChecksum = 0;

	stringLength = strlen(stringToCheck);

	for (uint iterator = 0; iterator < stringLength - 1; iterator += 2)
	{
		uint currentNumber = (stringToCheck[(stringLength - 2) - iterator] - '0') * 2;
		while (currentNumber != 0)
		{
			stringChecksum = stringChecksum + (currentNumber % 10);
			currentNumber = currentNumber / 10;
		}
	}

	for (uint iterator = 0; iterator < stringLength; iterator += 2)
	{
		stringChecksum += (stringToCheck[(stringLength - 1) - iterator] - '0');
	}

	if (stringChecksum % 10 == 0)
	{
		return true;
	}

	return false;
}

static bool IsAmex(uint numberOfDigits, char cardNumber[])
{
	if (numberOfDigits == AMEX_LENGTH
		&& (cardNumber[0] == AMEX_FIRST_DIGIT
			&& cardNumber[1] == AMEX_SECOND_DIGIT))
	{
		return true;
	}
	return false;
}

static bool IsMastercard(uint numberOfDigits, char cardNumber[])
{
	if (numberOfDigits == MASTERCARD_LENGTH
		&& (cardNumber[0] == MASTERCARD_FIRST_DIGIT
			&& (cardNumber[1] == MASTERCARD_SECOND_DIGIT[0]
				|| cardNumber[1] == MASTERCARD_SECOND_DIGIT[1]
				|| cardNumber[1] == MASTERCARD_SECOND_DIGIT[2]
				|| cardNumber[1] == MASTERCARD_SECOND_DIGIT[3]
				|| cardNumber[1] == MASTERCARD_SECOND_DIGIT[4])))
	{
		return true;
	}
	return false;
}

static bool IsVisa(uint numberOfDigits, char cardNumber[])
{
	if ((numberOfDigits == VISA_LENGTH_1
		|| numberOfDigits == VISA_LENGTH_2)
		&& cardNumber[0] == VISA_FIRST_DIGIT)
	{
		return true;
	}
	return false;
}

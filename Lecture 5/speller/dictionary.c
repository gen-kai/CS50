// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"

typedef unsigned int u_int;

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 100000;

const u_int wordPartLength = 4;

// Hash table
node *table[N];

// ---------------------------------
// My code starts here
static bool CheckWordSpelling(const char *pointerToWord, node *pointerToNode);
static bool AddWordToDictionary(u_int hashTableIndex, char *pointerToWord);
// My code ends here
// ---------------------------------

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // ---------------------------------
    // My code starts here
    u_int hashedWord = hash(word);
    node *p_currentNode = table[hashedWord];


    if (p_currentNode == NULL)
    {
        return false;
    }
    else
    {
        while (p_currentNode->next != NULL)
        {
            if (CheckWordSpelling(word, p_currentNode))
            {
                return true;
            }


            p_currentNode = p_currentNode->next;
        }


        if (CheckWordSpelling(word, p_currentNode))
        {
            return true;
        }


        return false;
    }
    // My code ends here
    // ---------------------------------
}

// ---------------------------------
// My code starts here
static bool CheckWordSpelling(const char *pointerToWord, node *pointerToNode)
{
    if (strlen(pointerToWord) != strlen(pointerToNode->word))
    {
        return false;
    }


    char lowercaseWord[LENGTH + 1] = {0};

    for (u_int charIterator = 0; pointerToWord[charIterator] != '\0'; charIterator++)
    {
        lowercaseWord[charIterator] = tolower(pointerToWord[charIterator]);
    }


    if (strcmp(lowercaseWord, pointerToNode->word) != 0)
    {
        return false;
    }


    return true;
}
// My code ends here
// ---------------------------------

// Hashes word to a number
unsigned int hash(const char *word)
{
    // ---------------------------------
    // My code starts here
    u_int resultingHash = 0;
    char lowercaseWord[LENGTH + 1] = {0};
    u_int wordLength = strlen(word);
    void(*wordParts[(LENGTH + 1) / wordPartLength]);


    for (u_int charIterator = 0; word[charIterator] != '\0'; charIterator++)
    {
        lowercaseWord[charIterator] = tolower(word[charIterator]);
    }


    if (wordLength <= wordPartLength)
    {
        resultingHash = 2153014U * (*(u_int *) &lowercaseWord);
    }
    else
    {
        for (u_int wordPartsIterator = 0; wordPartsIterator < wordLength; wordPartsIterator += wordPartLength)
        {
            wordParts[wordPartsIterator / wordPartLength] = (u_int *) &lowercaseWord[wordPartsIterator];
        }


        for (u_int wordPartsIterator = 0; wordPartsIterator < wordLength; wordPartsIterator += wordPartLength)
        {
            if ((wordPartsIterator / wordPartLength) == ((LENGTH + 1) / wordPartLength))
            {
                resultingHash += 2153014U * (* (unsigned short *) wordParts[wordPartsIterator / wordPartLength]);
                continue;
            }
            resultingHash += 2153014U * (* (u_int *) wordParts[wordPartsIterator / wordPartLength]);
        }
    }


    return resultingHash % N;
    // My code ends here
    // ---------------------------------
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // ---------------------------------
    // My code starts here
    FILE *p_dictionaryFile = fopen(dictionary, "r");

    if (p_dictionaryFile == NULL)
    {
        return false;
    }

    fseek(p_dictionaryFile, 0, SEEK_END);
    u_int fileSize = ftell(p_dictionaryFile);
    rewind(p_dictionaryFile);

    char fileBuffer[fileSize + 1];
    u_int currentPosition = fread(fileBuffer, 1, fileSize, p_dictionaryFile);
    fileBuffer[fileSize] = '\0';
    fclose(p_dictionaryFile);

    char *dictionaryWord = strtok(fileBuffer, "\n");

    while (dictionaryWord != NULL)
    {
        if (!AddWordToDictionary(hash(dictionaryWord), dictionaryWord))
        {
            unload();
            return false;
        }

        dictionaryWord = strtok(NULL, "\n");
    }


    return true;
    // My code ends here
    // ---------------------------------
}

// ---------------------------------
// My code starts here
static bool AddWordToDictionary(u_int hashTableIndex, char *pointerToWord)
{
    node *p_currentNode = table[hashTableIndex];

    if (p_currentNode == NULL)
    {
        table[hashTableIndex] = calloc(1, sizeof(node));

        if (table[hashTableIndex] == NULL)
        {
            return false;
        }


        p_currentNode = table[hashTableIndex];
    }
    else
    {
        while (p_currentNode->next != NULL)
        {
            p_currentNode = p_currentNode->next;
        }


        p_currentNode->next = calloc(1, sizeof(node));

        if (p_currentNode->next == NULL)
        {
            return false;
        }


        p_currentNode = p_currentNode->next;
    }


    for (u_int charIterator = 0; charIterator < strlen(pointerToWord); charIterator++)
    {
        p_currentNode->word[charIterator] = pointerToWord[charIterator];
    }

    p_currentNode->word[strlen(pointerToWord)] = '\0';


    return true;
}
// My code ends here
// ---------------------------------

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // ---------------------------------
    // My code starts here
    u_int dictionarySize = 0;

    for (u_int nodeIterator = 0; nodeIterator < N; nodeIterator++)
    {
        node *p_currentNode = table[nodeIterator];

        if (p_currentNode != NULL)
        {
            dictionarySize++;

            while (p_currentNode->next != NULL)
            {
                p_currentNode = p_currentNode->next;
                dictionarySize++;
            }
        }
    }


    return dictionarySize;
    // My code ends here
    // ---------------------------------
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // ---------------------------------
    // My code starts here
    for (u_int tableIterator = 0; tableIterator < N; tableIterator++)
    {
        node *p_currentNode = table[tableIterator];
        node *p_previousNode;

        if (p_currentNode == NULL)
        {
            continue;
        }

        while (p_currentNode->next != NULL)
        {
            p_previousNode = p_currentNode;
            p_currentNode = p_currentNode->next;


            free(p_previousNode);
            p_previousNode = NULL;
        }

        if (p_currentNode->next == NULL)
        {
            free(p_currentNode);
            p_currentNode = NULL;
        }
    }


    return true;
    // My code ends here
    // ---------------------------------
}

// Modifies the volume of an audio file

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Number of bytes in .wav header
#define HEADER_SIZE 44
#define SAMPLE_SIZE 2

typedef unsigned int uint;

uint getFileSize(FILE *input);
uint copyHeader(FILE *input, FILE *output);
uint changeVolume(FILE *input, FILE *output, uint dataSize, float changeFactor);

int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 4)
    {
        printf("Usage: ./volume input.wav output.wav factor\n");
        return 1;
    }

    // Open files and determine scaling factor
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    float factor = atof(argv[3]);

    // Check file size

    uint fileSize = getFileSize(input);

    // Copy header from input file to output file
    if (copyHeader(input, output) == 1)
    {
        printf("Could not copy header.\n");
    }

    // Read samples from input file and write updated data to output file
    uint dataSize = fileSize - HEADER_SIZE;

    uint errorCode = changeVolume(input, output, dataSize, factor);

    if (errorCode != 0 && errorCode > dataSize)
    {
        printf("Could not read input sample %i.\n", errorCode - dataSize);
    }
    else if (errorCode != 0 && errorCode < dataSize)
    {
        printf("Could not write output sample %i.\n", dataSize - errorCode);
    }

    // Close files
    fclose(input);
    fclose(output);

    return 0;
}

uint getFileSize(FILE *input)
{
    fseek(input, 0, SEEK_END);
    uint fileSize = ftell(input);

    rewind(input);

    return fileSize;
}

uint copyHeader(FILE *input, FILE *output)
{
    char wavHeader[HEADER_SIZE];

    fread(wavHeader, 1, HEADER_SIZE, input);

    if (fwrite(wavHeader, 1, HEADER_SIZE, output) < 1)
    {
        return 1;
    }

    return 0;
}

uint changeVolume(FILE *input, FILE *output, uint dataSize, float changeFactor)
{
    for (uint iterator = 0; iterator < dataSize / 2; iterator++)
    {
        short inputSample;

        if (fread(&inputSample, 1, SAMPLE_SIZE, input) < 1)
        {
            return dataSize + iterator;
        }

        inputSample *= changeFactor;

        if (fwrite(&inputSample, 1, SAMPLE_SIZE, output) < 1)
        {
            printf("Could not copy output sample.\n");
            return dataSize - iterator;
        }
    }

    return 0;
}

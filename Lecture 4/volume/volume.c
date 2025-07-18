// Modifies the volume of an audio file

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Number of bytes in .wav header
#define HEADER_SIZE 44U
#define SAMPLE_SIZE 2U

typedef unsigned int u_int;

u_int getFileSize(FILE *input);
u_int copyHeader(FILE *input, FILE *output);
u_int changeVolume(FILE *input, FILE *output, u_int dataSize, float changeFactor);

int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 4)
    {
        printf("Usage: ./volume input.wav output.wav factor\n");
        return 1;
    }

    // Open files and determine scaling factor
    FILE *input = fopen(argv[1], "rb");
    if (input == NULL)
    {
        printf("Could not open input file.\n");
        return 1;
    }

    FILE *output = fopen(argv[2], "wb");
    if (output == NULL)
    {
        printf("Could not open output file.\n");
        fclose(input);
        return 2;
    }

    float factor = atof(argv[3]);

    // ---------------------------------
    // My code starts here

    // Check file size
    u_int fileSize = getFileSize(input);

    // Copy header from input file to output file
    if (copyHeader(input, output) == 1)
    {
        printf("Could not copy header.\n");
        fclose(input);
        fclose(output);
        return 3;
    }

    // Read samples from input file and write updated data to output file
    u_int dataSize = fileSize - HEADER_SIZE;

    u_int errorCode = changeVolume(input, output, dataSize, factor);

    if (errorCode != 0 && errorCode > dataSize)
    {
        printf("Could not read input sample %i.\n", errorCode - dataSize);
        fclose(input);
        fclose(output);
        return 4;
    }
    else if (errorCode != 0 && errorCode < dataSize)
    {
        printf("Could not write output sample %i.\n", dataSize - errorCode);
        fclose(input);
        fclose(output);
        return 5;
    }
    // My code ends here
    // ---------------------------------

    // Close files
    fclose(input);
    fclose(output);

    return 0;
}

u_int getFileSize(FILE *input)
{
    fseek(input, 0, SEEK_END);
    u_int fileSize = ftell(input);

    rewind(input);

    return fileSize;
}

u_int copyHeader(FILE *input, FILE *output)
{
    char wavHeader[HEADER_SIZE];

    fread(wavHeader, HEADER_SIZE, 1, input);

    if (fwrite(wavHeader, HEADER_SIZE, 1, output) < 1)
    {
        return 1;
    }

    return 0;
}

u_int changeVolume(FILE *input, FILE *output, u_int dataSize, float changeFactor)
{
    for (u_int iterator = 0; iterator < dataSize / 2; iterator++)
    {
        short inputSample;

        if (fread(&inputSample, SAMPLE_SIZE, 1, input) < 1)
        {
            return dataSize + iterator;
        }

        inputSample *= changeFactor;

        if (fwrite(&inputSample, SAMPLE_SIZE, 1, output) < 1)
        {
            return dataSize - iterator;
        }
    }

    return 0;
}

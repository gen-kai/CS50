#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef unsigned int u_int;
typedef unsigned char u_char;

static const u_int FILE_SYSTEM_BLOCK_SIZE = 512;
static const u_char JPEG_SIGNATURE[] = {0xff, 0xd8, 0xff, 0xe0};
enum { signatureByteCount = sizeof(JPEG_SIGNATURE) / sizeof(JPEG_SIGNATURE[0]) };

long GetEofPosition(FILE *pointerToFile, long currentPosition);
bool ReadSignatureBytes(FILE *pointerToFile, u_char (*signatureBytes)[signatureByteCount]);
bool IsJpegSignature(u_char signatureBytes[4]);
bool WriteFile(FILE *pointerToFileToRead, char recoveredJpegName[8], long eofPosition);
bool WriteFileBlock(FILE *pointerToFileToRead, FILE *pointerToFileToWrite, u_int blockSize);
bool SkipFileBlock(FILE *pointerToFileToRead, u_int blockSize);

int main(int argCount, char *p_argValues[])
{
    if (argCount != 2)
    {
        printf("Usage: recover image_name.ext\n");
        return 1;
    }


    FILE *p_inputImage = fopen(p_argValues[1], "rb");

    if (p_inputImage == NULL)
    {
        printf("Couldn't open the file supplied.\n");
        return 1;
    }


    long eofPosition;

    if (ftell(p_inputImage) == -1)
    {
        fclose(p_inputImage);

        printf("Couldn't determine current position.\n");
        return 1;
    }

    eofPosition = GetEofPosition(p_inputImage, ftell(p_inputImage));

    if (eofPosition == -1)
    {
        fclose(p_inputImage);
        return 1;
    }


    u_int fileCount = 0;

    while (ftell(p_inputImage) < eofPosition)
    {
        u_char currentSignatureBytes[signatureByteCount] = {0};

        if (ftell(p_inputImage) + signatureByteCount >= eofPosition)
        {
            break;
        }


        if (!ReadSignatureBytes(p_inputImage, &currentSignatureBytes))
        {
            fclose(p_inputImage);
            return 1;
        }


        if (IsJpegSignature(currentSignatureBytes))
        {
            char recoveredJpegName[8];

            if (sprintf(recoveredJpegName, "%03d.jpg", fileCount) != 7)
            {
                printf("Couldn't create file name for file %d", fileCount);
                fclose(p_inputImage);


                return 1;
            }


            if (!WriteFile(p_inputImage, recoveredJpegName, eofPosition))
            {
                fclose(p_inputImage);
                return 1;
            }


            fileCount++;
            continue;
        }


        if (ftell(p_inputImage) + (long) FILE_SYSTEM_BLOCK_SIZE >= eofPosition)
        {
            break;
        }


        if (!SkipFileBlock(p_inputImage, FILE_SYSTEM_BLOCK_SIZE))
        {
            fclose(p_inputImage);
            return 1;
        }
    }


    fclose(p_inputImage);
    return 0;
}

long GetEofPosition(FILE *pointerToFile, long currentPosition)
{
    long eofPosition;

    if (fseek(pointerToFile, 0, SEEK_END) != 0)
    {
        printf("Couldn't set position to EOF.\n");
        return -1;
    }

    eofPosition = ftell(pointerToFile);

    if (eofPosition == -1L)
    {
        printf("Couldn't determine current position.\n");
        return -1;
    }


    if (fseek(pointerToFile, currentPosition, SEEK_SET) != 0)
    {
        printf("Couldn't set previous position.\n");
        return -1;
    }


    return eofPosition;
}

bool ReadSignatureBytes(FILE *pointerToFile, u_char(*signatureBytes)[signatureByteCount])
{
    int signatureSize = signatureByteCount * sizeof(JPEG_SIGNATURE[0]);
    long currentPosition = ftell(pointerToFile);


    u_int readResult = fread(signatureBytes,
                             sizeof(JPEG_SIGNATURE[0]),
                             signatureByteCount,
                             pointerToFile);

    if (readResult < signatureByteCount)
    {
        printf("Couldn't read signature bytes.\n");
        return false;
    }


    if (fseek(pointerToFile, 0 - signatureSize, SEEK_CUR) != 0)
    {
        printf("Couldn't reset position after reading signature.\n");
        return false;
    }


    return true;
}

bool IsJpegSignature(u_char signatureBytes[4])
{
    bool isFirstByteEqualsToSignature = signatureBytes[0] == JPEG_SIGNATURE[0];
    bool isSecondByteEqualsToSignature = signatureBytes[1] == JPEG_SIGNATURE[1];
    bool isThirdByteEqualsToSignature = signatureBytes[2] == JPEG_SIGNATURE[2];
    bool isFourthByteEqualsToSignature = false;

    if ((signatureBytes[3] ^ JPEG_SIGNATURE[3]) < 0x10)
    {
        isFourthByteEqualsToSignature = true;
    }


    return isFirstByteEqualsToSignature
        && isSecondByteEqualsToSignature
        && isThirdByteEqualsToSignature
        && isFourthByteEqualsToSignature;
}

bool WriteFile(FILE *pointerToFileToRead, char recoveredJpegName[8], long eofPosition)
{
    FILE *p_recoveredJpeg = fopen(recoveredJpegName, "wb");

    if (p_recoveredJpeg == NULL)
    {
        printf("Couldn't open file %s.\n", recoveredJpegName);
        return false;
    }


    while (ftell(pointerToFileToRead) < eofPosition)
    {
        if (ftell(pointerToFileToRead) + (long) FILE_SYSTEM_BLOCK_SIZE > eofPosition)
        {
            break;
        }


        if (!WriteFileBlock(pointerToFileToRead, p_recoveredJpeg, FILE_SYSTEM_BLOCK_SIZE))
        {
            fclose(p_recoveredJpeg);
            return false;
        }


        u_char currentSignatureBytes[signatureByteCount] = {0};

        if (ftell(pointerToFileToRead) + signatureByteCount > eofPosition)
        {
            fclose(p_recoveredJpeg);
            return true;
        }


        if (!ReadSignatureBytes(pointerToFileToRead, &currentSignatureBytes))
        {
            fclose(p_recoveredJpeg);
            return false;
        }


        if (IsJpegSignature(currentSignatureBytes))
        {
            fclose(p_recoveredJpeg);
            return true;
        }
    }


    fclose(p_recoveredJpeg);
    return true;
}

bool WriteFileBlock(FILE *pointerToFileToRead, FILE *pointerToFileToWrite, u_int blockSize)
{
    void *blockSizedBuffer = malloc(blockSize);

    if (blockSizedBuffer == NULL)
    {
        printf("Couldn't allocatebuffer.\n");
        return false;
    }


    u_int readResult = fread(blockSizedBuffer, blockSize, 1, pointerToFileToRead);

    if (readResult != 1)
    {
        free(blockSizedBuffer);
        blockSizedBuffer = NULL;


        printf("Couldn't read block.\n");
        return false;
    }


    u_int writeResult = fwrite(blockSizedBuffer, blockSize, 1, pointerToFileToWrite);

    if (writeResult != 1)
    {
        free(blockSizedBuffer);
        blockSizedBuffer = NULL;


        printf("Couldn't write block.\n");
        return false;
    }


    free(blockSizedBuffer);
    blockSizedBuffer = NULL;


    return true;
}

bool SkipFileBlock(FILE *pointerToFileToRead, u_int blockSize)
{
    void *blockSizedBuffer = malloc(blockSize);

    if (blockSizedBuffer == NULL)
    {
        printf("Couldn't allocatebuffer.\n");
        return false;
    }


    u_int readResult = fread(blockSizedBuffer, blockSize, 1, pointerToFileToRead);

    if (readResult != 1)
    {
        free(blockSizedBuffer);
        blockSizedBuffer = NULL;

        printf("Couldn't read block.\n");
        return false;
    }


    free(blockSizedBuffer);
    blockSizedBuffer = NULL;


    return true;
}

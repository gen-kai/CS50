#include <math.h>
#include <stdlib.h>

#include "helpers.h"

typedef unsigned int u_int;
// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (u_int heightIterator = 0; heightIterator < height; heightIterator++)
    {
        for (u_int widthIterator = 0; widthIterator < width; widthIterator++)
        {
            RGBTRIPLE currentPixel = image[heightIterator][widthIterator];

            BYTE resultingColor = round((currentPixel.rgbtBlue + currentPixel.rgbtGreen + currentPixel.rgbtRed) / 3.0);
            image[heightIterator][widthIterator].rgbtBlue = resultingColor;
            image[heightIterator][widthIterator].rgbtGreen = resultingColor;
            image[heightIterator][widthIterator].rgbtRed = resultingColor;
        }
    }

    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE *resultingLine = calloc(1, width * sizeof(RGBTRIPLE));
    if (resultingLine == NULL)
    {
        return;
    }

    for (u_int heightIterator = 0; heightIterator < height; heightIterator++)
    {
        for (u_int widthIterator = 0; widthIterator < width; widthIterator++)
        {
            resultingLine[widthIterator] = image[heightIterator][width - widthIterator - 1];
        }

        for (u_int widthIterator = 0; widthIterator < width; widthIterator++)
        {
            image[heightIterator][widthIterator] = resultingLine[widthIterator];
        }
    }

    free(resultingLine);
    resultingLine = NULL;

    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE(*resultingImage)[width] = calloc(height, width * sizeof(RGBTRIPLE));
    if (image == NULL)
    {
        return;
    }

    for (u_int heightIterator = 0; heightIterator < height; heightIterator++)
    {
        for (u_int widthIterator = 0; widthIterator < width; widthIterator++)
        {
            RGBTRIPLE blurBox[3][3] = {{0}};

            u_int blurBoxHeightIteratorStart = 0;
            u_int blurBoxMaxHeight = 2;

            u_int blurBoxWidthIteratorStart = 0;
            u_int blurBoxMaxWidth = 2;

            if (heightIterator == 0)
            {
                blurBoxHeightIteratorStart = 1;
            }
            if (heightIterator == height - 1)
            {
                blurBoxMaxHeight = 1;
            }
            if (widthIterator == 0)
            {
                blurBoxWidthIteratorStart = 1;
            }
            if (widthIterator == width - 1)
            {
                blurBoxMaxWidth = 1;
            }

            for (u_int blurBoxHeightIterator = blurBoxHeightIteratorStart; blurBoxHeightIterator <= blurBoxMaxHeight; blurBoxHeightIterator++)
            {
                for (u_int blurBoxWidthIterator = blurBoxWidthIteratorStart; blurBoxWidthIterator <= blurBoxMaxWidth; blurBoxWidthIterator++)
                {
                    blurBox[blurBoxHeightIterator][blurBoxWidthIterator] = image[heightIterator + blurBoxHeightIterator - 1][widthIterator + blurBoxWidthIterator - 1];
                }
            }

            u_int neighbourPixelCount = 9;

            if ((heightIterator == 0 || heightIterator == height - 1) && !(widthIterator == 0 || widthIterator == width - 1))
            {
                neighbourPixelCount = 6;
            }
            else if (!(heightIterator == 0 || heightIterator == height - 1) && (widthIterator == 0 || widthIterator == width - 1))
            {
                neighbourPixelCount = 6;
            }
            else if ((heightIterator == 0 || heightIterator == height - 1) && (widthIterator == 0 || widthIterator == width - 1))
            {
                neighbourPixelCount = 4;
            }

            RGBTRIPLE resultingColors = {0};
            u_int resultingBlueSum = 0, resultingGreenSum = 0, resultingRedSum = 0;

            for (u_int blurBoxHeightIterator = 0; blurBoxHeightIterator <= 2; blurBoxHeightIterator++)
            {
                for (u_int blurBoxWidthIterator = 0; blurBoxWidthIterator <= 2; blurBoxWidthIterator++)
                {
                    resultingBlueSum += blurBox[blurBoxHeightIterator][blurBoxWidthIterator].rgbtBlue;
                    resultingGreenSum += blurBox[blurBoxHeightIterator][blurBoxWidthIterator].rgbtGreen;
                    resultingRedSum += blurBox[blurBoxHeightIterator][blurBoxWidthIterator].rgbtRed;
                }
            }

            resultingColors.rgbtBlue = round(resultingBlueSum / (double) neighbourPixelCount);
            resultingColors.rgbtGreen = round(resultingGreenSum / (double) neighbourPixelCount);
            resultingColors.rgbtRed = round(resultingRedSum / (double) neighbourPixelCount);

            resultingImage[heightIterator][widthIterator] = resultingColors;
        }
    }

    for (u_int heightIterator = 0; heightIterator < height; heightIterator++)
    {
        for (u_int widthIterator = 0; widthIterator < width; widthIterator++)
        {
            image[heightIterator][widthIterator] = resultingImage[heightIterator][widthIterator];
        }
    }

    free(resultingImage);
    resultingImage = NULL;

    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE(*resultingImage)[width] = calloc(height, width * sizeof(RGBTRIPLE));
    if (image == NULL)
    {
        return;
    }

    for (u_int heightIterator = 0; heightIterator < height; heightIterator++)
    {
        for (u_int widthIterator = 0; widthIterator < width; widthIterator++)
        {
            RGBTRIPLE edgeBox[3][3] = {{0}};

            u_int edgeBoxHeightIteratorStart = 0;
            u_int edgeBoxMaxHeight = 2;

            u_int edgeBoxWidthIteratorStart = 0;
            u_int edgeBoxMaxWidth = 2;

            if (heightIterator == 0)
            {
                edgeBoxHeightIteratorStart = 1;
            }
            if (heightIterator == height - 1)
            {
                edgeBoxMaxHeight = 1;
            }
            if (widthIterator == 0)
            {
                edgeBoxWidthIteratorStart = 1;
            }
            if (widthIterator == width - 1)
            {
                edgeBoxMaxWidth = 1;
            }

            for (u_int edgeBoxHeightIterator = edgeBoxHeightIteratorStart; edgeBoxHeightIterator <= edgeBoxMaxHeight; edgeBoxHeightIterator++)
            {
                for (u_int edgeBoxWidthIterator = edgeBoxWidthIteratorStart; edgeBoxWidthIterator <= edgeBoxMaxWidth; edgeBoxWidthIterator++)
                {
                    edgeBox[edgeBoxHeightIterator][edgeBoxWidthIterator] = image[heightIterator + edgeBoxHeightIterator - 1][widthIterator + edgeBoxWidthIterator - 1];
                }
            }

            int xKernel[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
            int yKernel[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

            int resultingXBlueSum = 0, resultingXGreenSum = 0, resultingXRedSum = 0;
            int resultingYBlueSum = 0, resultingYGreenSum = 0, resultingYRedSum = 0;

            u_int resultingBlueColor = 0, resultingGreenColor = 0, resultingRedColor = 0;

            RGBTRIPLE resultingColors = {0};

            for (u_int edgeBoxHeightIterator = 0; edgeBoxHeightIterator <= 2; edgeBoxHeightIterator++)
            {
                for (u_int edgeBoxWidthIterator = 0; edgeBoxWidthIterator <= 2; edgeBoxWidthIterator++)
                {
                    resultingXBlueSum += xKernel[edgeBoxHeightIterator][edgeBoxWidthIterator] * edgeBox[edgeBoxHeightIterator][edgeBoxWidthIterator].rgbtBlue;
                    resultingXGreenSum += xKernel[edgeBoxHeightIterator][edgeBoxWidthIterator] * edgeBox[edgeBoxHeightIterator][edgeBoxWidthIterator].rgbtGreen;
                    resultingXRedSum += xKernel[edgeBoxHeightIterator][edgeBoxWidthIterator] * edgeBox[edgeBoxHeightIterator][edgeBoxWidthIterator].rgbtRed;

                    resultingYBlueSum += yKernel[edgeBoxHeightIterator][edgeBoxWidthIterator] * edgeBox[edgeBoxHeightIterator][edgeBoxWidthIterator].rgbtBlue;
                    resultingYGreenSum += yKernel[edgeBoxHeightIterator][edgeBoxWidthIterator] * edgeBox[edgeBoxHeightIterator][edgeBoxWidthIterator].rgbtGreen;
                    resultingYRedSum += yKernel[edgeBoxHeightIterator][edgeBoxWidthIterator] * edgeBox[edgeBoxHeightIterator][edgeBoxWidthIterator].rgbtRed;
                }
            }

            resultingBlueColor = round(sqrt(pow(resultingXBlueSum, 2) + pow(resultingYBlueSum, 2)));
            resultingGreenColor = round(sqrt(pow(resultingXGreenSum, 2) + pow(resultingYGreenSum, 2)));
            resultingRedColor = round(sqrt(pow(resultingXRedSum, 2) + pow(resultingYRedSum, 2)));

            if (resultingBlueColor > 255)
            {
                resultingColors.rgbtBlue = 255;
            }
            else
            {
                resultingColors.rgbtBlue = resultingBlueColor;
            }

            if (resultingGreenColor > 255)
            {
                resultingColors.rgbtGreen = 255;
            }
            else
            {
                resultingColors.rgbtGreen = resultingGreenColor;
            }

            if (resultingRedColor > 255)
            {
                resultingColors.rgbtRed = 255;
            }
            else
            {
                resultingColors.rgbtRed = resultingRedColor;
            }

            resultingImage[heightIterator][widthIterator] = resultingColors;
        }
    }

    for (u_int heightIterator = 0; heightIterator < height; heightIterator++)
    {
        for (u_int widthIterator = 0; widthIterator < width; widthIterator++)
        {
            image[heightIterator][widthIterator] = resultingImage[heightIterator][widthIterator];
        }
    }

    free(resultingImage);
    resultingImage = NULL;

    return;
}

#include <math.h>
#include <stdlib.h>

#include "helpers.h"

typedef unsigned int uint;
// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (uint heightIterator = 0; heightIterator < height; heightIterator++)
    {
        for (uint widthIterator = 0; widthIterator < width; widthIterator++)
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

    for (uint heightIterator = 0; heightIterator < height; heightIterator++)
    {
        for (uint widthIterator = 0; widthIterator < width; widthIterator++)
        {
            resultingLine[widthIterator] = image[heightIterator][width - widthIterator - 1];
        }

        for (uint widthIterator = 0; widthIterator < width; widthIterator++)
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

    for (uint heightIterator = 0; heightIterator < height; heightIterator++)
    {
        for (uint widthIterator = 0; widthIterator < width; widthIterator++)
        {
            RGBTRIPLE blurBox[3][3] = { {0} };

            uint blurBoxHeightIteratorStart = 0;
            uint blurBoxMaxHeight = 2;

            uint blurBoxWidthIteratorStart = 0;
            uint blurBoxMaxWidth = 2;

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

            for (uint blurBoxHeightIterator = blurBoxHeightIteratorStart; blurBoxHeightIterator <= blurBoxMaxHeight; blurBoxHeightIterator++)
            {
                for (uint blurBoxWidthIterator = blurBoxWidthIteratorStart; blurBoxWidthIterator <= blurBoxMaxWidth; blurBoxWidthIterator++)
                {
                    blurBox[blurBoxHeightIterator][blurBoxWidthIterator] = image[heightIterator + blurBoxHeightIterator - 1][widthIterator + blurBoxWidthIterator - 1];
                }
            }

            uint neighbourPixelCount = 9;

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

            RGBTRIPLE resultingColors = { 0 };
            uint resultingBlueSum = 0, resultingGreenSum = 0, resultingRedSum = 0;

            for (uint blurBoxHeightIterator = 0; blurBoxHeightIterator <= 2; blurBoxHeightIterator++)
            {
                for (uint blurBoxWidthIterator = 0; blurBoxWidthIterator <= 2; blurBoxWidthIterator++)
                {
                    resultingBlueSum += blurBox[blurBoxHeightIterator][blurBoxWidthIterator].rgbtBlue;
                    resultingGreenSum += blurBox[blurBoxHeightIterator][blurBoxWidthIterator].rgbtGreen;
                    resultingRedSum += blurBox[blurBoxHeightIterator][blurBoxWidthIterator].rgbtRed;
                }
            }

            resultingColors.rgbtBlue = round(resultingBlueSum / (double)neighbourPixelCount);
            resultingColors.rgbtGreen = round(resultingGreenSum / (double)neighbourPixelCount);
            resultingColors.rgbtRed = round(resultingRedSum / (double)neighbourPixelCount);

            resultingImage[heightIterator][widthIterator] = resultingColors;
        }
    }

    for (uint heightIterator = 0; heightIterator < height; heightIterator++)
    {
        for (uint widthIterator = 0; widthIterator < width; widthIterator++)
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

    for (uint heightIterator = 0; heightIterator < height; heightIterator++)
    {
        for (uint widthIterator = 0; widthIterator < width; widthIterator++)
        {
            RGBTRIPLE edgeBox[3][3] = { {0} };

            uint edgeBoxHeightIteratorStart = 0;
            uint edgeBoxMaxHeight = 2;

            uint edgeBoxWidthIteratorStart = 0;
            uint edgeBoxMaxWidth = 2;

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

            for (uint edgeBoxHeightIterator = edgeBoxHeightIteratorStart; edgeBoxHeightIterator <= edgeBoxMaxHeight; edgeBoxHeightIterator++)
            {
                for (uint edgeBoxWidthIterator = edgeBoxWidthIteratorStart; edgeBoxWidthIterator <= edgeBoxMaxWidth; edgeBoxWidthIterator++)
                {
                    edgeBox[edgeBoxHeightIterator][edgeBoxWidthIterator] = image[heightIterator + edgeBoxHeightIterator - 1][widthIterator + edgeBoxWidthIterator - 1];
                }
            }

            int xKernel[3][3] = { {-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1} };
            int yKernel[3][3] = { {-1, -2, -1}, {0, 0, 0}, {1, 2, 1} };

            int resultingXBlueSum = 0, resultingXGreenSum = 0, resultingXRedSum = 0;
            int resultingYBlueSum = 0, resultingYGreenSum = 0, resultingYRedSum = 0;

            uint resultingBlueColor = 0, resultingGreenColor = 0, resultingRedColor = 0;

            RGBTRIPLE resultingColors = { 0 };

            for (uint edgeBoxHeightIterator = 0; edgeBoxHeightIterator <= 2; edgeBoxHeightIterator++)
            {
                for (uint edgeBoxWidthIterator = 0; edgeBoxWidthIterator <= 2; edgeBoxWidthIterator++)
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

    for (uint heightIterator = 0; heightIterator < height; heightIterator++)
    {
        for (uint widthIterator = 0; widthIterator < width; widthIterator++)
        {
            image[heightIterator][widthIterator] = resultingImage[heightIterator][widthIterator];
        }
    }

    free(resultingImage);
    resultingImage = NULL;

    return;
}

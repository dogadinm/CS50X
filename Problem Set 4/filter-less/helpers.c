#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int row = 0; row < height; row++)
    {
        for (int colume = 0; colume < width; colume++)
        {
            float red = image[row][colume].rgbtRed;
            float green = image[row][colume].rgbtGreen;
            float blue = image[row][colume].rgbtBlue;

            int rgb_average = round((red + green + blue) / 3);
            image[row][colume].rgbtRed = image[row][colume].rgbtGreen = image[row][colume].rgbtBlue = rgb_average;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int row = 0; row < height; row++)
    {
        for (int colume = 0; colume < width; colume++)
        {

            int sepiaRed = round(0.393 * image[row][colume].rgbtRed + 0.769 * image[row][colume].rgbtGreen + 0.189 *
                                 image[row][colume].rgbtBlue);
            int sepiaGreen  = round(0.349 * image[row][colume].rgbtRed + 0.686 * image[row][colume].rgbtGreen + 0.168 *
                                    image[row][colume].rgbtBlue);
            int sepiaBlue  = round(0.272 * image[row][colume].rgbtRed + 0.534 * image[row][colume].rgbtGreen + 0.131 *
                                   image[row][colume].rgbtBlue);

            if (sepiaRed > 255)
            {
                sepiaRed = 255;
            }

            if (sepiaGreen  > 255)
            {
                sepiaGreen  = 255;
            }

            if (sepiaBlue > 255)
            {
                sepiaBlue = 255;
            }

            image[row][colume].rgbtRed =  sepiaRed ;
            image[row][colume].rgbtGreen = sepiaGreen ;
            image[row][colume].rgbtBlue = sepiaBlue;
        }
    }
    return;
}
// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int row = 0; row < height; row++)
    {
        for (int colume = 0; colume < width / 2; colume++)
        {
            RGBTRIPLE temp = image[row][colume];
            image[row][colume] = image[row][width - (colume + 1)];
            image[row][width - (colume + 1)] = temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp[height][width];

    for (int row = 0; row < height; row++)
    {
        for (int colume = 0; colume < width; colume++)
        {

            float sumb = 0, sumg = 0, sumr = 0, counter = 0;

            for (int R = -1; R < 2; R++)
            {
                for (int C = -1; C < 2; C++)
                {
                    if ((row + R < 0 || row + R > height - 1) || (colume + C < 0 || colume + C > width - 1))
                    {
                        continue;
                    }
                    sumr += image[row + R][colume + C].rgbtRed;
                    sumg += image[row + R][colume + C].rgbtGreen;
                    sumb += image[row + R][colume + C].rgbtBlue;
                    counter++;
                }
                temp[row][colume].rgbtRed = round(sumr / counter);
                temp[row][colume].rgbtGreen = round(sumg / counter);
                temp[row][colume].rgbtBlue = round(sumb / counter);
            }
        }
    }

    for (int row = 0; row < height; row++)
    {
        for (int colume = 0; colume < width; colume++)
        {
            image[row][colume].rgbtRed = temp[row][colume].rgbtRed;
            image[row][colume].rgbtGreen = temp[row][colume].rgbtGreen;
            image[row][colume].rgbtBlue = temp[row][colume].rgbtBlue;
        }
    }
    return;
}



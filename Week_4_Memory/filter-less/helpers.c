#include "helpers.h"
#include <math.h>


int SIZE_NEIGHBORS = 9;

void clip(float *x);
void swap(RGBTRIPLE *x, RGBTRIPLE *y);
RGBTRIPLE box_average(RGBTRIPLE n[]);
// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // take the average of each pixel
            float average = (image[i][j].rgbtBlue + image[i][j].rgbtRed + image[i][j].rgbtGreen) / 3.0;
            uint8_t a = (uint8_t) round(average);
            // update the pixel with new rgb values
            image[i][j].rgbtRed = a;
            image[i][j].rgbtGreen = a;
            image[i][j].rgbtBlue = a;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    // iterate over every pixel
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // calculate the new values for red, green, and blue
            BYTE originalRed = image[i][j].rgbtRed;
            BYTE originalGreen = image[i][j].rgbtGreen;
            BYTE originalBlue = image[i][j].rgbtBlue;

            float sepiaRed = .393 * originalRed + .769 * originalGreen + .189 * originalBlue;
            float sepiaGreen = .349 * originalRed + .686 * originalGreen + .168 * originalBlue;
            float sepiaBlue = .272 * originalRed + .534 * originalGreen + .131 * originalBlue;

            sepiaRed = round(sepiaRed);
            sepiaGreen = round(sepiaGreen);
            sepiaBlue = round(sepiaBlue);
            // make sure all sepia color values are below 255
            clip(&sepiaRed);
            clip(&sepiaGreen);
            clip(&sepiaBlue);

            // update the pixel with new rgb values
            image[i][j].rgbtRed = (uint8_t) sepiaRed;
            image[i][j].rgbtGreen = (uint8_t) sepiaGreen;
            image[i][j].rgbtBlue = (uint8_t) sepiaBlue;
        }
    }
        
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // loop over all rows
    for (int i = 0; i < height; i++)
    {
        // in a row of length width
        // iterate j over pixels 0 - width/2
        for (int j = 0; j < width / 2; j++)
        {
            // swap pixel j with pixel width - j
            RGBTRIPLE *a = &image[i][j];
            RGBTRIPLE *b = &image[i][width - j - 1];

            swap(a, b);
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // create a copy of the image where we will store the pixel values as we blur them 
    RGBTRIPLE copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }

    // iterate through every pixel in original
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float averageRed = 0.0;
            float averageGreen = 0.0;
            float averageBlue = 0.0;
            float neighbors_count = 0;

            // loop through all neighbors
            for (int k = -1; k < 2; k++)
            {
                for (int l = -1; l < 2; l++)
                {
                    if (k + i >= 0 && k + i < height)
                    {
                        if (l + j >= 0 && l + j < width)
                        {
                            // neighbors[k+1][l+1] = 1;
                            averageRed += image[k + i][l + j].rgbtRed;
                            averageGreen += image[k + i][l + j].rgbtGreen;
                            averageBlue += image[k + i][l + j].rgbtBlue;
                            neighbors_count++;
                        }
                    }
                }
            }

            averageRed /= neighbors_count;
            averageGreen /= neighbors_count;
            averageBlue /= neighbors_count;

            averageRed = round(averageRed);
            averageGreen = round(averageGreen);
            averageBlue = round(averageBlue);

            copy[i][j].rgbtRed = averageRed;
            copy[i][j].rgbtGreen = averageGreen;
            copy[i][j].rgbtBlue = averageBlue;  
        }
    }

    // read the pixels in copy back to the original
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = copy[i][j];
        }
    }
    return;
}

void clip(float *x)
{
    // takes a pointer to a float as input and clips the value to 255
    // so it can be converted to uint8_t
    if (*x > 255)
    {
        *x = 255;
    }
}

void swap(RGBTRIPLE *x, RGBTRIPLE *y)
{
    // takes two pointers to a RGBTRIPLES and swaps their values
    RGBTRIPLE temp = *x;
    *x = *y;
    *y = temp;
}


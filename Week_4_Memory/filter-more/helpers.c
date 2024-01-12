#include "helpers.h"
#include <math.h>

// sobel kernels
int GX[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
int GY[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

void swap(RGBTRIPLE *x, RGBTRIPLE *y);
void clip(float *x);
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

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // create a copy of the image where we will store pixels as we modify them
    RGBTRIPLE copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }

    // loop through every pixel
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float gyRed = 0;
            float gyGreen = 0;
            float gyBlue = 0;

            float gxRed = 0;
            float gxGreen = 0;
            float gxBlue = 0;
            // loop through neighbors
            for (int k = -1; k < 2; k++)
            {
                for (int l = -1; l < 2; l++)
                {
                    if (k + i >= 0 && k + i < height)
                    {
                        if (l + j >= 0 && l + j < width)
                        {
                            // for each valid neighbor
                            
                            // find gy
                            gyRed += image[k + i][l + j].rgbtRed * GY[k + 1][l + 1];
                            gyGreen += image[k + i][l + j].rgbtGreen * GY[k + 1][l + 1];
                            gyBlue += image[k + i][l + j].rgbtBlue * GY[k + 1][l + 1];

                            // find gx
                            gxRed += image[k + i][l + j].rgbtRed * GX[k + 1][l + 1];
                            gxGreen += image[k + i][l + j].rgbtGreen * GX[k + 1][l + 1];
                            gxBlue += image[k + i][l + j].rgbtBlue * GX[k + 1][l + 1];
                        }
                    }
                }
            }
            // combine gx and gy
            float red = pow(gxRed, 2) + pow(gyRed, 2);
            red = round(sqrt(red));
            clip(&red);
            float green = pow(gxGreen, 2) + pow(gyGreen, 2);
            green = round(sqrt(green));
            clip(&green);
            float blue = pow(gxBlue, 2) + pow(gyBlue, 2);
            blue = round(sqrt(blue));
            clip(&blue);

            copy[i][j].rgbtRed = (BYTE) red;
            copy[i][j].rgbtGreen = (BYTE) green;
            copy[i][j].rgbtBlue = (BYTE) blue;  
        }
    }

    // load pixels from copy into original
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = copy[i][j];
        }
    }
    return;
}

void swap(RGBTRIPLE *x, RGBTRIPLE *y)
{
    // takes two pointers to a RGBTRIPLES and swaps their values
    RGBTRIPLE temp = *x;
    *x = *y;
    *y = temp;
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
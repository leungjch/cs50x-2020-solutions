#include "helpers.h"
#include <math.h>
#include <stdio.h>
// Convert image to grayscale


typedef struct
{
    double  rgbtBlue;
    double  rgbtGreen;
    double  rgbtRed;
} __attribute__((__packed__))
RGBTRIPLE_float;

void sumColors(RGBTRIPLE pixel, int *redSum, int *greenSum, int *blueSum);
void applySobel(RGBTRIPLE pixel, int Gx_single, int Gy_single, RGBTRIPLE_float *gxsum, RGBTRIPLE_float *gysum);
int cap255(double value);
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // loop by height
    for (int i = 0; i < height; i++)
    {
        for (int j = 0 ; j < width; j++)
        {
            // grayscale is the average of the rgb values

            double r = (double) image[i][j].rgbtRed;
            double g = (double) image[i][j].rgbtGreen;
            double b = (double) image[i][j].rgbtBlue;

            int average = round((r + g + b) / 3.0);

            image[i][j].rgbtRed = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtBlue = average;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // loop by height
    for (int i = 0; i < height; i++)
    {
        for (int j = 0 ; j < width / 2; j++)
        {
            RGBTRIPLE temp = image[i][width - j - 1]; // get pixel
            image[i][width - j - 1] = image[i][j];
            image[i][j] = temp;
        }
    }
    return;

}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{


    // printf("Height:%i\nWidth:%i",height,width);
    // loop by height

    RGBTRIPLE copy[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0 ; j < width; j++)
        {
            int sumR = 0;
            int sumG = 0;
            int sumB = 0;

            int n = 0;

            double average;

            // get top left if possible
            if ((j - 1) >= 0 && (i - 1) >= 0)
            {
                sumColors(image[i - 1][j - 1], &sumR, &sumG, &sumB);
                n += 1;
            }

            // get top middle (pixel above) if possible
            if (i - 1 >= 0)
            {
                sumColors(image[i - 1][j], &sumR, &sumG, &sumB);
                n += 1;
            }

            // get top right if possible
            if ((j + 1) < width && (i - 1) >= 0)
            {
                sumColors(image[i - 1][j + 1], &sumR, &sumG, &sumB);
                n += 1;
            }
            // get middle left if possible
            if (j - 1 >= 0)
            {
                sumColors(image[i][j - 1], &sumR, &sumG, &sumB);
                n += 1;
            }

            // get middle pixel (this pixel)
            sumColors(image[i][j], &sumR, &sumG, &sumB);
            n += 1;

            // get middle right if possible
            if (j + 1 < (width))
            {
                sumColors(image[i][j + 1], &sumR, &sumG, &sumB);
                n += 1;
            }
            // get lower left if possible
            if ((j - 1) >= 0 && (i + 1) < (height))
            {
                sumColors(image[i + 1][j - 1], &sumR, &sumG, &sumB);
                n += 1;
            }
            // get lower middle if possible
            if (i < (height - 1))
            {
                sumColors(image[i + 1][j], &sumR, &sumG, &sumB);
                n += 1;
            }
            // get lower right if possible
            if ((j + 1) < (width) && (i + 1) < (height))
            {
                sumColors(image[i + 1][j + 1], &sumR, &sumG, &sumB);
                n += 1;
            }

            int averageRed =  round(((double)(sumR)) / ((double) n));
            int averageGreen = round(((double)(sumG)) / ((double) n));
            int averageBlue = round(((double)(sumB)) / ((double)n));

            copy[i][j].rgbtRed = averageRed;
            copy[i][j].rgbtGreen = averageGreen;
            copy[i][j].rgbtBlue = averageBlue;

            printf("%i ", averageBlue);

            // compute average
        }
        printf("\n");
    }

    for (int i = 0; i < height; i ++)
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
    // printf("Height:%i\nWidth:%i",height,width);
    // loop by height
    RGBTRIPLE copy[height][width];


    // define kernels
    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
    for (int i = 0; i < height; i++)
    {
        for (int j = 0 ; j < width; j++)
        {
            int n = 0;


            RGBTRIPLE_float Gx_Sum = {0.0, 0.0, 0.0};
            RGBTRIPLE_float Gy_Sum = {0.0, 0.0, 0.0};

            // start processing kernels

            // get top left if possible
            if (i > 0 && j > 0)
            {
                int green = image[i - 1][j - 1].rgbtGreen;
                applySobel(image[i - 1][j - 1], Gx[0][0], Gy[0][0], &Gx_Sum, &Gy_Sum);
                n += 1;
            }

            // get top middle (pixel above) if possible
            if (i > 0)
            {
                applySobel(image[i - 1][j], Gx[0][1], Gy[0][1], &Gx_Sum, &Gy_Sum);
                // printf("%i, %i \n", Gx[0][1], Gy[0][1]);
                n += 1;
            }
            // get top right if possible
            if (j < width - 1 && i > 0)
            {
                applySobel(image[i - 1][j + 1], Gx[0][2], Gy[0][2], &Gx_Sum, &Gy_Sum);
                n += 1;
            }

            // get middle left if possible
            if (j > 0)
            {
                applySobel(image[i][j - 1], Gx[1][0], Gy[1][0], &Gx_Sum, &Gy_Sum);
                n += 1;
            }

            // get middle pixel (this pixel)
            applySobel(image[i][j], Gx[1][1], Gy[1][1], &Gx_Sum, &Gy_Sum);
            n += 1;

            // get middle right if possible
            if (j < width - 1)
            {
                applySobel(image[i][j + 1], Gx[1][2], Gy[1][2], &Gx_Sum, &Gy_Sum);
                n += 1;
            }

            // get lower left if possible
            if (i < height - 1 && j > 0)
            {
                applySobel(image[i + 1][j - 1], Gx[2][0], Gy[2][0], &Gx_Sum, &Gy_Sum);
                n += 1;
            }

            // get lower middle if possible
            if (i < height - 1)
            {
                applySobel(image[i + 1][j], Gx[2][1], Gy[2][1], &Gx_Sum, &Gy_Sum);
                n += 1;
            }

            // get lower right if possible
            if (i < height - 1 && j < width - 1)
            {
                applySobel(image[i + 1][j + 1], Gx[2][2], Gy[2][2], &Gx_Sum, &Gy_Sum);
                n += 1;
            }

            // get magnitude of Gx and Gy
            int r = cap255(round(sqrt(pow(Gx_Sum.rgbtRed, 2) + pow(Gy_Sum.rgbtRed, 2))));
            int g = cap255(round(sqrt(pow(Gx_Sum.rgbtGreen, 2) + pow(Gy_Sum.rgbtGreen, 2))));
            int b = cap255(round(sqrt(pow(Gx_Sum.rgbtBlue, 2) + pow(Gy_Sum.rgbtBlue, 2))));

            copy[i][j].rgbtRed = r;
            copy[i][j].rgbtGreen = g;
            copy[i][j].rgbtBlue = b;

        }
    }
    for (int i = 0; i < height; i ++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = copy[i][j];
        }
    }

    return;
}

// take a pixel's RGB values and sum it
void sumColors(RGBTRIPLE pixel, int *redSum, int *greenSum, int *blueSum)
{
    *redSum += pixel.rgbtRed;
    *greenSum += pixel.rgbtGreen;
    *blueSum += pixel.rgbtBlue;

    printf("Red: %i, Green: %i, Blue: %i\n", *redSum, *greenSum, *blueSum);
}

// take a pixel's RGB values and apply a weighted sum according to the provided value of Gx and Gy
void applySobel(RGBTRIPLE pixel, int Gx_single, int Gy_single, RGBTRIPLE_float *gxsum, RGBTRIPLE_float *gysum)
{

    (*gxsum).rgbtRed += pixel.rgbtRed * Gx_single;
    (*gxsum).rgbtGreen += pixel.rgbtGreen * Gx_single;
    (*gxsum).rgbtBlue += pixel.rgbtBlue * Gx_single;

    (*gysum).rgbtRed += pixel.rgbtRed * Gy_single;
    (*gysum).rgbtGreen += pixel.rgbtGreen * Gy_single;
    (*gysum).rgbtBlue += pixel.rgbtBlue * Gy_single;

    // printf("Red: %i, Green: %i, Blue: %i\n", *redSum, *greenSum, *blueSum);
}

// void capValue(double *value)
// {
//     if (*value > 255)
//     {
//         *value = 255;
//     }
//     else if (*value < 0)
//     {
//         *value = 0;
//     }
//     else
//     {
//         *value = (int) round(*value);
//     }
// }

int cap255(double value)
{
    if (value > 255)
    {
        return 255;
    }
    else if (value < 0)
    {
        return 0;
    }
    else
    {
        return (int) value;
    }
}
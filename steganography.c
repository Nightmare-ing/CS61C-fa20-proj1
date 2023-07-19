/************************************************************************
**
** NAME:        steganography.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**				Justin Yokota - Starter Code
**				YOUR NAME HERE
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

//Determines what color the cell at the given row/col should be. This should not affect Image, and should allocate space for a new Color.
Color *evaluateOnePixel(Image *image, int row, int col)
{
    Color *result_color = (Color *) malloc(sizeof(Color));
    if (result_color == NULL) exit(-1);

	uint8_t B_value = image->image[row][col].B;
    if (B_value & 1) {
        result_color->R = 0;
        result_color->G = 0;
        result_color->B = 0;
    } else {
        result_color->R = 255;
        result_color->G = 255;
        result_color->B = 255;
    }
    return result_color;
}

//Given an image, creates a new image extracting the LSB of the B channel.
Image *steganography(Image *image)
{
    Image *decoded_image = (Image *) malloc(sizeof(Image));
    if (decoded_image == NULL) exit(-1);

    Color *decoded_pixel = NULL;
    uint32_t i = 0, j = 0, rows = image->rows, cols = image->cols;
    decoded_image->image = (Color **) malloc(sizeof(Color *) * rows);
    if (decoded_image->image == NULL) exit(-1);

    decoded_image->rows = image->rows;
    decoded_image->cols = image->cols;

    for (i = 0; i < image->rows; ++i) {
        Color *pixel_row = (Color *) malloc(sizeof(Color) * cols);
        if (pixel_row == NULL) exit(-1);

        for (j = 0; j < image->cols; ++j) {
            decoded_pixel = evaluateOnePixel(image, i, j);
            pixel_row[j] = *decoded_pixel;
            free(decoded_pixel);
        }
        decoded_image->image[i] = pixel_row;
    }
    return decoded_image;
}

/*
Loads a file of ppm P3 format from a file, and prints to stdout (e.g. with printf) a new image, 
where each pixel is black if the LSB of the B channel is 0, 
and white if the LSB of the B channel is 1.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a file of ppm P3 format (not necessarily with .ppm file extension).
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!
*/
int main(int argc, char **argv)
{
    Image *image = NULL;
    Image *decoded_image = NULL;
    char *filename;

    if (argc != 2) {
        printf("usage: %s filename\n", argv[0]);
        printf("filename is an ASCII PPM file (type P3) with maximuum value 255.\n");
        exit(-1);
    }
    filename = argv[1];

    image = readData(filename);
    decoded_image = steganography(image);
    writeData(decoded_image);
    freeImage(image);
    freeImage(decoded_image);

    return 0;
}

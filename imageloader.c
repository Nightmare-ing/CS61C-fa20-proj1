/************************************************************************
**
** NAME:        imageloader.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**              Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-15
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "imageloader.h"

//Opens a .ppm P3 image file, and constructs an Image object. 
//You may find the function fscanf useful.
//Make sure that you close the file with fclose before returning.
Image *readData(char *filename) 
{
	Image *image_st = (Image *) malloc(sizeof(Image));
    FILE *fp = fopen(filename, "r");
    char identifier[20];
    uint32_t cols = 0, rows = 0;
    uint32_t index_col = 0, index_row = 0;
    int R, G, B;
    int max_value;
    Color *pixel = NULL;

    fscanf(fp, "%s %d %d %d", identifier, &cols, &rows, &max_value);
    
    image_st->cols = cols;
    image_st->rows = rows;
    image_st->image = (Color **) malloc(sizeof(Color *) * rows);

    for (index_row = 0; index_row < rows; ++index_row) {
        Color *row = (Color *) malloc(sizeof(Color) * cols);
        for (index_col = 0; index_col < cols; ++index_col) {
            pixel = &row[index_col];
            fscanf(fp, "%d %d %d", &R, &G, &B);
            pixel->R = (uint8_t) R;
            pixel->G = (uint8_t) G;
            pixel->B = (uint8_t) B;
        }
        image_st->image[index_row] = row;
    }

    fclose(fp);
    return image_st;
}

//Given an image, prints to stdout (e.g. with printf) a .ppm P3 file with the image's data.
void writeData(Image *image)
{
    Color *pixel = NULL;
	printf("P3\n%d %d\n255\n", image->cols, image->rows);
    uint32_t i = 0, j = 0;

    for (i = 0; i < image->rows; ++i) {
        for (j = 0; j < image->cols - 1; ++j) {
            pixel = &(image->image[i][j]);
            printf("%3d %3d %3d   ", (int)pixel->R, (int)pixel->G, (int)pixel->B);
        }
        pixel = &(image->image[i][j]);
        printf("%3d %3d %3d\n", (int)pixel->R, (int)pixel->G, (int)pixel->B);
    }
}

//Frees an image
void freeImage(Image *image)
{
    uint32_t row = 0;
    for (row = 0; row < image->rows; ++row) {
        free(image->image[row]);
    }
    free(image->image);
    free(image);
}

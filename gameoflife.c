/************************************************************************
**
** NAME:        gameoflife.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <math.h>
#include <stdbool.h>
#include "imageloader.h"
//#include "test_gameoflife.h" // for custom test sets to verify the
//correctness of the code

#define EIGHT_BIT_MAX (255)
#define EIGHT_BIT_MAX_POW (256)


/* extract the bit at the bit_plane */
int take_plane_bit(Image *image, int row, int col, size_t bit_plane) {
    if (bit_plane < 8) {
        return image->image[row][col].B >> bit_plane & 1;
    } else if (bit_plane < 16) {
        return image->image[row][col].G >> (bit_plane - 8) & 1;
    } else {
        return image->image[row][col].R >> (bit_plane - 16) & 1;
    }
}

/* get the number of the alive neighbors at BIT_PLANE */
size_t get_alive_neighbors(Image *image, int row, int col, size_t bit_plane) {
    size_t alive_neighbors = 0;
    int i, j, i_copy, j_copy;
    for (i = row - 1; i < row + 2; ++i) {
        i_copy = (i + image->rows) % image->rows; /* correct i to fit the first line */
        for (j = col - 1; j < col + 2; ++j) {
            j_copy = (j + image->cols) % image->cols;
            if (i == row && j == col) continue;
            alive_neighbors += take_plane_bit(image, i_copy, j_copy, bit_plane);
        }
    }
    return alive_neighbors;
}

/* extract infomation of the alive situations from the rule code */
void alive_dead_situations(uint32_t rule, size_t *alive_neigh_array, size_t *dead_neigh_array,
        size_t *alive_array_size, size_t *dead_array_size) {
    size_t order = 0;
    *alive_array_size = 0;
    *dead_array_size = 0;

    /* construct dead situation array */
    for (order = 0; order < 9; ++order) {
        if (rule & ((uint32_t) pow(2, order))) {
            dead_neigh_array[*dead_array_size] = order;
            ++*dead_array_size;
        }
    }

    /* construct alive situation array */
    for (order = 9; order <18; ++order) {
        if (rule & ((uint32_t) pow(2, order))) {
            alive_neigh_array[*alive_array_size] = order - 9;
            ++*alive_array_size;
        }
    }
}

/* determine the pixel is alive or not at BIT_PLANE */
bool is_alive(Image *image, int row, int col, size_t bit_plane) {
    return take_plane_bit(image, row, col, bit_plane);
}

/* determine whether the ELEM is in ARRAY which has size SIZE */
bool in_array(size_t array[], size_t size, size_t elem) {
    size_t i = 0;
    for (i = 0; i < size; ++i) {
        if (array[i] == elem) return true;
    }
    return false;
}

/* convert 32bit RGB code into a Color object */
Color *convert_RGBcode(uint32_t RGB_code) {
    Color *result = (Color *) malloc(sizeof(Color));
    if (result == NULL) exit(-1);
    result->B = RGB_code & EIGHT_BIT_MAX;
    result->G = (RGB_code / EIGHT_BIT_MAX_POW) & EIGHT_BIT_MAX;
    result->R = (RGB_code / EIGHT_BIT_MAX_POW / EIGHT_BIT_MAX_POW) & EIGHT_BIT_MAX;
    return result;
}

//Determines what color the cell at the given row/col should be. This function allocates space for a new Color.
//Note that you will need to read the eight neighbors of the cell in question. The grid "wraps", so we treat the top row as adjacent to the bottom row
//and the left column as adjacent to the right column.
Color *evaluateOneCell(Image *image, int row, int col, uint32_t rule) {
    size_t neigh_num_alive[9], neigh_num_dead[9], alive_array_size, dead_array_size;
    size_t bit_plane = 0, alive_neighbors;
    uint32_t RGB_code = 0;
    alive_dead_situations(rule, neigh_num_alive, neigh_num_dead, &alive_array_size, &dead_array_size);

    for (bit_plane = 0; bit_plane < 24; ++bit_plane) {
        alive_neighbors = get_alive_neighbors(image, row, col, bit_plane);
        if (is_alive(image, row, col, bit_plane) && in_array(neigh_num_alive, alive_array_size, alive_neighbors)) {
            RGB_code += pow(2, bit_plane);
        }
        if (!is_alive(image, row, col, bit_plane) && in_array(neigh_num_dead, dead_array_size, alive_neighbors)) {
            RGB_code += pow(2, bit_plane);
        }
    }

    return convert_RGBcode(RGB_code);
}

//The main body of Life; given an image and a rule, computes one iteration of the Game of Life.
//You should be able to copy most of this from steganography.c
Image *life(Image *image, uint32_t rule)
{
    Image *convert_image = (Image *) malloc(sizeof(Image));
    if (convert_image == NULL) exit(-1);

    Color *convert_pixel = NULL;
    Color *pixel_row = NULL;
    uint32_t i = 0, j = 0, rows = image->rows, cols = image->cols;
    convert_image->image = (Color **) malloc(sizeof(Color *) * rows);
    if (convert_image->image == NULL) exit(-1);

    convert_image->rows = rows;
    convert_image->cols = cols;

    for (i = 0; i < image->rows; ++i) {
        pixel_row = (Color *) malloc(sizeof(Color) * cols);
        if (pixel_row == NULL) exit(-1);

        for (j = 0; j < image->cols; ++j) {
            convert_pixel = evaluateOneCell(image, i, j, rule);
            pixel_row[j] = *convert_pixel;
            free(convert_pixel);
        }
        convert_image->image[i] = pixel_row;
    }

    return convert_image;
}

/*
Loads a .ppm from a file, computes the next iteration of the game of life, then prints to stdout the new image.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a .ppm.
argv[2] should contain a hexadecimal number (such as 0x1808). Note that this will be a string.
You may find the function strtol useful for this conversion.
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!

You may find it useful to copy the code from steganography.c, to start.
*/
int main(int argc, char **argv)
{
    /*
    test_get_alive_neighbors();
    test_alive_dead_situations();
    test_is_alive();
    test_in_array();
    test_convert_RGBcode();
    test_evaluateOneCell();
     */

    char *filename = argv[1];
    Image *image = NULL;
    Image *convert_image = NULL;
    char *end_error_ptr = NULL;
    uint32_t rule = strtoul(argv[2], &end_error_ptr, 16);

    if (argc != 3) {
        printf("usage: %s filename %s\n", argv[0], argv[2]);
        printf("filename is an ASCII PPM file (type P3) with maximum value 255.\n");
        printf("rule is a hex number beginning with 0x; Life is 0x1808.\n");
        exit(-1);
    }

    image = readData(filename);
    convert_image = life(image, rule);
    writeData(convert_image);
    freeImage(image);
    freeImage(convert_image);
}

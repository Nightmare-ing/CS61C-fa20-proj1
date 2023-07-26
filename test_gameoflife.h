#ifndef TEST_GAMEOFLIFE_H
#define TEST_GAMEOFLIFE_H

#include <stdio.h>
#include "imageloader.h"
#include <stdbool.h>

extern size_t get_alive_neighbors(Image *image, int row, int col, size_t bit_plane);
extern void alive_dead_situations(uint32_t rule, size_t *alive_neigh_array, size_t *dead_neigh_array, size_t *alive_array_size, size_t *dead_array_size);
extern bool is_alive(Image *image, int row, int col, size_t bit_plane);
extern bool in_array(size_t array[], size_t size, size_t elem);
extern Color *convert_RGBcode(uint32_t RGB_code);
extern Color *evaluateOneCell(Image *image, int row, int col, uint32_t rule);

static char *filename1 = "testInputs/blinkerH.ppm";
static char *filename2 = "testInputs/blinkerH_modified.ppm";
static uint32_t rule1 = 0x1808;
static uint32_t rule2 = 0x154AA;

void test_get_alive_neighbors();
void test_alive_dead_situations();
void test_is_alive();
void test_in_array();
void test_convert_RGBcode();
void test_evaluateOneCell();

#endif

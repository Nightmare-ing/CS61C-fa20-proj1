#include "test_gameoflife.h"

void test_get_alive_neighbors() {
    Image *image1 = readData(filename1);
    Image *image2 = readData(filename2);

    /* test for edge rows, cols handling */
    if (get_alive_neighbors(image1, 0, 0, 0) != 2) {
        printf("get_alive_neighbors function error0\n");
    }
    if (get_alive_neighbors(image1, 0, 1, 0) != 3) {
        printf("get_alive_neighbors function error1\n");
    }
    if (get_alive_neighbors(image1, 0, 2, 0) != 2) {
        printf("get_alive_neighbors function error2\n");
    }

    /* test for specific color channel */
    if (get_alive_neighbors(image2, 0, 0, 0) != 1) {
        printf("RGB channel handle error0\n");
    }
    if (get_alive_neighbors(image2, 0, 0, 8) != 2) {
        printf("RGB channel handle error1\n");
    }
    if (get_alive_neighbors(image2, 0, 0, 16) != 2) {
        printf("RGB channel handle error1\n");
    }

    freeImage(image1);
    freeImage(image2);
}

void test_alive_dead_situations() {
    size_t alive_neigh_array[8], dead_neigh_array[8], alive_array_size, dead_array_size;

    /* test for rule 0x1808 */
    alive_dead_situations(rule1, alive_neigh_array, dead_neigh_array, &alive_array_size, &dead_array_size);
    if (alive_array_size != 2) printf("number of alive neighbor situations error for rule %x\n", rule1);
    if (dead_array_size != 1) printf("number of dead neighbor situations error for rule %x\n", rule1);
    if (alive_neigh_array[0] != 2 || alive_neigh_array[1] != 3) printf("alive neighbor situations error for rule %x\n", rule1);
    if (dead_neigh_array[0] != 3) printf("dead neighbor situations error for rule %x\n", rule1);
    
    /* test for rule 0x154AA */
    alive_dead_situations(rule2, alive_neigh_array, dead_neigh_array, &alive_array_size, &dead_array_size);
    if (alive_array_size != 4) printf("number of alive neighbor situations error for rule %x\n", rule2);
    if (dead_array_size != 4) printf("number of dead neighbor situations error for rule %x\n", rule2);
    if (alive_neigh_array[0] != 1 || alive_neigh_array[1] != 3 || alive_neigh_array[2] != 5 || alive_neigh_array[3] != 7) printf("alive neighbor situations error for rule %x\n", rule2);
    if (alive_neigh_array[0] != 1 || alive_neigh_array[1] != 3 || alive_neigh_array[2] != 5 || alive_neigh_array[3] != 7) printf("dead neighbor situations error for rule %x\n", rule2);

}

void test_is_alive() {
    Image *image1 = readData(filename1);
    Image *image2 = readData(filename2);

    if (is_alive(image1, 0, 0, 10) != false) printf("is_alive function error1");
    if (is_alive(image1, 1, 0, 10) != true) printf("is_alive function error2");
    if (is_alive(image2, 1, 0, 6) != false) printf("is_alive function error3");
    if (is_alive(image2, 1, 0, 10) != true) printf("is_alive function error4");


    freeImage(image1);
    freeImage(image2);
}

void test_in_array() {
    size_t array[5] = {1, 2, 3, 4, 5};
    if (in_array(array, 5, 3) != true) printf("in_array function error1");
    if (in_array(array, 5, 10) != false) printf("in_array function error2");
}

void test_convert_RGBcode() {
    uint32_t R_color = 16711680, G_color = 65280, B_color = 255;
    Color *R, *G, *B;

    R = convert_RGBcode(R_color);
    G = convert_RGBcode(G_color);
    B = convert_RGBcode(B_color);

    if (R->R != 255 || R->G != 0 || R->B != 0) printf("convert_RGBcode function error1\n");
    if (G->R != 0 || G->G != 255 || G->B != 0) printf("convert_RGBcode function error2\n");
    if (B->R != 0 || B->G != 0 || B->B != 255) printf("convert_RGBcode function error3\n");
}

void test_evaluateOneCell() {
    Image *image1 = readData("testInputs/blinkerH.ppm");
    Image *image2 = readData("testInputs/blinkerV.ppm");

    Color *color_0_1 = evaluateOneCell(image1, 0, 1, rule1);
    Color *color_1_1 = evaluateOneCell(image1, 1, 1, rule1);
    Color *color_2_1 = evaluateOneCell(image1, 2, 1, rule1);

    Color *target_color_0_1 = &image2->image[0][1];
    Color *target_color_1_1 = &image2->image[1][1];
    Color *target_color_2_1 = &image2->image[2][1];
    if (color_0_1->R != target_color_0_1->R || color_0_1->G != target_color_0_1->G || color_0_1->B != target_color_0_1->B) {
        printf("evaluateOneCell function error1\n");
    }
    if (color_1_1->R != target_color_1_1->R || color_1_1->G != target_color_1_1->G || color_1_1->B != target_color_1_1->B) {
        printf("evaluateOneCell function error2\n");
    }
    if (color_0_1->R != target_color_0_1->R || color_0_1->G != target_color_0_1->G || color_0_1->B != target_color_0_1->B) {
        printf("evaluateOneCell function error3\n");
    }

}

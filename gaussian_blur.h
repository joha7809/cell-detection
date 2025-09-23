#include "cbmp.h"

void gaussian_blur(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH],
                   unsigned char output_image[BMP_WIDTH][BMP_HEIGTH]);

void fill_holes(unsigned char image[BMP_WIDTH][BMP_HEIGTH]);

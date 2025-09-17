#ifndef COUNT_H
#define COUNT_H

#include "cbmp.h"
#include "otsu.h"

int cellCounter(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH]);

int erode(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH],
          unsigned char output_image[BMP_WIDTH][BMP_HEIGTH]);

#endif

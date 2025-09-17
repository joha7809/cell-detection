#ifndef OTSU_H
#define OTSU_H

#include "cbmp.h"

void otsu(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH],
          unsigned char output_image[BMP_WIDTH][BMP_HEIGTH]);

void grayscale(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS],
               unsigned char output_image[BMP_WIDTH][BMP_HEIGTH]);

#endif

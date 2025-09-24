#ifndef OTSU_H
#define OTSU_H

#include <stdio.h>
#include "cbmp.h"
#include "pixelarray.h"

void otsu(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH],
        u_int8_t* grid);

void threshold(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH],
               unsigned char output_image[BMP_WIDTH][BMP_HEIGTH]);

void grayscale(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS],
               unsigned char output_image[BMP_WIDTH][BMP_HEIGTH]);

void grayscale2(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS],
                unsigned char output_image[BMP_WIDTH][BMP_HEIGTH]);

#endif

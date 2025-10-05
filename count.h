#ifndef COUNT_H
#define COUNT_H

#include "cbmp.h"
#include "otsu.h"
#include "pixelarray.h"
#include "stdio.h"

int cellCounter(uint8_t *in, Coordinate_Array *array);

int erode(uint8_t *in, uint8_t *out);

#endif

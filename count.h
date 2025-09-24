#ifndef COUNT_H
#define COUNT_H

#include "cbmp.h"
#include "otsu.h"
#include "pixelarray.h"
#include "stdio.h"

int cellCounter(u_int8_t* in, Coordinate_Array* array);

int erode(u_int8_t* in,u_int8_t* out);

#endif

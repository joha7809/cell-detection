#ifndef WATERSHED_H
#define WATERSHED_H

#include "cbmp.h"
#include <sys/_types/_u_int8_t.h>

#ifdef __cplusplus
extern "C" {
#endif

void flood_fill(u_int8_t *input_img, int visited[BMP_WIDTH][BMP_HEIGTH], int x, int y, int marker);
void watershed(u_int8_t *input);

#ifdef __cplusplus
}
#endif

#endif // WATERSHED_H

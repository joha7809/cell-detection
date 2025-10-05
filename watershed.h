#ifndef WATERSHED_H
#define WATERSHED_H

#include "cbmp.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void flood_fill(uint8_t *input_img, int visited[BMP_WIDTH][BMP_HEIGTH], int x,
                int y, int marker);
void watershed(uint8_t *input);

#ifdef __cplusplus
}
#endif

#endif // WATERSHED_H

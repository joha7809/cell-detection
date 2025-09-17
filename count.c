#include "count.h"
#include "cbmp.h"
#include "pixelarray.h"

int cellCounter(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH], Coordinate_Array* array) {

  int cells = 0;

  for (int x = 0; x < BMP_WIDTH; x++) {
    for (int y = 0; y < BMP_HEIGTH; y++) {

      if (input_image[x][y] == 255) {

        int connected = 0;

        int l = 6, r = 6, u = 6, d = 6;

        if (x < l) {
          l = x;
        }
        if ((BMP_WIDTH - 1) - x < r) {
          r = (BMP_WIDTH - 1) - x;
        }
        if (y < u) {
          u = y;
        }
        if ((BMP_HEIGTH - 1) - y < d) {
          d = (BMP_HEIGTH - 1) - y;
        }

        for (int i = -l; i <= r; i++) {
          if (input_image[x + i][y - u] == 255 ||
              input_image[x + i][y + d] == 255) {
            connected = 1;
          }
        }

        for (int i = -u; i <= d; i++) {
          if (input_image[x - l][y + i] == 255 ||
              input_image[x + r][y + i] == 255) {
            connected = 1;
          }
        }

        // Create Struct to hold these values:
        if (!connected) {
            Coordinate coordinate = {coordinate.x = x, coordinate.y = y};
            append(coordinate, array);
    
            cells++;

          // HER SÆTTER DU IND I ARRAY


        
          // array of cell locations append.input_image[x][y]
          for (int i = -l; i <= r; i++) {
            for (int j = -u; j <= d; j++) {
              input_image[x + i][y + j] = 0;
            }
          }
        }
      }
    }
  }

  return cells;
}


int erode(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH],
          unsigned char output_image[BMP_WIDTH][BMP_HEIGTH]) {

  int change = 0;

  for (int x = 0; x < BMP_WIDTH; x++) {
    for (int y = 0; y < BMP_HEIGTH; y++) {

      if (input_image[x][y] == 255) {

        int isWhite = 1;

        if (x == 0 || x == BMP_WIDTH-1) {
            isWhite = 0;
        } else if (y == 0 || y == BMP_HEIGTH-1) {
            isWhite = 0;
        } else {
          if ((input_image[x - 1][y] == 0) ||
              (input_image[x + 1][y] == 0) ||
              (input_image[x][y - 1] == 0) ||
              (input_image[x][y + 1] == 0)) {
            isWhite = 0;
          }
        }

        if (isWhite) {
          output_image[x][y] = 255;
        } else {
          change = 1;
          output_image[x][y] = 0;
        }
      } else {
        output_image[x][y] = 0;
      }
    }
  }
  return change;
}
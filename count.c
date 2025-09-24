#include "count.h"
#include "cbmp.h"
#include "pixelarray.h"

int cellCounter(u_int8_t* grid, Coordinate_Array* array) {

  int cells = 0;

  for (int x = 0; x < BMP_WIDTH; x++) {
    for (int y = 0; y < BMP_HEIGTH; y++) {

      if (get(grid, x, y) == 1) {

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
          if (get(grid,(x + i),(y - u)) == 1 ||
              get(grid,(x + i),(y + d)) == 1) {
            connected = 1;
          }
        }

        for (int i = -u; i <= d; i++) {
          if (get(grid,(x - l),(y + i)) == 1 ||
              get(grid,(x + r),(y + i)) == 1) {
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
              set_zero(grid, (x + i),(y + j));
            }
          }
        }
      }
    }
  }

  return cells;
}


int erode(u_int8_t* grid_in,
          u_int8_t* grid_out) {

  int change = 0;

  for (int x = 0; x < BMP_WIDTH; x++) {
    for (int y = 0; y < BMP_HEIGTH; y++) {

      if (get(grid_in,x,y) == 1) {

        int isWhite = 1;

        if (x == 0 || x == BMP_WIDTH-1) {
            isWhite = 0;
        } else if (y == 0 || y == BMP_HEIGTH-1) {
            isWhite = 0;
        } else {
          if ((get(grid_in,(x-1),y) == 0) ||
              (get(grid_in,(x+1),y) == 0) ||
              (get(grid_in,x,(y-1)) == 0) ||
              (get(grid_in,x,(y+1)) == 0)) {
            isWhite = 0;
          }
        }

        if (isWhite) {
          set_one(grid_out, x,y);
        } else {
          change = 1;
          set_zero(grid_out, x,y);
        }
      } else {
        set_zero(grid_out, x,y);
      }
    }
  }
  return change;
}
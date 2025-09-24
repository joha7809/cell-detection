#include "count.h"
#include "cbmp.h"
#include "pixelarray.h"
#include "math.h"

int cellCounter(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH], Coordinate_Array* array) {

  int cells = 0;

  for (int x = 0; x < BMP_WIDTH; x++) {
    for (int y = 0; y < BMP_HEIGTH; y++) {

      if (input_image[x][y] == 255) {
        
        int size = 6;

        int connected = isConnectedCircle(input_image, x, y, size);

        // Create Struct to hold these values:
        if (!connected) {
          Coordinate coordinate = {coordinate.x = x, coordinate.y = y};
          // HER SÆTTER DU IND I ARRAY
          append(coordinate, array);
          cells++;
          clearCircle(input_image, x, y, size);
        }
      }
    }
  }

  return cells;
}

int isConnected(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH], int x, int y, int size) {
  // Exclusion Square
  int connected = 0;

  int left = x < size ? x : size;
  int right = ((BMP_WIDTH - 1) - x) < size ? ((BMP_WIDTH - 1) - x) : size;
  int up = y < size ? y : size;
  int down = ((BMP_HEIGTH - 1) - y) < size ? ((BMP_HEIGTH - 1) - y) : size;


  for (int i = -left; i <= right; i++) {
    if (input_image[x + i][y - up] == 255 ||
        input_image[x + i][y + down] == 255) {
      connected = 1;
    }
  }

  for (int i = -up; i <= down; i++) {
    if (input_image[x - left][y + i] == 255 ||
        input_image[x + right][y + i] == 255) {
      connected = 1;
    }
  }

  return connected;

}

int isConnectedCircle(unsigned char image[BMP_WIDTH][BMP_HEIGTH], int cx, int cy, int radius) {
    int connected = 0;
    int x = radius;
    int y = 0;
    int err = 1 - radius;  // initial decision parameter

    while (x >= y) {
        // plot 8 symmetric points
        int pts[8][2] = {
            { cx + x, cy + y },
            { cx + y, cy + x },
            { cx - y, cy + x },
            { cx - x, cy + y },
            { cx - x, cy - y },
            { cx - y, cy - x },
            { cx + y, cy - x },
            { cx + x, cy - y }
        };

        for (int i = 0; i < 8; i++) {
            int px = pts[i][0];
            int py = pts[i][1];
            if (px >= 0 && px < BMP_WIDTH && py >= 0 && py < BMP_HEIGTH) {
                if (0 < image[px][py]){
                  connected += 1;
                }
            }
        }

        y++;
        if (err < 0) {
            err += 2*y + 1;
        } else {
            x--;
            err += 2*(y - x) + 1;
        }
    }

    if (1 > connected) {
      return 0;
    } else {
      return 1;
    }
}


void clearSquare (unsigned char input_image[BMP_WIDTH][BMP_HEIGTH], int x, int y, int size) {

  int l = x < size ? x : size;
  int r = ((BMP_WIDTH - 1) - x) < size ? ((BMP_WIDTH - 1) - x) : size;
  int u = y < size ? y : size;
  int d = ((BMP_HEIGTH - 1) - y) < size ? ((BMP_HEIGTH - 1) - y) : size;

  for (int i = -l; i <= r; i++) {
      for (int j = -u; j <= d; j++) {
        input_image[x + i][y + j] = 0;
      }
    }
}


void clearCircle(unsigned char image[BMP_WIDTH][BMP_HEIGTH],
                         int cx, int cy, int radius) 
                                                                      {
    int x = radius;
    int y = 0;
    int err = 1 - radius;

    while (x >= y) {
        // draw horizontal spans across symmetric octants
        for (int dx = -x; dx <= x; dx++) {
            int px1 = cx + dx, py1 = cy + y;
            int px2 = cx + dx, py2 = cy - y;
            if (px1 >= 0 && px1 < BMP_WIDTH && py1 >= 0 && py1 < BMP_HEIGTH) {
                image[px1][py1] = 0;
            }
            if (px2 >= 0 && px2 < BMP_WIDTH && py2 >= 0 && py2 < BMP_HEIGTH) {
                image[px2][py2] = 0;
            }
        }

        for (int dx = -y; dx <= y; dx++) {
            int px1 = cx + dx, py1 = cy + x;
            int px2 = cx + dx, py2 = cy - x;
            if (px1 >= 0 && px1 < BMP_WIDTH && py1 >= 0 && py1 < BMP_HEIGTH) {
                image[px1][py1] = 0;
            }
            if (px2 >= 0 && px2 < BMP_WIDTH && py2 >= 0 && py2 < BMP_HEIGTH) {
                image[px2][py2] = 0;
            }
        }

        y++;
        if (err < 0) {
            err += 2*y + 1;
        } else {
            x--;
            err += 2*(y - x) + 1;
        }
    }
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
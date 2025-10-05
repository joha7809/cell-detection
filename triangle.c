#include "cbmp.h"

void drawTriangle(
    unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], int x,
    int y, int size) {
  int currX = x;
  int endX = x + size;

  int currY = y;
  int endY = y - size;

  int count = 0;
  int count2 = 0;

  for (; currY > endY; currY--) {

    count2++;
    if (count2 % 2 == 0) {
      count++;
    }
    if (currY < 0 || currY >= BMP_HEIGTH) {
      continue;
    }

    for (; currX < endX; currX++) {
      if (currX < 0 || currX >= BMP_WIDTH) {
        continue;
      }

      input_image[currX][currY][0] = currX % 255;
      input_image[currX][currY][1] = currY % 255;
      input_image[currX][currY][2] = (currX + currY) % 255;
    }

    currX = x;
    endX = x + size;
    currX += count;
    endX -= count;
  }
}

void triforce(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS],
              int x, int y) {
  int size = 8;
  // bottom right
  drawTriangle(input_image, x, y + size, size);

  // bottom left
  drawTriangle(input_image, x - size, y + size, size);

  // top
  drawTriangle(input_image, x - (size / 2), y, size);
}

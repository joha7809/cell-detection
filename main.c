// To compile (linux/mac): gcc cbmp.c main.c otsu.c count.c -o main.out
// -std=c99gcc -o test_coordinates coordinates.c test_coordinates.c To run
// (linux/mac): ./main.out example.bmp example_inv.bmp

// To compile (win): gcc cbmp.c main.c -o main.exe -std=c99
// To run (win): main.exe example.bmp example_inv.bmp

#include "cbmp.h"
#include "count.h"
#include "otsu.h"
#include "pixelarray.h"
#include "triangle.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "math.h"

// Declaring the array to store the image (unsigned char = unsigned 8 bit)
unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
unsigned char temp_image[BMP_WIDTH][BMP_HEIGTH];
unsigned char temp_image2[BMP_WIDTH][BMP_HEIGTH];

void drawCircle(unsigned char image[BMP_WIDTH][BMP_HEIGTH][3],
                int cx, int cy, int radius) {
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
                image[px][py][0] = 0;   // red=0
                image[px][py][1] = 255; // green=255
                image[px][py][2] = 0;   // blue=0
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

void fillCircleBresenham(unsigned char image[BMP_WIDTH][BMP_HEIGTH][3],
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
                image[px1][py1][0] = 0;
                image[px1][py1][1] = 255;
                image[px1][py1][2] = 0;
            }
            if (px2 >= 0 && px2 < BMP_WIDTH && py2 >= 0 && py2 < BMP_HEIGTH) {
                image[px2][py2][0] = 0;
                image[px2][py2][1] = 255;
                image[px2][py2][2] = 0;
            }
        }

        for (int dx = -y; dx <= y; dx++) {
            int px1 = cx + dx, py1 = cy + x;
            int px2 = cx + dx, py2 = cy - x;
            if (px1 >= 0 && px1 < BMP_WIDTH && py1 >= 0 && py1 < BMP_HEIGTH) {
                image[px1][py1][0] = 0;
                image[px1][py1][1] = 255;
                image[px1][py1][2] = 0;
            }
            if (px2 >= 0 && px2 < BMP_WIDTH && py2 >= 0 && py2 < BMP_HEIGTH) {
                image[px2][py2][0] = 0;
                image[px2][py2][1] = 255;
                image[px2][py2][2] = 0;
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



// Main function
int main(int argc, char **argv) {

  // argc counts how may arguments are passed
  // argv[0] is a string with the name of the program
  // argv[1] is the first command line argument (input image)
  // argv[2] is the second command line argument (output image)

  // Checking that 2 arguments are passed
  if (argc != 3) {
    fprintf(stderr, "Usage: %s <output file path> <output file path>\n",
            argv[0]);
    exit(1);
  }

  printf("Program Running...\n");

  // Load image from file
  read_bitmap(argv[1], input_image);

  grayscale(input_image, temp_image);

  threshold(temp_image, temp_image);
  // otsu(temp_image, temp_image);
  // write to bitmap

  unsigned char (*input)[BMP_HEIGTH] = temp_image;
  unsigned char (*output)[BMP_HEIGTH] = temp_image2;

  int change = 1;
  int cells = 0;
  Coordinate_Array array = init_array(50);
  
  while (change) {
    change = erode(input, output);
    
    cells += cellCounter(output, &array);

    unsigned char (*tmp)[BMP_HEIGTH] = input;
    input = output;
    output = tmp;
  }

  printf("Cells: %d\n", cells);

  // Mark every cell in the original image with a triforce
  for (int i = 0; i < cells; i++) {
    int x = array.data[i].x;
    int y = array.data[i].y;
  
    triforce(input_image, x, y);
  }



  
  // output the original image with triforces
  write_bitmap(input_image, "pretty.bmp");
  printf("Done!\n");
  return 0;
}

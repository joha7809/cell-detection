// To compile (linux/mac): gcc cbmp.c main.c otsu.c count.c -o main.out -std=c99
// To run (linux/mac): ./main.out example.bmp example_inv.bmp

// To compile (win): gcc cbmp.c main.c -o main.exe -std=c99
// To run (win): main.exe example.bmp example_inv.bmp

#include "cbmp.h"
#include "count.h"
#include "otsu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Declaring the array to store the image (unsigned char = unsigned 8 bit)
unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
unsigned char temp_image[BMP_WIDTH][BMP_HEIGTH];
unsigned char temp_image2[BMP_WIDTH][BMP_HEIGTH];

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

  printf("Example program - 02132 - A1\n");

  // Load image from file
  read_bitmap(argv[1], input_image);

  // Run inversion
  grayscale(input_image, temp_image);

  // threshold(temp_image, temp_image);
  otsu(temp_image, temp_image);
  // write to bitmap

  unsigned char (*input)[BMP_HEIGTH] = temp_image;
  unsigned char (*output)[BMP_HEIGTH] = temp_image2;

  int change = 1;
  int temp = 20;
  int tempCounter = temp;
  int cells = 0;
  while (change) {
    if (temp == 0) {
      break;
    }
    temp--;
    // change = erode(input, output);

    erode(input, output);

    cells += cellCounter(output);

    unsigned char (*tmp)[BMP_HEIGTH] = input;
    input = output;
    output = tmp;

    // erode2(input);

    for (int x = 0; x < BMP_WIDTH; x++) {
      for (int y = 0; y < BMP_HEIGTH; y++) {

        for (int c = 0; c < BMP_CHANNELS; c++) {
          output_image[x][y][c] = output[x][y];
        }
      }
    }

    char filename[64]; // stack buffer
    snprintf(filename, sizeof(filename), "output/erode%d.bmp",
             tempCounter - temp); // no /
    write_bitmap(output_image, filename);
  }

  printf("Cells: %d\n", cells);

  printf("Done!\n");
  return 0;
}

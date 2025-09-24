// To compile (linux/mac): gcc cbmp.c main.c otsu.c count.c -o main.out
// -std=c99gcc -o test_coordinates coordinates.c test_coordinates.c To run
// (linux/mac): ./main.out example.bmp example_inv.bmp

// To compile (win): gcc cbmp.c main.c -o main.exe -std=c99
// To run (win): main.exe example.bmp example_inv.bmp

#include "cbmp.h"
#include "count.h"
#include "gaussian_blur.h"
#include "otsu.h"
#include "pixelarray.h"
#include "triangle.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Declaring the array to store the image (unsigned char = unsigned 8 bit)
unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
unsigned char procs_image[BMP_WIDTH][BMP_HEIGTH];
u_int8_t temp_image[(BMP_WIDTH*BMP_HEIGTH+7)/8] = {0};
u_int8_t temp_image2[(BMP_WIDTH*BMP_HEIGTH+7)/8] = {0};
unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];

void convert_to_image(u_int8_t* grid, unsigned char out[BMP_WIDTH][BMP_HEIGTH]) {
    for (int y = 0; y < BMP_HEIGTH; y++) {
        for (int x = 0; x < BMP_WIDTH; x++) {
            int byteIndex = (y * BMP_WIDTH + x) / 8;          // Which byte contains the pixel
            int bitIndex = x % 8;                             // Which bit within that byte corresponds to the pixel

            // Extract the bit and reverse the bit order within the byte
            int bit = (grid[byteIndex] & (1 << (7 - bitIndex))) >> (7 - bitIndex);
            int color;
            if (bit) {
              color = 255;
            } else {
              color = 0;
            }
            // Map the result into the temp_image2 array
            out[x][y] = color;
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

  // grayscale(input_image, temp_image);
  grayscale(input_image, procs_image);

  // gaussian_blur(temp_image, temp_image2);

  // write_bitmap(output_image, "blurred.bmp");
  // threshold(temp_image, temp_image);
  otsu(procs_image, temp_image);
  // fill_holes(temp_image);

  u_int8_t (*input) = temp_image;
  u_int8_t (*output) = temp_image2;

  int change = 1;
  int cells = 0;
  Coordinate_Array array = init_array(50);

  int erode_count = 0;
  while (change) {
    change = erode(input, output);
    convert_to_image(output, output_image); 
    char filename[256];
    sprintf(filename, "output/step_%d_.bmp", erode_count);
    write_bitmap(output_image, filename);
    erode_count++;

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

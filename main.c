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
#include <time.h>

// source:
// https://stackoverflow.com/questions/39729876/is-it-a-way-to-create-a-clever-macro-to-automatically-benchmark-something-in-c
// Enabled with -DBENCHMARK
#ifdef BENCHMARK
#define benchmark(x)                                                           \
  for (double startTime = (double)clock() / CLOCKS_PER_SEC, run = 1.0;         \
       run == 1.0;                                                             \
       run = 0, printf("\n%s\nTime elapsed: %f s\n", x,                        \
                       (double)clock() / CLOCKS_PER_SEC - startTime))
#else
#define benchmark(x) if (1)
#endif

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
  benchmark("greyscale") { grayscale(input_image, temp_image); }

  // threshold(temp_image, temp_image);
  benchmark("Threshold") { threshold(temp_image, temp_image); }
  // otsu(temp_image, temp_image);
  // write to bitmap

  unsigned char (*input)[BMP_HEIGTH] = temp_image;
  unsigned char (*output)[BMP_HEIGTH] = temp_image2;

  int change = 1;
  int temp = 20;
  int tempCounter = temp;
  int cells = 0;

  // benchmark("init of \"array\"") { Coordinate_Array array = init_array(50); }
  Coordinate_Array array = init_array(50);
  while (change) {
    if (temp == 0) {
      break;
    }
    temp--;
    // change = erode(input, output);

    benchmark("erode") { erode(input, output); }

    benchmark("cell_counter") { cells += cellCounter(output, &array); }

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

  int found;
  found = array.index;
  printf("Cells in array: %d\n", found);

  for (int i = 0; i < found; i++) {
    int x = array.data[i].x;
    int y = array.data[i].y;

    triforce(input_image, x, y);
    // printf("Point: (%d, %d) \n", x, y);
  }
  write_bitmap(input_image, "pretty.bmp");
  printf("Done!\n");
  return 0;
}

// To compile (linux/mac): gcc cbmp.c main.c -o main.out -std=c99
// To run (linux/mac): ./main.out example.bmp example_inv.bmp

// To compile (win): gcc cbmp.c main.c -o main.exe -std=c99
// To run (win): main.exe example.bmp example_inv.bmp

#include "cbmp.h"
#include <stdio.h>
#include <stdlib.h>

// Declaring the array to store the image (unsigned char = unsigned 8 bit)
unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
unsigned char temp_image[BMP_WIDTH][BMP_HEIGTH];
unsigned char temp_image2[BMP_WIDTH][BMP_HEIGTH];

void grayscale(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS],
               unsigned char output_image[BMP_WIDTH][BMP_HEIGTH]) {
  for (int x = 0; x < BMP_WIDTH; x++) {
    for (int y = 0; y < BMP_HEIGTH; y++) {
      int gray =
          (input_image[x][y][0] + input_image[x][y][1] + input_image[x][y][2]) /
          3;

      output_image[x][y] = gray;
    }
  }
}

void grayscale2(
    unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS],
    unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS]) {
  for (int x = 0; x < BMP_WIDTH; x++) {
    for (int y = 0; y < BMP_HEIGTH; y++) {

      int gray = (input_image[x][y][0] * 0.299 + input_image[x][y][1] * 0.587 +
                  input_image[x][y][2] * 0.114);

      for (int c = 0; c < BMP_CHANNELS; c++) {
        output_image[x][y][c] = gray;
      }
    }
  }
}

void threshold(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH],
               unsigned char output_image[BMP_WIDTH][BMP_HEIGTH]) {
  for (int x = 0; x < BMP_WIDTH; x++) {
    for (int y = 0; y < BMP_HEIGTH; y++) {
      if (input_image[x][y] <= 90) {
        output_image[x][y] = 0;
      } else {
        output_image[x][y] = 255;
      }
    }
  }
}

int erode(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH],
          unsigned char output_image[BMP_WIDTH][BMP_HEIGTH]) {

  int change = 0;

  // Se om dette kan fikses
  for (int x = 0; x < BMP_WIDTH; x++) {
    for (int y = 0; y < BMP_HEIGTH; y++) { 

      if (input_image[x][y] == 255) {
        
        int locChange = 0;
        if (x != 0) {
          if (input_image[x-1][y] == 0) {
            change = 1;
            locChange = 1;
          }
        }
        
        if (x != BMP_WIDTH-1) {
          if (input_image[x+1][y] == 0) {
            change = 1;
            locChange = 1;
          }
        }

        if (y != 0) {
          if (input_image[x][y-1] == 0) {
            change = 1;
            locChange = 1;
          }
        }
        
        if (y != BMP_HEIGTH-1) {
          if (input_image[x][y+1] == 0) {
            change = 1;
            locChange = 1;
          }
        }

        if (locChange) {
          output_image[x][y] = 0;
        } else {
          output_image[x][y] = 255;
        }

      }

    }
  }
  return change;
}


int cellCounter (unsigned char input_image[BMP_WIDTH][BMP_HEIGTH]){
  
  int cells = 0;
  int dy0, dy1, dx0, dy1, size = 6;


  for (int x = 0; x < BMP_WIDTH; x++) {
    for (int y = 0; y < BMP_HEIGTH; y++) { 
      

      if (input_image[x][y] == 255) {
          
        int l,r,u,d = 6;

        if (x < l) {l = x;}
        if ((BMP_WIDTH-1)-x < r) {r = (BMP_WIDTH-1)-x;}
        if (y < u) {u = y;}
        if ((BMP_HEIGTH-1)-y < r) {r = (BMP_HEIGTH-1)-y;}
        

        
        if (x != 0) {
          if (input_image[x-1][y] == 0) {
          }
        }
        
        if (x != BMP_WIDTH-1) {
          if (input_image[x+1][y] == 0) {
          }
        }

        if (y != 0) {
          if (input_image[x][y-1] == 0) {
          }
        }
        
        if (y != BMP_HEIGTH-1) {
          if (input_image[x][y+1] == 0) {
          }
        }

       

      }

    }
  }

  return cells;
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

  printf("Example program - 02132 - A1\n");

  // Load image from file
  read_bitmap(argv[1], input_image);

  // Run inversion
  grayscale(input_image, temp_image);

  threshold(temp_image, temp_image);

  erode(temp_image, temp_image2);

  // Save image to file

  for (int x = 0; x < BMP_WIDTH; x++)
  {
    for (int y = 0; y < BMP_HEIGTH; y++)
    { 
      
      for (int c = 0; c < BMP_CHANNELS; c++){
        output_image[x][y][c] = temp_image[x][y];
      }
    }
  }

  write_bitmap(output_image, argv[2]);

  for (int x = 0; x < BMP_WIDTH; x++) {
    for (int y = 0; y < BMP_HEIGTH; y++) {

      for (int c = 0; c < BMP_CHANNELS; c++) {
        output_image[x][y][c] = temp_image2[x][y];
      }
    }
  }

  write_bitmap(output_image, argv[2] + 1);


  int temp = 20;
  while (temp) {
    erode(temp_image, temp_image2);
    
    for (int x = 0; x < BMP_WIDTH; x++) {
      for (int y = 0; y < BMP_HEIGTH; y++) {  
        temp_image[x][y] = temp_image2[x][y];
      }
    }

    temp--;

    for (int x = 0; x < BMP_WIDTH; x++) {
      for (int y = 0; y < BMP_HEIGTH; y++) {

        for (int c = 0; c < BMP_CHANNELS; c++) {
          output_image[x][y][c] = temp_image2[x][y];
        }
      }
    }

   char filename[64];   // stack buffer
snprintf(filename, sizeof(filename), "erode%d.bmp", 22 - temp);  // no /
write_bitmap(output_image, filename);

  }


  for (int x = 0; x < BMP_WIDTH; x++) {
    for (int y = 0; y < BMP_HEIGTH; y++) {

      for (int c = 0; c < BMP_CHANNELS; c++) {
        output_image[x][y][c] = temp_image2[x][y];
      }
    }
  }

  write_bitmap(output_image, argv[2] + 2);




  /*
  unsigned char (*input)[BMP_HEIGTH] = temp_image;
  unsigned char (*output)[BMP_HEIGTH] = temp_image2;

  int change = 1;
  int t = 0;
  while (change) {
    if (t == 10) {
      break;
    }
    t++;
    change = erode(input, output);

    unsigned char (*tmp)[BMP_HEIGTH] = input;
    input = output;
    output = tmp;
  }

  for (int x = 0; x < BMP_WIDTH; x++) {
    for (int y = 0; y < BMP_HEIGTH; y++) {

      for (int c = 0; c < BMP_CHANNELS; c++) {
        output_image[x][y][c] = output[x][y];
      }
    }
  }

  write_bitmap(output_image, argv[2] + 2);
  */


  printf("Done!\n");
  return 0;
}

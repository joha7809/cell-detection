#include "otsu.h"
#include "cbmp.h"

#define total_pixels (950 * 950);

// void threshold(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH],
//                unsigned char output_image[BMP_WIDTH][BMP_HEIGTH]) {
//   for (int x = 0; x < BMP_WIDTH; x++) {
//     for (int y = 0; y < BMP_HEIGTH; y++) {
//       if (input_image[x][y] <= 90) {
//         output_image[x][y] = 0;
//       } else {
//         output_image[x][y] = 255;
//       }
//     }
//   }
// }

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

// void grayscale2(
//     unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS],
//     unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS]) {
//   for (int x = 0; x < BMP_WIDTH; x++) {
//     for (int y = 0; y < BMP_HEIGTH; y++) {

//       int gray = (input_image[x][y][0] * 0.299 + input_image[x][y][1] * 0.587 +
//                   input_image[x][y][2] * 0.114);

//       for (int c = 0; c < BMP_CHANNELS; c++) {
//         output_image[x][y][c] = gray;
//       }
//     }
//   }
// }


void otsu(
    unsigned char input_image[BMP_WIDTH][BMP_HEIGTH],
    unsigned char output_image[BMP_WIDTH][BMP_HEIGTH]) { // THRESHOLD VERSION
                                                         // #2: OTSU METHOD

  unsigned int histogram[256] = {
      0}; // Making a histogram over the frequency of every intensity (assuming
          // it's been grayscaled already so there's only 1)
  // Spent 3 hours debugging, turned out it being an unsigned char was making it
  // overflow )))))))) IDI NAHUI

  for (int x = 0; x < BMP_WIDTH; x++) {
    for (int y = 0; y < BMP_HEIGTH; y++) {
      histogram[input_image[x][y]] += 1;
    }
  }

  unsigned int cumul_sum = 0;

  // Need to go through once first since the cumuluative sum should be constant
  // for the later calculations
  for (int k = 0; k < 256; k++) {
    cumul_sum += k * histogram[k];
  }

  unsigned int background = 0;
  unsigned int current_sum = 0;
  unsigned int threshold = 0;
  int foreground = 0; // Made it a signed int since it might go kerfuffle if
                      // unsigned due to the subtraction?
  float mean_background = 0;
  // Floats since division
  float mean_foreground = 0;
  float variance = 0;
  float max_variance = 0;

  // Calculating all the possible thresholds (Need the means & stuff for the
  // threshold)
  for (int l = 0; l < 255; l++) {
    current_sum += l * histogram[l];
    background += histogram[l];
    foreground = (950*950) - background;

    if (background != 0 &&
        foreground !=
            0) { // Will be dividing with them, so if they're 0 it's a big no no

      mean_background = (float)current_sum / background;
      mean_foreground = (float)(cumul_sum - current_sum) / foreground;
      variance = (double)background * (double)foreground *
                 ((mean_background - mean_foreground) *
                  (mean_background - mean_foreground));

      if (variance > max_variance) {
        max_variance = variance;
        threshold = l;
      }

    } else {
      continue;
    }
  }

  for (int x = 0; x < BMP_WIDTH; x++) {
    for (int y = 0; y < BMP_HEIGTH; y++) {
      if (input_image[x][y] <= threshold) {
        output_image[x][y] = 0;
      } else {
        output_image[x][y] = 255;
      }
    }
  }
}

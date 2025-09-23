#include "gaussian_blur.h"
#include "cbmp.h"

// void gaussian_blur(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH],
//                    unsigned char output_image[BMP_WIDTH][BMP_HEIGTH]) {
//   float kernel[3][3] = {
//       {0.094716f, 0.118318f, 0.094716f},
//       {0.118318f, 0.147761f, 0.118318f},
//       {0.094716f, 0.118318f, 0.094716f},
//   };
//
//   for (int x = 1; x < BMP_WIDTH - 1; x++) {
//     for (int y = 1; y < BMP_HEIGTH - 1; y++) {
//       float sum = 0.0f;
//
//       for (int kx = -1; kx <= 1; kx++) {
//         for (int ky = -1; ky <= 1; ky++) {
//           int pixel = input_image[x + kx][y + ky];
//           float weight = kernel[kx + 1][ky + 1];
//
//           sum += pixel * weight;
//         }
//       }
//
//       // Clamp to 0–255 range and convert back to byte
//       if (sum < 0.0f)
//         sum = 0.0f;
//       if (sum > 255.0f)
//         sum = 255.0f;
//
//       output_image[x][y] = (unsigned char)(sum + 0.5f); // rounding
//     }
//   }
// }
void gaussian_blur(unsigned char input_image[BMP_HEIGTH][BMP_WIDTH],
                   unsigned char output_image[BMP_HEIGTH][BMP_WIDTH]) {
  int kernel[3] = {1, 2, 1};
  int temp[BMP_HEIGTH][BMP_WIDTH];

  // Horizontal pass
  for (int y = 0; y < BMP_HEIGTH; y++) {
    for (int x = 1; x < BMP_WIDTH - 1; x++) {
      int sum = input_image[y][x - 1] * kernel[0] +
                input_image[y][x] * kernel[1] +
                input_image[y][x + 1] * kernel[2];
      temp[y][x] = sum / 4; // normalize horizontally
    }
  }

  // Vertical pass
  for (int y = 1; y < BMP_HEIGTH - 1; y++) {
    for (int x = 0; x < BMP_WIDTH; x++) {
      int sum = temp[y - 1][x] * kernel[0] + temp[y][x] * kernel[1] +
                temp[y + 1][x] * kernel[2];
      output_image[y][x] = sum / 4; // normalize vertically
    }
  }
}
// Flood fill (DFS or BFS)
void flood_fill(unsigned char img[BMP_WIDTH][BMP_HEIGTH],
                int visited[BMP_WIDTH][BMP_HEIGTH], int x, int y) {
  if (x < 0 || x >= BMP_WIDTH || y < 0 || y >= BMP_HEIGTH)
    return;
  if (visited[x][y])
    return;
  if (img[x][y] != 0)
    return; // only flood black

  visited[x][y] = 1;

  flood_fill(img, visited, x + 1, y);
  flood_fill(img, visited, x - 1, y);
  flood_fill(img, visited, x, y + 1);
  flood_fill(img, visited, x, y - 1);
}

void fill_holes(unsigned char image[BMP_WIDTH][BMP_HEIGTH]) {
  int visited[BMP_WIDTH][BMP_HEIGTH] = {0};

  // Flood-fill from border pixels
  for (int x = 0; x < BMP_WIDTH; x++) {
    flood_fill(image, visited, x, 0);
    flood_fill(image, visited, x, BMP_HEIGTH - 1);
  }
  for (int y = 0; y < BMP_HEIGTH; y++) {
    flood_fill(image, visited, 0, y);
    flood_fill(image, visited, BMP_WIDTH - 1, y);
  }

  // Any black pixel not visited = hole, fill it
  for (int x = 0; x < BMP_WIDTH; x++) {
    for (int y = 0; y < BMP_HEIGTH; y++) {
      if (image[x][y] == 0 && !visited[x][y]) {
        image[x][y] = 255; // fill hole
      }
    }
  }
}

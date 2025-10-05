#include "cbmp.h"
#include "pixelarray.h"
#include <stdio.h>
#include <stdlib.h>

#define min(a, b) ((a) < (b) ? (a) : (b))
#define MAX_MARKERS 1000

typedef struct {
  int x;
  int y;
  u_int16_t type; // marker label
} Marker;

// Flood fill (DFS or BFS)
void flood_fill(uint8_t *input_img, uint16_t visited[BMP_WIDTH][BMP_HEIGTH],
                int x, int y, int marker) {
  if (x < 0 || x >= BMP_WIDTH || y < 0 || y >= BMP_HEIGTH)
    return;
  if (visited[x][y])
    return;
  if (get(input_img, x, y) == 0)
    return; // only flood black

  visited[x][y] = marker;
  // printf("Marked visited[%d][%d] with marker %d\n", x, y, marker);

  flood_fill(input_img, visited, x + 1, y, marker);
  flood_fill(input_img, visited, x - 1, y, marker);
  flood_fill(input_img, visited, x, y + 1, marker);
  flood_fill(input_img, visited, x, y - 1, marker);
}

uint16_t *height_map(uint8_t *input) {
  const int MAX_INT = 65500;
  uint16_t *buffer = malloc(BMP_WIDTH * BMP_HEIGTH * sizeof(uint16_t));

  // Initialize
  for (int y = 0; y < BMP_HEIGTH; y++) {
    for (int x = 0; x < BMP_WIDTH; x++) {
      buffer[y * BMP_WIDTH + x] = (get(input, x, y) == 0) ? 0 : MAX_INT;
    }
  }

  // First pass: top-left → bottom-right
  for (int y = 0; y < BMP_HEIGTH; y++) {
    for (int x = 0; x < BMP_WIDTH; x++) {
      if (buffer[y * BMP_WIDTH + x] != 0) {
        int val = buffer[y * BMP_WIDTH + x];

        // Horizontal and vertical neighbors
        if (x > 0)
          val = min(val, buffer[y * BMP_WIDTH + (x - 1)] + 1);
        if (y > 0)
          val = min(val, buffer[(y - 1) * BMP_WIDTH + x] + 1);

        buffer[y * BMP_WIDTH + x] = val;
      }
    }
  }

  // Second pass: bottom-right → top-left
  for (int y = BMP_HEIGTH - 1; y >= 0; y--) {
    for (int x = BMP_WIDTH - 1; x >= 0; x--) {
      if (buffer[y * BMP_WIDTH + x] != 0) {
        int val = buffer[y * BMP_WIDTH + x];

        // Horizontal and vertical neighbors
        if (x < BMP_WIDTH - 1)
          val = min(val, buffer[y * BMP_WIDTH + (x + 1)] + 1);
        if (y < BMP_HEIGTH - 1)
          val = min(val, buffer[(y + 1) * BMP_WIDTH + x] + 1);

        buffer[y * BMP_WIDTH + x] = val;
      }
    }
  }

  return buffer;
}

/* tune these */
#define LOCAL_RADIUS                                                           \
  1 /* 1 => 3x3 neighborhood (includes diagonals). Increase to 2 for 5x5, ...  \
     */
#define MIN_HEIGHT                                                             \
  1 /* ignore maxima with value < MIN_HEIGHT; set to 1 to keep all */

int find_local_maxima_per_component(uint16_t *height_map,
                                    uint16_t visited[BMP_WIDTH][BMP_HEIGTH],
                                    Marker *markers, int max_markers) {
  int count = 0;

  // Find maximum label
  uint16_t max_label = 0;
  for (int y = 0; y < BMP_HEIGTH; y++)
    for (int x = 0; x < BMP_WIDTH; x++)
      if (visited[x][y] > max_label)
        max_label = visited[x][y];

  for (uint16_t label = 1; label <= max_label; label++) {
    for (int y = 0; y < BMP_HEIGTH; y++) {
      for (int x = 0; x < BMP_WIDTH; x++) {
        if (visited[x][y] != label)
          continue;
        int val = height_map[y * BMP_WIDTH + x];
        if (val < MIN_HEIGHT)
          continue;

        int is_max = 1;
        for (int dy = -LOCAL_RADIUS; dy <= LOCAL_RADIUS && is_max; dy++) {
          for (int dx = -LOCAL_RADIUS; dx <= LOCAL_RADIUS; dx++) {
            if (dx == 0 && dy == 0)
              continue;
            int nx = x + dx, ny = y + dy;
            if (nx < 0 || nx >= BMP_WIDTH || ny < 0 || ny >= BMP_HEIGTH)
              continue;
            if (height_map[ny * BMP_WIDTH + nx] >= val) {
              is_max = 0;
              break;
            }
          }
        }

        if (is_max) {
          if (count >= max_markers)
            return count;
          markers[count].x = x;
          markers[count].y = y;
          markers[count].type = label;
          count++;
        }
      }
    }
  }
  return count;
}

void watershed(uint8_t *input) {
  // We directly modify the input grid
  uint16_t visited[BMP_WIDTH][BMP_HEIGTH] = {0};

  uint16_t marker = 1;
  for (int x = 0; x < BMP_WIDTH; x++) {
    for (int y = 0; y < BMP_HEIGTH; y++) {
      if ((visited[x][y] == 0) && (get(input, x, y) == 1)) {
        flood_fill(input, visited, x, y, marker);
        marker++;
      }
    }
  }
  uint16_t *buffer = height_map(input);

  // Find local maxima as markers
  Marker markers[MAX_MARKERS];
  int num_markers =
      find_local_maxima_per_component(buffer, visited, markers, MAX_MARKERS);

  printf("Found %d markers (one per component):\n", num_markers);
  for (int i = 0; i < num_markers; i++) {
    printf("Marker for component %d: x=%d, y=%d, height=%d\n", markers[i].type,
           markers[i].x, markers[i].y,
           buffer[markers[i].y * BMP_WIDTH + markers[i].x]);
  }
  free(buffer);
}

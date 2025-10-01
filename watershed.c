#include "cbmp.h"
#include "pixelarray.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/_types/_u_int16_t.h>
#include <sys/_types/_u_int8_t.h>

#define min(a, b) ((a) < (b) ? (a) : (b))
#define MAX_MARKERS 1000

typedef struct {
  int x;
  int y;
  u_int16_t type; // marker label
} Marker;

// Flood fill (DFS or BFS)
void flood_fill(u_int8_t *input_img, u_int16_t visited[BMP_WIDTH][BMP_HEIGTH],
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

u_int16_t *height_map(u_int8_t *input) {
  const int MAX_INT = 65500;
  u_int16_t *buffer = malloc(BMP_WIDTH * BMP_HEIGTH * sizeof(u_int16_t));

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

int find_local_maxima_per_component(u_int16_t *height_map,
                                    u_int16_t visited[BMP_WIDTH][BMP_HEIGTH],
                                    Marker *markers, int max_markers) {
  int count = 0;

  // Find maximum label
  u_int16_t max_label = 0;
  for (int y = 0; y < BMP_HEIGTH; y++)
    for (int x = 0; x < BMP_WIDTH; x++)
      if (visited[x][y] > max_label)
        max_label = visited[x][y]; // Scan each component
  for (u_int16_t label = 1; label <= max_label; label++) {
    for (int y = 1; y < BMP_HEIGTH - 1; y++) {
      for (int x = 1; x < BMP_WIDTH - 1; x++) {
        if (visited[x][y] != label)
          continue;
        int val = height_map[y * BMP_WIDTH + x];
        if (val < 2)
          continue; // Ignore low maxima
        int is_max = 1;
        for (int dy = -1; dy <= 1; dy++) {
          for (int dx = -1; dx <= 1; dx++) {
            if (dx == 0 && dy == 0)
              continue;
            if (height_map[(y + dy) * BMP_WIDTH + (x + dx)] >= val) {
              is_max = 0;
              break;
            }
          }
          if (!is_max)
            break;
        }
        if (is_max && val > 0) {
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
void watershed(u_int8_t *input) {
  // We directly modify the input grid
  u_int16_t visited[BMP_WIDTH][BMP_HEIGTH] = {0};

  u_int16_t marker = 1;
  for (int x = 0; x < BMP_WIDTH; x++) {
    for (int y = 0; y < BMP_HEIGTH; y++) {
      if ((visited[x][y] == 0) && (get(input, x, y) == 1)) {
        flood_fill(input, visited, x, y, marker);
        marker++;
      }
    }
  }
  u_int16_t *buffer = height_map(input);

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

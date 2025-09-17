#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
  int x;
  int y;
} Coordinate;

typedef struct {
  int index;
  int capacity;
  Coordinate *data;
} Coordinate_Array;

Coordinate_Array init_array(int size) {
  Coordinate *ptr = malloc(size * sizeof(Coordinate));
  if (ptr == NULL) {
  };

  Coordinate_Array array;
  array.index = 0;
  array.capacity = size;
  array.data = ptr;

  return array;
};

void append(Coordinate point, Coordinate_Array *array) {
  // Apparently type->elem means (*type).elem
  if (array->index == array->capacity - 1) {
    array->capacity *= 2;
    array->data = realloc(array->data, array->capacity * sizeof(Coordinate));
    if (array->data == NULL) {
      // TODO: KILL THE CHILD
    };
  }

  array->data[array->index] = point;
  array->index += 1;
};

// int main() {
//   int n = 100000000; // Number of appends to test
//   Coordinate_Array arr = init_array(2);
//   Coordinate pt = {0, 0};
//   clock_t start = clock();
//
//   for (int i = 0; i < n; ++i) {
//     append(pt, &arr);
//   }
//
//   clock_t end = clock();
//   double total_time = (double)(end - start) / CLOCKS_PER_SEC;
//   printf("Total time: %f seconds\n", total_time);
//   printf("Amortized time per append: %e seconds\n", total_time / n);
//
//   // ... free memory, etc. ...
// }

// Linkedlist måske

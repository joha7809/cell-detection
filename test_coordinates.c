#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "pixelarray.h"

// int main() {
//     Coordinate_Array arr = init_array(2);

//     assert(arr.index == 0);
//     assert(arr.capacity == 2);
//     assert(arr.data != NULL);

//     Coordinate p1 = {1, 2};
//     append(p1, &arr);
//     assert(arr.index == 1);
//     assert(arr.data[0].x == 1 && arr.data[0].y == 2);

//     // Append more to test resizing
//     append((Coordinate){3,4}, &arr);
//     assert(arr.index == 2);
//     append((Coordinate){5,6}, &arr);
//     assert(arr.index == 3);
//     assert(arr.capacity != 2);

//     printf("All tests passed!\n");

//     free(arr.data);
//     return 0;
// }

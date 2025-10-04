#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    int x;
    int y;
} Coordinate;

void set_one(u_int8_t* grid, int x, int y){
    int byte_index = y*950+x;
    grid[byte_index/8] |= (1<<(byte_index%8));
};

void set_zero(u_int8_t* grid, int x, int y){
    int byte_index = y*950+x;
    grid[byte_index/8] &= ~(1 << (byte_index%8));
};

int get(u_int8_t* grid, int x, int y){
    int byte_index = y*950+x;
    return (grid[byte_index/8] & (1 << (byte_index%8))) >> (byte_index%8);
};




typedef struct {
    int index;
    int capacity;
    Coordinate* data;
} Coordinate_Array;

Coordinate_Array init_array(int size) {
    Coordinate* ptr = malloc(size * sizeof(Coordinate));
    if(ptr == NULL){};
    
    Coordinate_Array array;
    array.index = 0;
    array.capacity = size;
    array.data = ptr;

    return array;
};


void append(Coordinate point, Coordinate_Array* array) {
    // Apparently type->elem means (*type).elem
    if (array->index == array->capacity-1) {
        array->capacity *= 2;
        array->data = realloc(array->data, array->capacity * sizeof(Coordinate));
        if(array->data == NULL){
            //TODO: DESTROY THE CHILD; CORRUPT THEM ALL
        };
    }
    
    array->data[array->index] = point;
    array->index += 1;
};



// int main() {
//     u_int8_t grid[(950*950+7)/8] = {0};

//     set_one(grid, 38, 40);
//     printf("%d \n", get(grid, 38, 40));
//     set_one(grid, 0, 0);
//     printf("%d \n", get(grid, 0, 0));
//     set_zero(grid, 0,0);
//     printf("%d \n", get(grid, 0, 0));
// }


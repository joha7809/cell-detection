#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    int x;
    int y;
} Coordinate;


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
            //TODO: KILL THE CHILD
        };
    }
    
    array->data[array->index] = point;
    array->index += 1;
};


//Linkedlist måske
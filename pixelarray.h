// coordinates.h
#ifndef COORDINATES_H
#define COORDINATES_H

typedef struct {
    int x;
    int y;
} Coordinate;

typedef struct {
    int index;
    int capacity;
    Coordinate* data;
} Coordinate_Array;

Coordinate_Array init_array(int size);
void append(Coordinate point, Coordinate_Array* array);

#endif // COORDINATES_H

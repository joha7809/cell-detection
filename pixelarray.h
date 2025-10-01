// coordinates.h
#ifndef COORDINATES_H
#define COORDINATES_H

#include <sys/_types/_u_int8_t.h>
typedef struct {
  int x;
  int y;
} Coordinate;

typedef struct {
  int index;
  int capacity;
  Coordinate *data;
} Coordinate_Array;

Coordinate_Array init_array(int size);
void append(Coordinate point, Coordinate_Array *array);

int get(u_int8_t *grid, int x, int y);
void set_one(u_int8_t *grid, int x, int y);
void set_zero(u_int8_t *grid, int x, int y);

#endif // COORDINATES_H

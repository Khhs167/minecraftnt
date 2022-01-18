#ifndef MC_VECTORS_H
#define MC_VECTORS_H

#include <stdlib.h>

typedef struct
{
    float x;
    float y;
    float z;
} Vector3;

Vector3 *Vector3_new(float x, float y, float z);

#endif
#include "vectors.h"

Vector3 *Vector3_new(float x, float y, float z){
    Vector3 *v = malloc(sizeof(Vector3));
    v->x = x;
    v->y = y;
    v->z = z;
    return v;
}
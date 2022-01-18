#ifndef MC_CONST_H

#include "vectors.h"

#define MC_CONST_H

#define MC_WINDOW_SIZE_X (1280)
#define MC_WINDOW_SIZE_Y (720)

#define MC_FACE_FRONT (0)
#define MC_FACE_BACK (1)
#define MC_FACE_RIGHT (2)
#define MC_FACE_LEFT (3)
#define MC_FACE_TOP (4)
#define MC_FACE_BOTTOM (5)

#define MC_CHUNK_WIDTH (16)
#define MC_CHUNK_HEIGHT (256)

#define MC_WORLD_WIDTH (10)

#define MC_TRUE (1)
#define MC_FALSE (0)

Vector3 *face_normals[6];

extern char inside_map(float x, float y, float z);

#define printe(args...)      \
    fprintf(stderr, ##args); \
    exit(-1);

#endif
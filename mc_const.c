#include "mc_const.h"

char inside_map(float x, float y, float z)
{
    if (x < 0 || x >= MC_CHUNK_WIDTH)
        return MC_FALSE;
    if (z < 0 || z >= MC_CHUNK_WIDTH)
        return MC_FALSE;
    if (y < 0 || y >= MC_CHUNK_HEIGHT)
        return MC_FALSE;
    return MC_TRUE;
}
#ifndef MC_CHUNK_H
#define MC_CHUNK_H

#include "mc_const.h"

typedef struct _Chunk{
    int* map;
    char* map_faces_culled;
}Chunk;

extern void Chunk_Cull(Chunk* chunk);
extern void Chunk_Cull_Clear(Chunk* chunk);
extern void Chunk_Generate(Chunk* chunk);
extern Chunk* Chunk_New();
extern void Chunk_Free(Chunk* chunk);
extern void Chunk_Render(Chunk* chunk);

extern int Chunk_Index(int x, int y, int z);
extern int Chunk_Index_Face(int x, int y, int z, int p);

#endif
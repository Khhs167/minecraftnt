#include "chunk.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <GL/gl.h>

#include <GL/glu.h>

#include <GL/freeglut.h>
#include "vectors.h"
#include "perlin.h"
#include "voxelrenderer.h"

int Chunk_Index(int x, int y, int z) { 
    return (z * MC_CHUNK_WIDTH * MC_CHUNK_HEIGHT) + (y * MC_CHUNK_WIDTH) + x; 
}

int Chunk_Index_Face(int x, int y, int z, int p) { 
    return (x + y * MC_CHUNK_WIDTH + z * MC_CHUNK_WIDTH * MC_CHUNK_HEIGHT + p * MC_CHUNK_HEIGHT * MC_CHUNK_WIDTH * 6);
}

void Chunk_Render(Chunk* chunk){
    glBegin(GL_QUADS);
    for (int x = 0; x < MC_CHUNK_WIDTH; x++)
    {
        for (int z = 0; z < MC_CHUNK_WIDTH; z++)
        {
            for (int y = 0; y < MC_CHUNK_HEIGHT; y++)
            {
                int i = Chunk_Index(x, y, z);
                if (chunk->map[i] > 0)
                {
                    for (int p = 0; p < 6; p++)
                    {
                        int pi = Chunk_Index_Face(x, y, z, p);
                        if (chunk->map_faces_culled[pi] == MC_TRUE)
                        {
                            SetFaceColour(117, 168, 80, p);
                            RenderFace(x, y, -z - 5, p);
                        }
                    }

                }
            }
        }
    }
    glEnd();
}

void Chunk_Cull(Chunk* chunk){
    printf("Culling chunk\n");
    Chunk_Cull_Clear(chunk);
    for (int x = 0; x < MC_CHUNK_WIDTH; x++)
    {
        for (int z = 0; z < MC_CHUNK_WIDTH; z++)
        {
            for (int y = 0; y < MC_CHUNK_HEIGHT; y++)
            {
                int i = Chunk_Index(x, y, z);
                if(chunk->map[i] > 0){
                    for (int p = 0; p < 6; p++)
                    {
                        Vector3 *n = face_normals[p];
                        int tx = n->x + x;
                        int ty = n->y + y;
                        int tz = n->z + z;
                        if (inside_map(tx, ty, tz))
                        {
                            int ti = Chunk_Index(tx, ty, tz);
                            if (chunk->map[ti] <= 0)
                            {
                                int pi = Chunk_Index_Face(x, y, z, p);
                                chunk->map_faces_culled[pi] = MC_TRUE;
                            }
                        }
                    }
                }
            }
        }
    }
}

void Chunk_Cull_Clear(Chunk* chunk){
    for (int x = 0; x < MC_CHUNK_WIDTH; x++)
    {
        for (int z = 0; z < MC_CHUNK_WIDTH; z++)
        {
            for (int y = 0; y < MC_CHUNK_HEIGHT; y++){
                for (int p = 0; p < 6; p++)
                {
                    int i = Chunk_Index_Face(x, y, z, p);
                    chunk->map_faces_culled[i] = MC_FALSE;
                }
            }
        }
    }
}

void Chunk_Generate(Chunk* chunk){
    for (int x = 0; x < MC_CHUNK_WIDTH; x++)
    {
        for (int z = 0; z < MC_CHUNK_WIDTH; z++)
        {
            float height = 1 + Perlin_Get2d(x / 10.0f, z / 10.0f, 1, 1) * 10;
            for (int y = 0; y < MC_CHUNK_HEIGHT; y++)
            {
                if (y <= height)
                {
                    int i = Chunk_Index(x, y, z);
                    chunk->map[i] = 1;
                }
            }
        }
    }

    Chunk_Cull(chunk);
}

void Chunk_Free(Chunk* chunk){
    free(chunk->map);
    free(chunk->map_faces_culled);
    free(chunk);
}

Chunk* Chunk_New(){
    Chunk* chunk =  malloc(sizeof(Chunk));
    chunk->map = (int*)malloc(MC_CHUNK_WIDTH * MC_CHUNK_WIDTH * MC_CHUNK_HEIGHT * sizeof(int*));
    chunk->map_faces_culled = (char*)calloc(MC_CHUNK_WIDTH * MC_CHUNK_WIDTH * MC_CHUNK_HEIGHT * 6, sizeof(char*));
    Chunk_Cull_Clear(chunk);
    return chunk;
}
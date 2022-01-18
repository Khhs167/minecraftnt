#include "voxelrenderer.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <GL/gl.h>

#include <GL/glu.h>

#include <GL/freeglut.h>

#include "mc_const.h"

void RenderFace(float x, float y, float z, int face)
{
    switch (face)
    {
    case MC_FACE_FRONT:
        glVertex3f(x + 1, y, z);
        glVertex3f(x + 1, y + 1, z);
        glVertex3f(x, y + 1, z);
        glVertex3f(x, y, z);
        break;
    case MC_FACE_BACK:
        glVertex3f(x, y, z + 1);
        glVertex3f(x, y + 1, z + 1);
        glVertex3f(x + 1, y + 1, z + 1);
        glVertex3f(x + 1, y, z + 1);
        break;
    case MC_FACE_RIGHT:
        glVertex3f(x + 1, y, z + 1);
        glVertex3f(x + 1, y + 1, z + 1);
        glVertex3f(x + 1, y + 1, z);
        glVertex3f(x + 1, y, z);

        break;
    case MC_FACE_LEFT:
        glVertex3f(x, y, z);
        glVertex3f(x, y + 1, z);
        glVertex3f(x, y + 1, z + 1);
        glVertex3f(x, y, z + 1);
        break;
    case MC_FACE_TOP:
        glVertex3f(x, y + 1, z);
        glVertex3f(x + 1, y + 1, z);
        glVertex3f(x + 1, y + 1, z + 1);
        glVertex3f(x, y + 1, z + 1);
        break;
    case MC_FACE_BOTTOM:
        glVertex3f(x, y, z + 1);
        glVertex3f(x + 1, y, z + 1);
        glVertex3f(x + 1, y, z);
        glVertex3f(x, y, z);
        break;
    default:
        printe("Unable to render face %i\n", face);
        break;
    }
}

void SetFaceColour(u_char r, u_char g, u_char b, int face)
{
    switch (face)
    {
    case MC_FACE_FRONT:
        glColor3ub(r * 0.9f, g * 0.9f, b * 0.9f);
        break;
    case MC_FACE_BACK:
        glColor3ub(r * 0.25f, g * 0.25f, b * 0.25f);
        break;
    case MC_FACE_RIGHT:
        glColor3ub(r * 0.75f, g * 0.75f, b * 0.75f);
        break;
    case MC_FACE_LEFT:
        glColor3ub(r * 0.5f, g * 0.5f, b * 0.5f);
        break;
    case MC_FACE_TOP:
        glColor3ub(r, g, b);
        break;
    case MC_FACE_BOTTOM:
        glColor3ub(r * 0.1f, g * 0.1f, b * 0.1f);
        break;
    default:
        printe("Invalid face %i\n", face);
        break;
    }
}
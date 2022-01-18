#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <GL/gl.h>

#include <GL/glu.h>

#include <GL/freeglut.h>
#include "chunk.h"

#include "mc_const.h"

char keysDown[512] = {MC_FALSE};
int frame = 0, time, timebase = 0;

Vector3 *camera_position;
float camera_rotation = 0.0f;

/* 
Face orders:
Front
Back
Right
Left
Up
Down
aas
*/

Vector3 *face_normals[6];

#pragma region CubeRendering



void RenderCube(float x, float y, float z, u_char r, u_char g, u_char b)
{
    glBegin(GL_QUADS);
    for (int i = 0; i < 6; i++)
    {
        SetFaceColour(r, g, b, i);
        RenderFace(x, y, z, i);
    }
    glEnd();
}

#pragma endregion CubeRendering

Chunk* chunk;

#pragma region Keyboard

#pragma region KeyboardManager

void normalKeyDown(unsigned char key, int x, int y)
{
    keysDown[key] = MC_TRUE;
}

void normalKeyUp(unsigned char key, int x, int y)
{
    keysDown[key] = MC_FALSE;
}

void specialKeyDown(int key, int x, int y)
{
    keysDown[key + 256] = MC_TRUE;
}

void specialKeyUp(int key, int x, int y)
{
    keysDown[key + 256] = MC_FALSE;
}

#pragma endregion KeyboardManager

void processKeyboardInput(unsigned char key, unsigned char special)
{
    float lx = sin(camera_rotation);
    float lz = -cos(camera_rotation);

    float inputSpeed = 0.05f;
    if (!special)
    {
        if (key == 'd')
            camera_rotation += inputSpeed * 0.5f;
        if (key == 'a')
            camera_rotation -= inputSpeed * 0.5f;
        if (key == 'w')
        {
            camera_position->x += lx * inputSpeed;
            camera_position->z += lz * inputSpeed;
        }
        if (key == 's')
        {
            camera_position->x -= lx * inputSpeed;
            camera_position->z -= lz * inputSpeed;
        }

        if (key == ' ')
        {
            camera_position->y += inputSpeed;
        }

        if (key == 27)
            exit(0);
    }
    else
    {
        if (key == GLUT_KEY_CTRL_L)
            camera_position->y -= inputSpeed;
    }
}

#pragma endregion Keyboard

void render()
{

    frame++;
    time = glutGet(GLUT_ELAPSED_TIME);
    if (time - timebase > 1000)
    {
        printf("FPS: %4.2f\n",
               frame * 1000.0 / (time - timebase));
        timebase = time;
        frame = 0;
    }

    glClearColor(0.54f, 0.76f, 0.87f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    float lx = sin(camera_rotation);
    float lz = -cos(camera_rotation);

    gluLookAt(
        camera_position->x, camera_position->y, camera_position->z,
        camera_position->x + lx, camera_position->y, camera_position->z + lz,
        0.0f, 1.0f, 0.0f);

    Chunk_Render(chunk);

    glutSwapBuffers();

    for (int i = 0; i < 512; i++)
    {
        if (keysDown[i])
            processKeyboardInput(i, i > 255);
    }
}

void resize(int width, int height)
{
    // Prevent a divide by zero, when window is too short
    // (you cant make a window of zero width).
    if (height == 0)
        height = 1;
    float ratio = 1.0 * width / height;

    // Use the Projection Matrix
    glMatrixMode(GL_PROJECTION);

    // Reset Matrix
    glLoadIdentity();

    // Set the viewport to be the entire window
    glViewport(0, 0, width, height);

    // Set the correct perspective.
    gluPerspective(45, ratio, 1, 1000);

    // Get Back to the Modelview
    glMatrixMode(GL_MODELVIEW);
}


void unload()
{
    free(camera_position);
    Chunk_Free(chunk);
    printf("Bye Bye!\n");
}

int main(int argc, char **argv)
{

    camera_position = Vector3_new(0.0, 0.0, 10.0);

    /* 
Face orders:
Front
Back
Right
Left
Up
Down

*/

    face_normals[0] = Vector3_new(0.0, 0.0, 1.0);
    face_normals[1] = Vector3_new(0.0, 0.0, -1.0);
    face_normals[2] = Vector3_new(1.0, 0.0, 0.0);
    face_normals[3] = Vector3_new(-1.0, 0.0, 0.0);
    face_normals[4] = Vector3_new(0.0, 1.0, 0.0);
    face_normals[5] = Vector3_new(0.0, -1.0, 0.0);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(320, 180);
    glutInitWindowSize(MC_WINDOW_SIZE_X, MC_WINDOW_SIZE_Y);
    glutCreateWindow("Minecraftn't");

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glCullFace(GL_FRONT);

    chunk = Chunk_New();
    Chunk_Generate(chunk);

    glutDisplayFunc(render);
    glutIdleFunc(render);

    glutKeyboardFunc(normalKeyDown);
    glutKeyboardUpFunc(normalKeyUp);
    glutSpecialFunc(specialKeyDown);
    glutSpecialUpFunc(specialKeyUp);

    glutReshapeFunc(resize);

    glutCloseFunc(unload);

    glutMainLoop();
    return 0;
}
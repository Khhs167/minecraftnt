#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <GL/gl.h>

#include <GL/glu.h>

#include <GL/freeglut.h>

#include "perlin.h"

#define printe(args...) fprintf(stderr, ##args); exit(-1);

#define MC_WINDOW_SIZE_X (1280)
#define MC_WINDOW_SIZE_Y (720)

#define MC_FACE_FRONT  (0)
#define MC_FACE_BACK   (1)
#define MC_FACE_RIGHT  (2)
#define MC_FACE_LEFT   (3)
#define MC_FACE_TOP    (4)
#define MC_FACE_BOTTOM (5)

#define MC_CHUNK_WIDTH (32)
#define MC_CHUNK_HEIGHT (256)


char keysDown[512] = { GL_FALSE };
int frame=0,time,timebase=0;

typedef struct {
    float x;
    float y;
    float z;
} Vector3;

Vector3* Vector3_new(float x, float y, float z){
    Vector3* v = malloc(sizeof(Vector3));
    v->x = x;
    v->y = y;
    v->z = z;
    return v;
}

Vector3* camera_position;
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

Vector3* face_normals[6];

#pragma region CubeRendering

void RenderFace(float x, float y, float z, int face){
    switch(face){
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
            printe("Invalid face %i\n", face);
            break;
    }
}

void SetFaceColour(u_char r, u_char g, u_char b, int face){
    switch(face){
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

void RenderCube(float x, float y, float z, u_char r, u_char g, u_char b){
    glBegin(GL_QUADS);
        for(int i = 0; i < 6; i++){
            SetFaceColour(r, g, b, i);
            RenderFace(x, y, z, i);
        }
    glEnd();
}

#pragma endregion CubeRendering

int map[MC_CHUNK_WIDTH][MC_CHUNK_WIDTH][256] = { { { 0 }}};

char inside_map(float x, float y, float z){
    if(x < 0 || x >= MC_CHUNK_WIDTH)
        return GL_FALSE;
    if(z < 0 || z >= MC_CHUNK_WIDTH)
        return GL_FALSE;
    if(y < 0 || y >= MC_CHUNK_HEIGHT)
        return GL_FALSE;
    return GL_TRUE;
}

void render(){

    frame++;
	time=glutGet(GLUT_ELAPSED_TIME);
	if (time - timebase > 1000) {
		printf("FPS: %4.2f\n",
			frame*1000.0/(time-timebase));
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
    glBegin(GL_QUADS);
    for(int x = 0; x < MC_CHUNK_WIDTH; x++){
        for(int z = 0; z < MC_CHUNK_WIDTH; z++){
            for(int y = 0; y < MC_CHUNK_HEIGHT; y++){
                if(map[x][z][y] > 0){
                    for(int p = 0; p < 6; p++){
                        Vector3* n = face_normals[p];
                        int tx = n -> x + x;
                        int ty = n -> y + y;
                        int tz = n -> z + z;
                        if(inside_map(tx, ty, tz)){
                            if(map[tx][tz][ty] <= 0){
                                SetFaceColour(117, 168, 80, p);
                                RenderFace(x, y, -z - 5, p);
                            }
                        }
                    }
                }
            }
        }
    }
    glEnd();

    

    glutSwapBuffers();

    for(int i = 0; i < 512; i++){
        if(keysDown[i])
            processKeyboardInput(i, i > 255);
    }
}

void GenerateMap(){

    for(int x = 0; x < MC_CHUNK_WIDTH; x++){
        for(int z = 0; z < MC_CHUNK_WIDTH; z++){
            float height = 1 + Perlin_Get2d(x / 10.0f, z / 10.0f, 1, 1) * 10;
            for(int y = 0; y < MC_CHUNK_HEIGHT; y++){
                if(y <= height){
                    map[x][z][y] = 1;
                }
            }
        }
    }

}

void resize(int width, int height) {
    // Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if(height == 0)
		height = 1;
	float ratio = 1.0* width / height;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

    // Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, width, height);

	// Set the correct perspective.
	gluPerspective(45,ratio,1,1000);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}

#pragma region Keyboard

void normalKeyDown(unsigned char key, int x, int y) {   
    keysDown[key] = GL_TRUE;
}

void normalKeyUp(unsigned char key, int x, int y) {   
    keysDown[key] = GL_FALSE;
}

void specialKeyDown(unsigned char key, int x, int y) {   
    keysDown[key + 256] = GL_TRUE;
}

void specialKeyUp(unsigned char key, int x, int y) {   
    keysDown[key + 256] = GL_FALSE;
}

#pragma endregion Keyboard

void processKeyboardInput(unsigned char key, unsigned char special){
    float lx = sin(camera_rotation);
    float lz = -cos(camera_rotation);

    float inputSpeed = 0.05f;
    if(!special){
        if (key == 'd')
            camera_rotation += inputSpeed * 0.5f;
        if (key == 'a')
            camera_rotation -= inputSpeed * 0.5f;
        if (key == 'w'){
            camera_position->x += lx * inputSpeed;
            camera_position->z += lz * inputSpeed;
        }
        if (key == 's'){
            camera_position->x -= lx * inputSpeed;
            camera_position->z -= lz * inputSpeed;
        }

        if (key == ' '){
            camera_position->y += inputSpeed;
        }

        if (key == 27)
            exit(0);
    } else{
        if (key == GLUT_KEY_CTRL_L)
            camera_position->y -= inputSpeed;
    }
}


void unload(){
    free(camera_position);
    printf("Bye Bye!\n");
    exit(0);
}

int main(int argc, char **argv){

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

    GenerateMap();

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
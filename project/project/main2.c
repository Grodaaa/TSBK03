// gcc main2.c ../common/*.c ../common/*.m -framework OpenGL -framework Cocoa -o main2 -I../common

#include <OpenGL/gl3.h>
#include <stdlib.h>
#include "MicroGlut.h"

#include "LoadTGA.h"
#include "VectorUtils3.h"
#include "GL_utilities.h"
#include "loadobj.h"
#include "zpr.h"

#define W 512
#define H 512

#define MAX_PARTICLES 10000

typedef struct Particle{
    vec3 position;
    mat4 mMatrix;
    //vec3 colour;
    //float size;
    //int life;
    //float fade;
} Particle;

Particle par_sys[MAX_PARTICLES];

void onTimer(int value);

mat4 projectionMatrix, viewMatrix, rotateMatrix, modelViewMatrix;

GLfloat particlePositionData[MAX_PARTICLES * 4 * 4];

Point3D cam, point;
GLuint shader = 0;
GLuint elapsedTimeUniform;

GLuint vertexArray = 0;
GLuint billBoard = 0;
GLuint particlePositionBuffer = 0;
float weight = 0;


float randPos[MAX_PARTICLES];

GLfloat vertices[] =
                    { 
                        0.03f, 0.03f, 0.0f, 1.0f,
                        0.03f, -0.03f, 0.0f, 1.0f,
                        -0.03f, -0.03f, 0.0f, 1.0f,
                    };

void initParticleSystem()
{
    srand((unsigned int)time(NULL));

    int i;
    float max = 2, max2 = 1;
    for(i = 0; i < MAX_PARTICLES; i++)
    {
    	par_sys[i].mMatrix = IdentityMatrix();

    	par_sys[i].mMatrix.m[3] = max * ((float)rand()/(float)RAND_MAX - 0.5);  //x
        par_sys[i].mMatrix.m[7] = max2 * ((float)rand()/(float)RAND_MAX - 0.1); //y
        par_sys[i].mMatrix.m[11] = max * ((float)rand()/(float)RAND_MAX - 0.5); //z

        par_sys[i].position = SetVector(par_sys[i].mMatrix.m[3], par_sys[i].mMatrix.m[7], par_sys[i].mMatrix.m[11]);

        particlePositionData[16 * i + 0] = par_sys[i].mMatrix.m[0]; 	//0,0
        particlePositionData[16 * i + 1] = par_sys[i].mMatrix.m[4]; 	//0,1
        particlePositionData[16 * i + 2] = par_sys[i].mMatrix.m[8]; 	//0,2
        particlePositionData[16 * i + 3] = par_sys[i].mMatrix.m[12]; 	//0,3
        particlePositionData[16 * i + 4] = par_sys[i].mMatrix.m[1]; 	//1,0
        particlePositionData[16 * i + 5] = par_sys[i].mMatrix.m[5]; 	//1,1
        particlePositionData[16 * i + 6] = par_sys[i].mMatrix.m[9]; 	//1,2
        particlePositionData[16 * i + 7] = par_sys[i].mMatrix.m[13]; 	//1,3
        particlePositionData[16 * i + 8] = par_sys[i].mMatrix.m[2]; 	//2,0
        particlePositionData[16 * i + 9] = par_sys[i].mMatrix.m[6]; 	//2,1
        particlePositionData[16 * i + 10] = par_sys[i].mMatrix.m[10]; 	//2,2
        particlePositionData[16 * i + 11] = par_sys[i].mMatrix.m[14]; 	//2,3
        particlePositionData[16 * i + 12] = par_sys[i].mMatrix.m[3]; 	//3,0
        particlePositionData[16 * i + 13] = par_sys[i].mMatrix.m[7]; 	//3,1
        particlePositionData[16 * i + 14] = par_sys[i].mMatrix.m[11]; 	//3,2
        particlePositionData[16 * i + 15] = par_sys[i].mMatrix.m[15]; 	//3,3

    }

}

void initDrawParticles()
{
	shader = loadShaders("shader.vert", "shader.frag");
	elapsedTimeUniform = glGetUniformLocation(shader, "time");

    static const GLfloat vertexBufferData[] =
	{
		-1.0f , -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
	};

	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);

	//add billboard, its static since it wont change
	glGenBuffers(1, &billBoard);
	glBindBuffer(GL_ARRAY_BUFFER, billBoard);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBufferData), vertexBufferData, GL_STATIC_DRAW);

	//Prepare for position buffers
	glGenBuffers(1, &particlePositionBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, particlePositionBuffer);
	glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES*4*4*sizeof(GLfloat), NULL, GL_STREAM_DRAW);

	glBindVertexArray(0);

	glUseProgram(shader);

	GLuint lolmat = glGetUniformLocation(shader, "MVP");
	glUniformMatrix4fv(lolmat, 1, GL_FALSE, modelViewMatrix.m);
}

void display()
{
    printError("pre display");

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    printError("display");

    glUseProgram(shader);

    glUniform1f(elapsedTimeUniform, glutGet(GLUT_ELAPSED_TIME) / 1000.0f);

    glBindBuffer(GL_ARRAY_BUFFER, particlePositionBuffer);
    glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * 4 * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, MAX_PARTICLES * sizeof(GLfloat) * 4 * 4, particlePositionData);

    glBindVertexArray(vertexArray);

    glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, billBoard);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	printError("ey lol");

    int pos = glGetAttribLocation(shader, "transformmatrix"); 

	glEnableVertexAttribArray(pos);
	glEnableVertexAttribArray(pos + 1);
	glEnableVertexAttribArray(pos + 2);
	glEnableVertexAttribArray(pos + 3);
	glBindBuffer(GL_ARRAY_BUFFER, particlePositionBuffer);

	glVertexAttribPointer(pos, 4, GL_FLOAT,  GL_FALSE, sizeof(GLfloat)*4*4, (void*)(0));
	glVertexAttribPointer(pos + 1, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*4*4, (void*)(sizeof(float)*4));
	glVertexAttribPointer(pos + 2, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*4*4, (void*)(sizeof(float)*8));
	glVertexAttribPointer(pos + 3, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*4*4, (void*)(sizeof(float)*12));

	glVertexAttribDivisor(0, 0); // particles vertices : always reuse the same 4 vertices -> 0
    glVertexAttribDivisor(pos, 1);
	glVertexAttribDivisor(pos + 1, 1);
	glVertexAttribDivisor(pos + 2, 1);
	glVertexAttribDivisor(pos + 3, 1);

	//glVertexAttribPointer(glGetAttribLocation(shader, "position"), 3, GL_FLOAT, GL_FALSE, 0, 0); 
    //glEnableVertexAttribArray(glGetAttribLocation(shader, "position"));

    //glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, MAX_PARTICLES);
    glDrawArraysInstanced(GL_POINTS, 0, 4, MAX_PARTICLES);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(pos);
	glDisableVertexAttribArray(pos + 1);
	glDisableVertexAttribArray(pos + 2);
	glDisableVertexAttribArray(pos + 3);

	glBindVertexArray(0);

    glutSwapBuffers();
    glutPostRedisplay();
}
void onTimer(int value)
{
    glutPostRedisplay();
    glutTimerFunc(5, &onTimer, value);
}

void reshape(GLsizei w, GLsizei h)
{
    glViewport(0, 0, w, h);
    GLfloat ratio = (GLfloat) w / (GLfloat) h;
    projectionMatrix = perspective(90, ratio, 1.0, 1000);
}
void key(unsigned char key, int x, int y)
{
        printf("weight lol = %f\n", weight);

    switch (key)
    {
        case 27: exit(0); 
            break;

        case GLUT_KEY_LEFT: weight -= 1.0;
                break;
        case GLUT_KEY_RIGHT: weight += 1.0; 
            break;
  }
}
int main(int argc, char *argv[])
{
   	glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitContextVersion(3, 2);
    glutInitWindowSize(W, H);
    glutCreateWindow ("Project");

    glutDisplayFunc(display);

    glutTimerFunc(5, &onTimer, 0);
    glutReshapeFunc(reshape);
    glutSpecialFunc(key);

    initParticleSystem();
    initDrawParticles();


    zprInit(&viewMatrix, cam, point);

    glutMainLoop();

    return 0;
}
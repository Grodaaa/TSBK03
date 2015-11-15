/*
TSBK03 Slutprojekt - Snösimulering
Kristina Engström
Ellen Häger
Handledare: Ingemar Ragnemalm
*/

// gcc main.c ../common/*.c ../common/*.m -framework OpenGL -framework Cocoa -o main -I../common

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

//Sätter antal partiklar
#define MAX_PARTICLES 10000

typedef struct Particle
{
    mat4 mMatrix; //Matris med random positioner
}Particle;

Particle par_sys[MAX_PARTICLES]; //Skapar partikelsystem

void onTimer(int value);

mat4 projectionMatrix, viewMatrix, rotateMatrix, modelViewMatrix;

GLfloat particlePositionData[MAX_PARTICLES * 4 * 4]; //Buffer med positioninfo

Point3D cam, point;
GLuint shader = 0;
GLuint elapsedTimeUniform;

//Variabler faktorerna denistet samt temperatur
double density = 0.7;
GLuint density_temp;

double temperature = -10;
GLuint temp_temp;

//Sätter storleken på partiklarna till 1
double size = 1;

GLuint vertexArray = 0;
GLuint particlePositionBuffer = 0;

void initParticleSystem()
{
    srand((unsigned int)time(NULL));

    int i;
    float max = 2, max2 = 1;
    for(i = 0; i < MAX_PARTICLES; i++)
    {
    	par_sys[i].mMatrix = IdentityMatrix();

        //Randomiserar positioner i x- och y-led.
    	par_sys[i].mMatrix.m[3] = max * ((float)rand()/(float)RAND_MAX - 0.5);  //x
        par_sys[i].mMatrix.m[7] = max2 * ((float)rand()/(float)RAND_MAX - 0.1); //y

        //Lägger in positionsdatan inför positionsbuffern.
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
    //Laddar shadern
	shader = loadShaders("shader.vert", "shader.frag");

    //Genererar vertex array object
	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);

	//Genrerar en buffer för positionerna
	glGenBuffers(1, &particlePositionBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, particlePositionBuffer);
	glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES*4*4*sizeof(GLfloat), NULL, GL_STREAM_DRAW);

	glBindVertexArray(0);

    //Aktiverar shader
	glUseProgram(shader);

    //Laddar in uniformvariabler till vertexshadern
    elapsedTimeUniform = glGetUniformLocation(shader, "time");

    density_temp = glGetUniformLocation(shader, "density");
    glUniform1f(density_temp, density);

    temp_temp = glGetUniformLocation(shader, "temperature");
    glUniform1f(temp_temp, temperature);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shader);

    //Hämtar tidsvariabeln
    glUniform1f(elapsedTimeUniform, glutGet(GLUT_ELAPSED_TIME) / 1000.0f);

    //Binder partikeldata till partikelbuffern
    glBindBuffer(GL_ARRAY_BUFFER, particlePositionBuffer);
    glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * 4 * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, MAX_PARTICLES * sizeof(GLfloat) * 4 * 4, particlePositionData);

    //Binder vertex array object
    glBindVertexArray(vertexArray);

    //Laddar attributdata
    int pos = glGetAttribLocation(shader, "transformmatrix"); 

	glEnableVertexAttribArray(pos);
	glEnableVertexAttribArray(pos + 1);
	glEnableVertexAttribArray(pos + 2);
	glEnableVertexAttribArray(pos + 3);
	glBindBuffer(GL_ARRAY_BUFFER, particlePositionBuffer);

	glVertexAttribPointer(pos    , 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*4*4, (void*)(0));
	glVertexAttribPointer(pos + 1, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*4*4, (void*)(sizeof(float)*4));
	glVertexAttribPointer(pos + 2, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*4*4, (void*)(sizeof(float)*8));
	glVertexAttribPointer(pos + 3, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*4*4, (void*)(sizeof(float)*12));

	//Partikel vertexar: always reuse the same 4 vertices -> 0
    glVertexAttribDivisor(0, 0);
    glVertexAttribDivisor(pos    , 1);
	glVertexAttribDivisor(pos + 1, 1);
	glVertexAttribDivisor(pos + 2, 1);
	glVertexAttribDivisor(pos + 3, 1);

    //Tillåter förändring av storlek på GL_POINTS
    glEnable(GL_PROGRAM_POINT_SIZE);

    //Ritar partiklarna
    glDrawArraysInstanced(GL_POINTS, 0, 4, MAX_PARTICLES);

    //Rensar data
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

//Funktion för att ändra olika faktorer som ska påverka partiklarna.
void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27: exit(0); 
            break;

        //Ändrar densiteten
        case GLUT_KEY_LEFT: density -= 0.01;
             if(density <= 0.0)
             {
                density = 0.0;
                printf("Density: %f\n", density);
                printf("Density can't go lower than 0.0 %f\n", NULL);
             }
             glUniform1f(density_temp, density);
             printf("Density: %f\n", density);
                break;
        case GLUT_KEY_RIGHT: density += 0.01; 
             glUniform1f(density_temp, density);
             printf("Density: %f\n", density);
                break;

        //Ändrar temperaturen
        case GLUT_KEY_UP: temperature += 1.0;
             if(temperature >= 0.0)
             {
                temperature = -1.0;
                printf("Temperature: %f\n", temperature);
                printf("Temperature can't go higher than -1 %f\n", NULL);
             }
             glUniform1f(temp_temp, temperature);
             printf("Temperature: %f\n", temperature);
                break;
        case GLUT_KEY_DOWN: temperature -= 1.0;
             if(temperature < -10.0)
             {
                temperature = -10.0;
                printf("Temperature: %f\n", temperature);
                printf("Temperature can't go lowe than -10 %f\n", NULL);
             }
             glUniform1f(temp_temp, temperature);
             printf("Temperature: %f\n", temperature);
                break;

        //Ändrar storlek på GL_POINTS
        case GLUT_KEY_F2: size += 0.2;
             glPointSize(size);
             printf("Snowsize: %f\n", size);
                break;
        case GLUT_KEY_F1: size -= 0.2;
             glPointSize(size);
             printf("Snowsize: %f\n", size);
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

    //Startar tidsfunktionen
    glutTimerFunc(5, &onTimer, 0);
    //Startar knappfunktionen 
    glutSpecialFunc(key);
    //Initialiserar partikelsystemet
    initParticleSystem();
    //Initialiserar ritandet av partiklar
    initDrawParticles();

    zprInit(&viewMatrix, cam, point);

    glutMainLoop();
}
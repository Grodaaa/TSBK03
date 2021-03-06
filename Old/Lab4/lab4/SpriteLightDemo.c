// Demo of heavily simplified sprite engine
// by Ingemar Ragnemalm 2009
// used as base for lab 4 in TSBK03.
// OpenGL 3 conversion 2013.

// gcc SpriteLightDemo.c SpriteLight.c ../common/*.c ../common/*.m -framework OpenGL -framework Cocoa -o SpriteLightDemo -I../common

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	#include "MicroGlut.h"
	// uses framework Cocoa
#else
	#include <GL/gl.h>
	#include "MicroGlut.h"
#endif

#include <stdlib.h>
#include "LoadTGA.h"
#include "SpriteLight.h"
#include "GL_utilities.h"
#include <math.h>

// L�gg till egna globaler h�r efter behov.


void SpriteBehavior() // Din kod!
{	
// L�gg till din labbkod h�r. Det g�r bra att �ndra var som helst i
// koden i �vrigt, men mycket kan samlas h�r. Du kan utg� fr�n den
// globala listroten, gSpriteRoot, f�r att kontrollera alla sprites
// hastigheter och positioner, eller arbeta fr�n egna globaler.

	SpritePtr agent, otherAgent;
	float maxDist = 300, maxSpeed = 1.0, alignmentWeight = 0.7, cohesionWeight = 0.5;

	agent = gSpriteRoot;
	do
	{
		float counter = 0;

		otherAgent = gSpriteRoot;
		do
		{
			if(agent != otherAgent)
			{
				float dist = sqrt((otherAgent->position.h*otherAgent->position.h) - (agent->position.h*agent->position.h) + 
							 	  (otherAgent->position.v*otherAgent->position.v) - (agent->position.v*agent->position.v));
				if(dist < maxDist)
				{
					agent->speedDiff.h += otherAgent->speed.h - agent->speed.h;
					agent->speedDiff.v += otherAgent->speed.v - agent->speed.v;

					agent->avgPos.h += otherAgent->position.h;
					agent->avgPos.v += otherAgent->position.v;
					
					counter++;
				}
			}
			otherAgent = otherAgent->next;
		}while(otherAgent != NULL);

		if(counter > 0)
		{
			agent->speedDiff.h /= counter;
			agent->speedDiff.v /= counter;

			agent->avgPos.h /= counter;
			agent->avgPos.v /= counter;

			agent->speed.h += agent->speedDiff.h * alignmentWeight + (agent->avgPos.h - agent->position.h) * cohesionWeight;
			agent->speed.v += agent->speedDiff.v * alignmentWeight + (agent->avgPos.v - agent->position.v) * cohesionWeight;
		}

		float length = sqrt((agent->speed.h * agent->speed.h) + (agent->speed.v * agent->speed.v));
		if(length > maxSpeed)
		{
			agent->speed.h = (agent->speed.h/length)*maxSpeed;
			agent->speed.v = (agent->speed.v/length)*maxSpeed;
		}

		agent->position.h += agent->speed.h;
		agent->position.v += agent->speed.v;

		agent = agent->next;
	}while(agent != NULL);
}

// Drawing routine
void Display()
{
	SpritePtr sp;
	
	glClearColor(0, 0, 0.2, 1);
	glClear(GL_COLOR_BUFFER_BIT+GL_DEPTH_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	DrawBackground();
	
	SpriteBehavior(); // Din kod!
	
// Loop though all sprites. (Several loops in real engine.)
	sp = gSpriteRoot;
	float idNUM = 0;
	do
	{
		HandleSprite(sp); // Callback in a real engine
		DrawSprite(sp);
		
		sp = sp->next;
	} while (sp != NULL);
	
	glutSwapBuffers();
}

void Reshape(int h, int v)
{
	glViewport(0, 0, h, v);
	gWidth = h;
	gHeight = v;
}

void Timer(int value)
{
	glutTimerFunc(20, Timer, 0);
	glutPostRedisplay();
}

// Example of user controllable parameter
float someValue = 0.0;

void Key(unsigned char key,
         __attribute__((unused)) int x,
         __attribute__((unused)) int y)
{
  switch (key)
  {
    case '+':
    	someValue += 0.1;
    	printf("someValue = %f\n", someValue);
    	break;
    case '-':
    	someValue -= 0.1;
    	printf("someValue = %f\n", someValue);
    	break;
    case 0x1b:
      exit(0);
  }
}

void Init()
{
	TextureData *sheepFace, *blackFace, *dogFace, *foodFace;
	
	LoadTGATextureSimple("bilder/leaves.tga", &backgroundTexID); // Bakgrund
	
	sheepFace = GetFace("bilder/sheep.tga"); // Ett f�r
	blackFace = GetFace("bilder/blackie.tga"); // Ett svart f�r
	dogFace = GetFace("bilder/dog.tga"); // En hund
	foodFace = GetFace("bilder/mat.tga"); // Mat
	
	NewSprite(blackFace, 100, 200, 1, 1);
	NewSprite(blackFace, 200, 100, 1.5, -1);
	NewSprite(blackFace, 250, 200, -1, 1.5);
	/*NewSprite(blackFace, 250, 200, -1, -1.5);
	NewSprite(sheepFace, 150, 110, -1, 1.5);
	NewSprite(sheepFace, 200, 250, 1, -1.5);*/

}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(800, 600);
	glutInitContextVersion(3, 2);
	glutCreateWindow("SpriteLight demo / Flocking");
	
	glutDisplayFunc(Display);
	glutTimerFunc(20, Timer, 0); // Should match the screen synch
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Key);
	
	InitSpriteLight();
	Init();
	
	glutMainLoop();
	return 0;
}

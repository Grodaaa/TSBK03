#version 330
/*
TSBK03 Slutprojekt - Snösimulering
Kristina Engström
Ellen Häger
Handledare: Ingemar Ragnemalm
*/
//Hämtar attributdata från main.c
layout(location = 1) in vec3 color;
layout(location = 2) in mat4 transformmatrix;

//Hämtar uniformvariabler
uniform float time;
uniform float density;
uniform float temperature;

//Utdata
out vec3 Color;

void main()
{ 
  //Plockar positionsdata från matrisen
  vec4 xyzs = transformmatrix[3];
  vec3 particlePosition = xyzs.xyz;

  //Delar upp partiklarna i olika skikt så att inte alla beter sig likadant
  if(gl_InstanceID % 2 == 0)
  {
    particlePosition.y = particlePosition.y - time * 0.15 + 0.01 * gl_InstanceID;
    Color = vec3(0.0, 0.0, 0.0);
  }
  else if(gl_InstanceID % 3 == 0)
  {
    particlePosition.y = particlePosition.y - time * 0.1 + 0.001 * gl_InstanceID;
    //"Vind"
    particlePosition.x += particlePosition.y * 0.2;
    Color = vec3(0.1, 0.1, 0.1);
  }
  else
  {
    particlePosition.y = particlePosition.y - time * 0.1 + 0.04 * gl_InstanceID;
    Color = vec3(0.1, 0.1, 0.1);
  }

  //Temperatur
  float temp_new = -temperature/10;
  particlePosition.y /= temp_new;
  //Densitet
  particlePosition.y /= density;

  gl_Position = vec4(particlePosition, 1.0f);
}
#version 330
/*
TSBK03 Slutprojekt - Snösimulering
Kristina Engström
Ellen Häger
Handledare: Ingemar Ragnemalm
*/

in vec3 Color;

out vec4 outColor;

void main() 
{
    outColor = vec4(1.0 - Color, 1.0);
}

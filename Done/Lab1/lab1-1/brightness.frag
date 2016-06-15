#version 150

in vec2 outTexCoord;
uniform sampler2D texUnit;
out vec4 out_Color;


void main(void)
{
    out_Color = max(vec4(0.0),texture(texUnit, outTexCoord) - vec4(1.0));
}
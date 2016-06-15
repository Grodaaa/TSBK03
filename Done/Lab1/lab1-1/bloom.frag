#version 150

in vec2 outTexCoord;
uniform sampler2D texUnit;
uniform sampler2D texUnit2;
out vec4 fragColor;


void main(void)
{
	vec4 a = texture(texUnit, outTexCoord);
	vec4 b = texture(texUnit2, outTexCoord);

	fragColor = (0.8*a + 0.6*b);
}

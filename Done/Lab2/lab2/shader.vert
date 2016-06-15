#version 150

//in vec3 in_Color;
in vec3 in_Position;
in vec3 in_Normal;
in vec2 in_TexCoord;

uniform mat4 matrix;

uniform mat4 rotationBoneZero;
uniform mat4 rotationBoneOne;

uniform vec3 translationBoneZero;
uniform vec3 translationBoneOne;

out vec4 g_color;
const vec3 lightDir = normalize(vec3(0.3, 0.5, 1.0));

// Uppgift 3: Soft-skinning på GPU
//
// Flytta över din implementation av soft skinning från CPU-sidan
// till vertexshadern. Mer info finns på hemsidan.

void main(void)
{

	//Uppgift3

	vec3 temp1 = in_Position - translationBoneZero;
	vec4 temp1_1 = rotationBoneZero * vec4(temp1, 1.0);
	temp1 = vec3(temp1_1) + translationBoneZero;
	temp1 *= in_TexCoord.x;

	vec3 temp2 = in_Position - translationBoneOne;
	vec4 temp2_2 = rotationBoneOne * vec4(temp2, 1.0);
	temp2 = vec3(temp2_2) + translationBoneOne;
	temp2 *= in_TexCoord.y;

	vec3 namnam = temp1 + temp2;

	// transformera resultatet med ModelView- och Projection-matriserna
	gl_Position = matrix * vec4(namnam, 1.0);
	//gl_Position = matrix * vec4(in_Position, 1.0);

	// sätt röd+grön färgkanal till vertex Weights
	vec4 color = vec4(in_TexCoord.x, in_TexCoord.y, 0.0, 1.0);

	// Lägg på en enkel ljussättning på vertexarna 	
	float intensity = dot(in_Normal, lightDir);
	color.xyz *= intensity;

	g_color = color;
}


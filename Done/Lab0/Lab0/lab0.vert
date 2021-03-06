/*
 * random comment here
 * makes syntax highlight appaer
 * colors like springs sprouts
 */

#version 150

// in  vec3  in_Position;
// in  vec3  in_Normal;
// in  vec2  in_TexCoord;
// 
// uniform mat4 projectionMatrix;
// uniform mat4 viewMatrix;
// 
// out float shade;
// 
// void main(void)
// {
// 	//shade = (mat3(viewMatrix)*in_Normal).z; // Fake shading
// 	shade = inverse(transpose(mat3(viewMatrix))) * in_Normal;
// 	gl_Position=projectionMatrix*viewMatrix*vec4(in_Position, 1.0);
// }

in  vec2 in_TexCoord;
in  vec3 in_Position;
in  vec3 in_Normal;

out vec3 exNormal; // Phong
out vec3 exSurface; // Phong (specular)
out vec2 frag_texcoord;



uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main(void)
{
	exNormal = inverse(transpose(mat3(viewMatrix))) * in_Normal; // Phong, "fake" normal transformation

	exSurface = vec3(viewMatrix * vec4(in_Position, 1.0)); // Don't include projection here - we only want to go to view coordinates
  
	
	frag_texcoord = in_TexCoord;

	gl_Position = projectionMatrix * viewMatrix * vec4(in_Position, 1.0); // This should include projection
}
/*
 * random comment here
 * makes syntax highlight appaer
 * colors like springs sprouts
 */

#version 150

// in float shade;
// 
// out vec4 out_Color;
// 
// void main(void)
// {
// 	out_Color=vec4(shade,shade,shade,1.0);
// }

in vec3 exNormal; // Phong
in vec3 exSurface; // Phong (specular)
//in vec3 exSurface2; 
in vec2 frag_texcoord;
out vec4 out_Color;
uniform sampler2D exampletexture;

void main(void)
{
	const vec3 light = vec3(0.58, 0.58, 0.58); // Given in VIEW coordinates! You will usually give light in world coordinates.
	float diffuse, specular, shade;
	float shade2;
	const vec3 light2 = vec3(0.60, 0.60, 0.60);
	
	// Diffuse
	diffuse = dot(normalize(exNormal), light);
	diffuse = max(0.0, diffuse); // No negative light 
	
	// Specular
	vec3 r = reflect(-light2, normalize(exNormal));
	vec3 v = normalize(-exSurface); // View direction
	specular = dot(r, v);
	if (specular > 0.0)
		specular = 1.0 * pow(specular, 150.0);
	specular = max(specular, 0.0);
	shade = 0.7*diffuse;
	shade2 = specular;
	

	out_Color = vec4(shade, shade, shade, 1.0) + vec4(shade2, shade2, shade2, 1.0)+ texture(exampletexture, frag_texcoord);

}
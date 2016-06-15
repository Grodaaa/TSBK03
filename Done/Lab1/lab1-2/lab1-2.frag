#version 150
// bump mapping should be calculated
// 1) in view coordinates
// 2) in texture coordinates

in vec2 outTexCoord;
in vec3 out_Normal;
in vec3 Ps;
in vec3 Pt;
in vec3 pixPos;  // Needed for specular reflections
uniform sampler2D texUnit;
out vec4 out_Color;

void main(void)
{
    vec3 light = vec3(0.0, 0.7, 0.7); // Light source in view coordinates
	
	// Calculate gradients here
	float offset = 1.0 / 256.0; // texture size, same in both directions
	
    vec3 normal = normalize(out_Normal);

    //BUMP MAP VIEW, 2a
    
    vec2 step_x = vec2(1.0/512.0, 0.0);
    vec2 step_y = vec2(0.0, 1.0/512.0);
    
    //BUMP MAP DEL 1
    /*
    vec3 crossPt = cross(Pt, normal);
    vec3 crossPs = cross(normal, Ps);

    vec3 bs = vec3(texture(texUnit, (outTexCoord + step_x)) - texture(texUnit, outTexCoord));
    vec3 bt = vec3(texture(texUnit, outTexCoord) - texture(texUnit, (outTexCoord + step_y)));

    vec3 dNormal = vec3(normal + bs*crossPt + bt*crossPs); 
    dNormal = normalize(dNormal);
    out_Color = vec4( dot(dNormal, light));
    */

    //BUMP MAP DEL 2 
    
    float bs = float(texture(texUnit, (outTexCoord + step_x)) - texture(texUnit, outTexCoord));
    float bt = float(texture(texUnit, outTexCoord) - texture(texUnit, (outTexCoord + step_y)));

    vec3 dNormal = vec3(bs, bt, 1.0);
    dNormal = normalize(dNormal);

    mat3 Mvt = mat3(Ps, Pt, normal);
    Mvt = transpose(Mvt);

    vec3 L = Mvt * light;

    out_Color = vec4( dot(dNormal, L));
    

	// Simplified lighting calculation.
	// A full solution would include material, ambient, specular, light sources, multiply by texture.

    //out_Color = vec4( dot(normal, light)) * texture(texUnit, outTexCoord);
}

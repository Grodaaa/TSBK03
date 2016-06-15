#version 150

in vec2 outTexCoord;
uniform sampler2D texUnit;
out vec4 out_Color;


void main(void)
{
    vec4 sum = vec4(0.0, 0.0, 0.0, 0.0);
    vec2 step_w = vec2(1.0/512.0, 0.0);
    vec2 step_h = vec2(0.0, 1.0/512.0);

    sum += texture(texUnit,(outTexCoord + step_w + step_h))*1.0/36.0;
    sum += texture(texUnit,(outTexCoord - step_w + step_h))*1.0/36.0;
    sum += texture(texUnit,(outTexCoord + step_w - step_h))*1.0/36.0;
    sum += texture(texUnit,(outTexCoord - step_w - step_h))*1.0/36.0;
    sum += texture(texUnit,(outTexCoord + step_h))*4.0/36.0;
    sum += texture(texUnit,(outTexCoord + step_w))*4.0/36.0;
    sum += texture(texUnit,(outTexCoord - step_h))*4.0/36.0;
    sum += texture(texUnit,(outTexCoord - step_w))*4.0/36.0;
    sum += texture(texUnit,(outTexCoord))*16.0/36.0;

    out_Color = sum;

    //out_Color = texture(texUnit, outTexCoord);
}

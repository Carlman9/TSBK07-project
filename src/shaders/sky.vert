#version 150
in vec3 inPosition;
out vec3 TexCoords;


uniform mat4 projMatrix;
uniform mat4 mdlMatrix;
uniform mat4 cameraMatrix;

out float visability;

const float density = 0.0075;
const float gradient = 2;

void main()
{

vec4 World_pos = mdlMatrix * vec4(inPosition,1.0);
vec4 pos_rel_cam = cameraMatrix * World_pos;
    TexCoords = inPosition;
    float distance = length(pos_rel_cam.xyz);
  	visability = exp(-pow(distance*density,gradient));
  	visability = clamp(visability,0,1);
    gl_Position = projMatrix * cameraMatrix * mdlMatrix * vec4(inPosition, 1.0);
}

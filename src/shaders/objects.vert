#version 150

in  vec3 inPosition;
in  vec3 inNormal;
in vec2 inTexCoord;
out vec2 texCoord;
out vec3 vertNormal;
out vec3 vertSurface;

// NY
uniform mat4 projMatrix;
uniform mat4 mdlMatrix;
uniform mat4 cameraMatrix;

out float visability;

const float density = 0.007;
const float gradient = 1.5;

void main(void)
{
vec4 World_pos = mdlMatrix * vec4(inPosition,1.0);
vec4 pos_rel_cam = cameraMatrix * World_pos;
	vertNormal = inNormal;
	vertSurface = inPosition;
	texCoord = inTexCoord;
	float distance = length(pos_rel_cam.xyz);
	visability = exp(-pow(distance*density,gradient));
	visability = clamp(visability,0,1);
	gl_Position = projMatrix * cameraMatrix * mdlMatrix * vec4(inPosition, 1.0);
}

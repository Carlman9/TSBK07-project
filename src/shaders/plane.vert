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

void main(void)
{
	vertNormal = inNormal;
	vertSurface = inPosition;
	texCoord = inTexCoord;
	gl_Position = projMatrix * cameraMatrix * mdlMatrix * vec4(inPosition, 1.0);
}

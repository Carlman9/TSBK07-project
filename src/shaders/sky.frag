#version 150

out vec4 outColor;

in vec3 TexCoords;
in float visability;
vec3 skyColor = vec3(0.6,0.6,0.6);

uniform samplerCube tex_sky;

void main()
{
    outColor = texture(tex_sky, TexCoords);
    outColor = mix(vec4(skyColor,1.0),outColor, visability);
}

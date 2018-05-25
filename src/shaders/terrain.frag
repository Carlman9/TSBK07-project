#version 150

out vec4 outColor;
in vec2 texCoord;
in float visability;

uniform sampler2D tex_water;
uniform sampler2D tex_dirt2;
uniform sampler2D tex_road;
uniform sampler2D tex_sand;
uniform sampler2D tex_grass;
uniform sampler2D blendmap;
uniform vec3 cameraPos;



in vec3 vertNormal;
in vec3 vertSurface;
float attenuation;
bool isDirectional = false;
vec3 lightSourcesDirPosArr = vec3(0,200,1000);
float distanceToLight = 0;
vec3 specular = vec3(0,0,0);
vec3 surfaceToLight = vec3(0,0,0);
vec3 diffuse = vec3(0,0,0);
const vec3 light = vec3(1,0,1);
vec3 inciVec = vec3(0,0,0);
vec3 reflection = vec3(0,0,0);
vec3 surfaceToCamera = vec3(0,0,0);
float cosAngle = 0;
float specularTemp = 0;
vec3 lightSourcesColorArr = vec3(0.8,0.8,0.8);
float specularExponent =10;
float dirt_level = 0.5;
float grass_level = 3;
float sea_level = 0;

vec3 skyColor = vec3(0.99,0.99,0.99);



uniform mat4 mdlMatrix;
void main(void)
{

	mat3 normalMatrix = transpose(inverse(mat3(mdlMatrix)));
	vec3 normal = normalize(vertNormal);
	vec3 fragPos = vec3(mdlMatrix * vec4(vertSurface,1));
  lightSourcesColorArr = lightSourcesColorArr * 1;

	float k = 0.0000001;


	if (isDirectional) {
	surfaceToLight = normalize(lightSourcesDirPosArr);
	} else {
	distanceToLight = length(lightSourcesDirPosArr - fragPos);
	attenuation = 1/(1 + k*pow(distanceToLight,2));
	surfaceToLight = attenuation * normalize(lightSourcesDirPosArr - fragPos);
	}
	float temp_dif = dot(normal,surfaceToLight);
	temp_dif = max(0.0, temp_dif); // No negative light
	diffuse = temp_dif * lightSourcesColorArr;

	float ambientCoefficient = 0.001;
	vec3 ambient = ambientCoefficient * lightSourcesColorArr;

	if (isDirectional) {
	surfaceToLight = normalize(lightSourcesDirPosArr);
	} else {
	distanceToLight = length(lightSourcesDirPosArr - fragPos);
	attenuation = 1/(1 + k*pow(distanceToLight,2));
	surfaceToLight = attenuation * normalize(lightSourcesDirPosArr - fragPos);
	}
	inciVec = -surfaceToLight;
	reflection = reflect(inciVec, normal);
	surfaceToCamera = normalize(cameraPos - fragPos);
	cosAngle = max(0.0,dot(surfaceToCamera,reflection));
	float specularCoefficient = pow(cosAngle,specularExponent);
	specular = specularCoefficient * lightSourcesColorArr;

//olika texture beroende på höjd, scala mellan 0-1
float scale_d;
float scale_g;
float scale_w;
if(vertSurface.y > sea_level && vertSurface.y < dirt_level){
scale_d = (vertSurface.y-sea_level)/0.5;
scale_w = 1-(vertSurface.y-sea_level)/0.5;
scale_g = 0;
}
if(vertSurface.y >= dirt_level && vertSurface.y <= grass_level){
scale_d = 1-(vertSurface.y-dirt_level)/2.5;
scale_g = (vertSurface.y-dirt_level)/2.5;
scale_w = 0;
}

if(vertSurface.y <= sea_level){
scale_d = 0;
scale_g = 0;
scale_w = 1;
}
if(vertSurface.y > grass_level){
scale_d = 0;
scale_g = 1;
scale_w = 0;
}








  vec3 final = diffuse + ambient + specular;

	outColor = (texture(tex_grass,texCoord)*scale_g + texture(tex_water,texCoord)*scale_w + texture(tex_dirt2,texCoord)*scale_d) * vec4(final,1.0);
	outColor = mix(vec4(skyColor,1.0),outColor, visability);

//outColor = texture(tex_grass,texCoord) * scale_g * vec4(final,1.0);
}

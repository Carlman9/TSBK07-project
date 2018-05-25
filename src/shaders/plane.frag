#version 150

out vec4 outColor;
in vec2 texCoord;


uniform vec3 cameraPos;


in vec3 vertNormal;
in vec3 vertSurface;
float attenuation;
bool isDirectional = true;
vec3 lightSourcesDirPosArr = vec3(-1,1,-1);
float distanceToLight = 0;
vec3 specular = vec3(0,0,0);
vec3 surfaceToLight = vec3(0,0,0);
vec3 diffuse = vec3(0,0,0);
vec3 inciVec = vec3(0,0,0);
vec3 reflection = vec3(0,0,0);
vec3 surfaceToCamera = vec3(0,0,0);
float cosAngle = 0;
float specularTemp = 0;
vec3 lightSourcesColorArr = vec3(0.1,0.1,0.1);
float specularExponent =0.1;
float wheel = -1.3;
float low_body = -0.28;
float upper_body = -0.28;
float props = 4.5;
vec3 red = vec3(0.8,0,0.17);
vec3 white = vec3(1,1,1);
vec3 black = vec3(0.1,0.1,0.1);

uniform mat4 mdlMatrix;
void main(void)
{

	mat3 normalMatrix = transpose(inverse(mat3(mdlMatrix)));
	vec3 normal = normalize(normalMatrix*vertNormal);
	vec3 fragPos = vec3(mdlMatrix * vec4(vertSurface,1));
  lightSourcesColorArr = lightSourcesColorArr * 1;

	float k = 0.1;


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

	float ambientCoefficient = 0.5;
  float scale_r = 0;
  float scale_w = 0;
	float scale_b = 0;

	if(vertSurface.y < wheel || vertSurface.z > props){
  scale_r = 0;
	scale_w = 0;
	scale_b = 1;
}
if(vertSurface.y > wheel && vertSurface.y < low_body && vertSurface.z < props){
	scale_w = 1;
	scale_r = 0;
	scale_b = 0;
}
if(vertSurface.y > upper_body && vertSurface.z < props){
  scale_w=0;
	scale_r =1;
	scale_b =0;
}



	vec3 ambient = ambientCoefficient * red * scale_r + ambientCoefficient * white * scale_w + ambientCoefficient*black*scale_b;

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




  vec3 final = diffuse + ambient + specular;
  outColor = vec4(final,1.0);
	//outColor = vec4(final,1.0);

//outColor = texture(tex_grass,texCoord) * scale_g * vec4(final,1.0);
}

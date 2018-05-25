/*
Kontroll för att röra sig runt, flygplansrörelse ligger här
*/

#include "controller.h"

void planeController(float *engine_speed,float *elevator, float *rudder, float *right_flap, float *left_flap, bool *start, bool *init_flag, bool * collision, bool *fps){

	if (glutKeyIsDown('q') || *collision) {
		*start = false;
		*init_flag = true;
	}
	if (glutKeyIsDown('z')) {
		*fps = true;
	}
	if (glutKeyIsDown('x')) {
		*fps = false;
	}
	if (glutKeyIsDown('+') && *engine_speed < 6000){
		*engine_speed +=10;
		//printf("coord %f\n", plane_coord->x);
	}
	if (glutKeyIsDown('-') && *engine_speed > 0){
		*engine_speed -= 10;
	}
	if (glutKeyIsDown('w')){
		*elevator += 0.001;
	}
	else if (*elevator > 0) {
		*elevator -= 0.001;
	}
	if (glutKeyIsDown('s')) {
		*elevator -= 0.001;
	}
	else if (*elevator < 0) {
		*elevator += 0.001;
	}
	if (glutKeyIsDown('d')){
		*rudder += 0.001;
		*right_flap += 0.001;
		*left_flap -= 0.001;
	}
	else if (*rudder > 0) {
		*rudder = 0;
	}
	else if (*right_flap > 0) {
		*right_flap -= 0.001;
		*left_flap += 0.001;
	}
	if (glutKeyIsDown('a')){
		*rudder -= 0.001;
		*right_flap -= 0.001;
		*left_flap += 0.001;
	}
	else if (*rudder < 0) {
		*rudder = 0;
	}
	else if (*right_flap < 0) {
		*right_flap += 0.001;
		*left_flap -= 0.001;
	}


}

void startGameCheck(vec3 * cam,vec3 *lookAtPoint,vec3 *upVector,float camera_speed, bool *start){

	if (glutKeyIsDown('p')){
		*start = true;

	}
	if(glutKeyIsDown(27)){
		exit(0);
	}
	else {
		*cam = (vec3){10,10,10};
		*lookAtPoint = (vec3){50, 10, 50};
		vec3 n = Normalize(VectorSub(*cam, *lookAtPoint));
		vec3 u = Normalize(CrossProduct(n, *upVector));
		*upVector = CrossProduct(u, n);
	}
}


void keyboardFunc(vec3* cam,vec3* lookAtPoint, vec3* upVector, float speed){

	vec3 camDirection = Normalize(VectorSub(*cam,*lookAtPoint));
	vec3 projectedDir = Normalize((vec3){camDirection.x,0,camDirection.z});
	vec3 translateCam;

	if (glutKeyIsDown(27)) {
		exit(0);
	}
	if (glutKeyIsDown('w')){

		translateCam = ScalarMult(camDirection,-speed);
		*cam = VectorAdd(*cam, translateCam);
		*lookAtPoint = VectorAdd(*lookAtPoint, translateCam);

	}
	if (glutKeyIsDown('s')){
		translateCam = ScalarMult(projectedDir,speed);
		*cam = VectorAdd(*cam, translateCam);
		*lookAtPoint = VectorAdd(*lookAtPoint, translateCam);
	}
	if (glutKeyIsDown('a')){
		translateCam = ScalarMult(Normalize(CrossProduct(projectedDir,*upVector)),speed);
		*cam = VectorAdd(*cam, translateCam);
		*lookAtPoint = VectorAdd(*lookAtPoint, translateCam);
	}
	if (glutKeyIsDown('d')){
		translateCam = ScalarMult(Normalize(CrossProduct(projectedDir,*upVector)),-speed);
		*cam = VectorAdd(*cam, translateCam);
		*lookAtPoint = VectorAdd(*lookAtPoint, translateCam);
	}

}
float radians(float x){
	float radian = x * PI/180;
	return radian;
}


void mouse(int x, int y, vec3 *cam,vec3* lookAtPoint, vec3 *upVector, float *pitch, float *yaw, bool *firstMouse)
{

	float xoffset = x - 300;
	float yoffset = 300 - y;

	float sensitivity = 0.05;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	*yaw += xoffset;
	*pitch += yoffset;

	if(*pitch > 89.0f){
		*pitch = 89.0f;
	}

	if(*pitch < -89.0f){
		*pitch = -89.0f;
	}

	float yaw_rad = radians(*yaw);
	float pitch_rad = radians(*pitch);


	glutWarpPointer(300,300);

	vec3 dirr;
	dirr.x = cos(yaw_rad);
	dirr.y = sin(pitch_rad);
	dirr.z = sin(yaw_rad);

	*lookAtPoint = VectorAdd(dirr,*cam);

}

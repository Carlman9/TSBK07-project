#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <OpenGL/gl3.h>
#include "MicroGlut.h"
#include <math.h>
#include <string.h>
#include "GL_utilities.h"
#include "loadobj.h"
#include "LoadTGA.h"
#include "VectorUtils3.h"
#include "Plane.h"

#define PI 3.14159265359


void planeController(float *engine_speed,float *elevator, float *rudder, float *right_flap, float *left_flap, bool *start, bool *init_flag, bool* collision, bool *fps);
void startGameCheck(vec3 * cam,vec3 *lookAtPoint,vec3 *upVector,float camera_speed, bool* start);
void keyboardFunc(vec3* cam,vec3* lookAtPoint, vec3* upVector, float speed);
void mouse(int x, int y, vec3* cam, vec3* lookAtPoint, vec3* upVector, float* pitch, float* yaw, bool* firstMouse);



#endif

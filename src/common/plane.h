#ifndef PLANE_H
#define PLANE_H

#include <OpenGL/gl3.h>
#include "MicroGlut.h"
#include <math.h>
#include <string.h>
#include "GL_utilities.h"
#include "loadobj.h"
#include "LoadTGA.h"
#include "VectorUtils3.h"
#include "Heightmap.h"
#include <stdio.h>
#include <stdlib.h>


#define PI 3.14159265359


void updatePlane(float *plane_speed, vec3 *plane_coord, float *engine_speed, float *pitch, float *yaw, float *roll, float elevator, float rudder, Model * terrain, bool *collision,float *lift,float *trees, float *right_flap, float *left_flap);
void initPlane(vec3 *plane_coord,float *yaw, float start_x,float start_y,float start_z, float start_yaw);
bool checkCollision(Model *terrain,vec3 plane_coord, float *trees, float yaw, float pitch, float roll);
float calculateHeightPlane(float pitch, float plane_speed);
float calculateRoll(float plane_speed, float right_flap, float left_flap);
float calculateYaw(float rudder, float plane_speed);
float calculatePitch(float elevator, float plane_speed);
float calculateDrag(float plane_speed, float c_d);
float calculateLift(float plane_speed,float pitch);
float calculateThrust(float engine_speed);
float calculatePlaneSpeed(float engine_speed);



#endif

#ifndef HEIGHTMAP_H
#define HEIGHTMAP_H

#include <OpenGL/gl3.h>
#include "MicroGlut.h"
#include <math.h>
#include <string.h>
#include "GL_utilities.h"
#include "loadobj.h"
#include "LoadTGA.h"
#include "VectorUtils3.h"

vec3 calculateNormal(GLfloat *vertice,int x,int z,int texWidth,int texHeight);
Model* GenerateTerrain(TextureData *tex);
float barryCentric(vec3 p1, vec3 p2, vec3 p3, vec3 pos);
float calculateHeight(Model* model,float x, float z,texWidth);


#endif

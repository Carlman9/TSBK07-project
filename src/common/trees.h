#ifndef TREES_H
#define TREES_H
#include <math.h>
#include <string.h>
#include "GL_utilities.h"
#include "loadobj.h"
#include "LoadTGA.h"
#include "VectorUtils3.h"
#include "Heightmap.h"
#include <stdio.h>
#include <stdlib.h>



vec3 generateRandomSeed(Model *terrain, float width);
void generateTrees(float *trees, Model *terrain, float width, float radius);










#endif

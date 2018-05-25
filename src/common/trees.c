

#include "trees.h"


vec3 generateRandomSeed(Model *terrain, float width){
  //srand((unsigned)time(NULL));
  float x = fmodf((float)rand(), width-1);
  float z = fmodf((float)rand(), width-1);
  float y = calculateHeight(terrain,x,z,width);
  while ((y < 2) || (x > 90 && x < 150) ||(z > 570 && z < 600)) {
    x = fmodf((float)rand(), width-1);
    z = fmodf((float)rand(), width-1);
    y = calculateHeight(terrain,x,z,width);
  }

  return (vec3){x,y,z};

}

void generateTrees(float *trees, Model *terrain, float width, float radius){
  vec3 random_seed;
  random_seed = generateRandomSeed(terrain, width);
  trees[0*3+0] = random_seed.x;
  trees[0*3+1] = random_seed.y;
  trees[0*3+2] = random_seed.z;
  for (int i = 1; i < 1000; i++) {
      random_seed = generateRandomSeed(terrain, width);
      trees[i*3+0] = random_seed.x;
      trees[i*3+1] = random_seed.y;
      trees[i*3+2] = random_seed.z;
  }

}

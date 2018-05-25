//struct with plane, holds position, yaw, roll, pitch


#include "plane.h"


void updatePlane(float *plane_speed, vec3 *plane_coord, float *engine_speed, float *pitch, float *yaw, float *roll, float elevator, float rudder,Model *terrain, bool *collision, float *lift, float *trees, float *right_flap, float *left_flap) {

  *plane_speed = calculatePlaneSpeed(*engine_speed);
  *pitch = calculatePitch(elevator, *plane_speed);
  *yaw = fmodf(*yaw + calculateYaw(rudder,*plane_speed), 2*PI);
  *roll = calculateRoll(*plane_speed,*right_flap,*left_flap);
  float p_x = plane_coord->x + *plane_speed*cosf(*yaw)*0.001;
  float p_z = plane_coord->z + *plane_speed*sinf(*yaw)*0.001;
  float p_y;
  *lift = calculateLift(*plane_speed, *pitch);
  if (*plane_speed > 100) {
    p_y = plane_coord->y + calculateHeightPlane(*pitch,*plane_speed);
  }
  else {
    p_y = calculateHeight(terrain, p_x, p_z, 1024)+0.169;
  }

  *plane_coord = (vec3){p_x,p_y,p_z};
  *collision = checkCollision(terrain,*plane_coord,trees, *yaw, *pitch, *roll);
}


void initPlane(vec3 *plane_coord,float *yaw, float start_x,float start_y,float start_z, float start_yaw){
  *plane_coord = (vec3){start_x,start_y,start_z};
  *yaw = start_yaw;
}


bool checkCollision(Model *terrain,vec3 plane_coord, float *trees, float yaw, float pitch, float roll) {
  float terrain_y = calculateHeight(terrain, plane_coord.x+0.7*cosf(yaw), plane_coord.z+0.7*sinf(yaw), 1024);
  float terrain_left = calculateHeight(terrain, plane_coord.x+0.7*sinf(yaw)*cosf(roll), plane_coord.z-0.7*cosf(yaw)*cosf(roll), 1024);
  float terrain_right = calculateHeight(terrain, plane_coord.x-0.7*sinf(yaw)*cosf(roll), plane_coord.z+0.7*cosf(yaw)*cosf(roll), 1024);
  float plane_nose_y = plane_coord.y+0.6*sinf(pitch);
  vec3 coord_front = (vec3){plane_coord.x+0.35*cosf(yaw), plane_coord.y+0.3*sinf(pitch), plane_coord.z+0.35*sinf(yaw)};
  vec3 coord_left_w = (vec3){plane_coord.x+0.35*sinf(yaw)*cosf(roll), plane_coord.y+0.3*sinf(roll), plane_coord.z-0.35*cosf(yaw)*cosf(roll)};
  vec3 coord_right_w = (vec3){plane_coord.x-0.35*sinf(yaw)*cosf(roll), plane_coord.y+0.3*sinf(roll), plane_coord.z+0.35*cosf(yaw)*cosf(roll)};
  vec3 coord_back = (vec3){plane_coord.x-0.35*cosf(yaw), plane_coord.y-0.3*sinf(pitch), plane_coord.z-0.35*sinf(yaw)};
  float plane_radius = 0.35;
  float tree_r = 2;
  float radius_sum = tree_r + plane_radius;
  if (terrain_y > plane_nose_y) {
    return true;
  }
  for (int i = 0; i < 1000; i++) {
    float x=trees[i*3+0];
    float y=trees[i*3+1];
    float z=trees[i*3+2];
    float r = sqrtf((powf(plane_coord.x-x, 2)+powf(plane_coord.y-y, 2)+powf(plane_coord.z-z, 2)));
    if (r < 10){
      float rf = sqrtf((powf(coord_front.x-x, 2)+powf(coord_front.y-y, 2)+powf(coord_front.z-z, 2)));
      float rl = sqrtf((powf(coord_left_w.x-x, 2)+powf(coord_left_w.y-y, 2)+powf(coord_left_w.z-z, 2)));
      float rr = sqrtf((powf(coord_right_w.x-x, 2)+powf(coord_right_w.y-y, 2)+powf(coord_right_w.z-z, 2)));
      float rb = sqrtf((powf(coord_back.x-x, 2)+powf(coord_back.y-y, 2)+powf(coord_back.z-z, 2)));
      if (radius_sum > rf ||radius_sum > rl ||radius_sum > rr ||radius_sum > rb) {
        return true;
      }
    }
  }
  return false;
}

float calculateRoll(float plane_speed, float right_flap, float left_flap){
  return right_flap*plane_speed*0.03;
}

float calculateHeightPlane(float pitch, float plane_speed){
  return pitch+plane_speed*0.000001;
}

float calculatePitch(float elevator,float plane_speed){
  float scale = 0.01;
  return elevator*scale*plane_speed;
}
float calculateYaw(float rudder, float plane_speed){
  float scale = 0.7;
  return rudder*scale;
}

float calculateDrag(float plane_speed, float c_d){

  return c_d * powf(plane_speed, 2);
}

float calculateLift(float plane_speed, float pitch){
  float plane_down_force = 2000*9.82;
  float lift = pitch+powf(plane_speed, 2)*0.01;
  printf("lift%f\n", lift);
  return lift;
}

float calculateThrust(float engine_speed){
  float eta = 0.01;
  return engine_speed*eta;
}

float calculatePlaneSpeed(float engine_speed){

  return engine_speed*0.1;
}

// projekt flight simulator, TSBK07

#include <OpenGL/gl3.h>
#include "MicroGlut.h"


#include <math.h>
#include <string.h>
#include "GL_utilities.h"
#include "loadobj.h"
#include "LoadTGA.h"
#include "VectorUtils3.h"
#include "Heightmap.h"
#include "controller.h"
#include "plane.h"
#include <stdio.h>
#include <stdlib.h>
#include "trees.h"
#include "simplefont.h"

#define PI 3.14159265359

// Globala variabler
mat4 projectionMatrix;
Model *m, *m2, *tm, *lowpoly_tree, *skybox, *cirrus;
// Reference to shader program
GLuint program, prog_sky, prog_plane, prog_obj;
GLuint tex_road, tex_water, tex_sand, tex_dirt2, tex_grass, blendmap, tex_sky;
TextureData ttex; // terrain
TextureData right, left, top, bottom, back, front; //skybox

bool debug = false;
bool start = false;
bool init_flag = true;
bool fps = false;
float start_x = 97;
float start_z = 587;
float zoom = 3;


GLfloat camera_speed = 1;
vec3 upVector = {0, 1, 0};
vec3 cam = {0, 10, 0};
vec3 lookAtPoint = {15, 1, 15};
mat4 camMatrix, modelView;
float a = 0;
bool firstMouse = true;
float alpha = 0;
float min_y;


//physic variables
static const gravity = 9.81;

//Planevariables
vec3 plane_coord;
float elevator = 0; // change pitch
float rudder = 0; // change yaw
float right_flap = 0; //lift and drag
float left_flap = 0; // lift and drag
float engine_speed = 0;
float plane_weight = 2000 * gravity;
float plane_speed = 0;
float drag;
float lift;
float yaw = 0;
float pitch = 0;
float roll = 0;
float thrust = 0;
float c_d = 0.1;
float c_l = 1;
float scale = 0.1;
bool collision = false;


//object tree_coords
int number_of_trees = 1000;
float trees[3000];




void init(void)
{
	// GL inits

	glClearColor(0.99, 0.99, 0.99, 1);//gray color, same as fog color
  glClearDepth(1);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	printError("GL inits");

	projectionMatrix = frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 250);

	// Load and compile shader
	program = loadShaders("src/shaders/terrain.vert", "src/shaders/terrain.frag");
	prog_sky = loadShaders("src/shaders/sky.vert", "src/shaders/sky.frag");
	prog_plane = loadShaders("src/shaders/plane.vert", "src/shaders/plane.frag");
	prog_obj = loadShaders("src/shaders/objects.vert", "src/shaders/objects.frag");

	glUseProgram(prog_obj);
	lowpoly_tree = LoadModelPlus("res/models/Lowpoly_tree.obj");
	glUniformMatrix4fv(glGetUniformLocation(prog_obj, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);

	glUseProgram(program);
	printError("init shader");


	//Load models and texture for terrain
	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
	glUniform1i(glGetUniformLocation(program, "tex_water"), 0); // Texture unit 0
	glUniform1i(glGetUniformLocation(program, "tex_grass"), 1);
	glUniform1i(glGetUniformLocation(program, "tex_dirt2"), 2);
	glUniform1i(glGetUniformLocation(program, "tex_road"), 3);
	glUniform1i(glGetUniformLocation(program, "tex_sand"), 4);

	LoadTGATextureSimple("res/textures/water_tex.tga", &tex_water);
	LoadTGATextureSimple("res/textures/sand.tga", &tex_sand);
	LoadTGATextureSimple("res/textures/road.tga", &tex_road);
	LoadTGATextureSimple("res/textures/dirt2.tga", &tex_dirt2);
	LoadTGATextureSimple("res/textures/grass.tga", &tex_grass);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex_water);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex_grass);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, tex_dirt2);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, tex_road);

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, tex_sand);

	LoadTGATextureData("res/textures/honolulu.tga", &ttex);
	tm = GenerateTerrain(&ttex);

	generateTrees(trees,tm, 1024, 10);




	// Skybox load
	glUseProgram(prog_sky);
	skybox = LoadModelPlus("res/models/skybox.obj");
	//projectionMatrix = perspective(/* field of view in degree */ 90.0,
	//	/* aspect ratio */ 1.0, /* Z near */ 0.2, /* Z far */ 10.0);


	projectionMatrix = frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 3.5);


	glUniformMatrix4fv(glGetUniformLocation(prog_sky, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);

	glUniform1i(glGetUniformLocation(prog_sky, "tex_sky"), 5); // Texture unit


LoadTGATextureData("res/textures/TropicalSunnyDay/front.tga", &right);
LoadTGATextureData("res/textures/TropicalSunnyDay/back.tga", &left);
LoadTGATextureData("res/textures/TropicalSunnyDay/left.tga", &front);
LoadTGATextureData("res/textures/TropicalSunnyDay/right.tga", &back);
LoadTGATextureData("res/textures/TropicalSunnyDay/top.tga", &top);
LoadTGATextureData("res/textures/TropicalSunnyDay/down.tga", &bottom);

	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_CUBE_MAP,tex_sky); // Bind texture unit
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, right.width, right.height, 0, GL_RGB, GL_UNSIGNED_BYTE, right.imageData);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, left.width, left.height, 0, GL_RGB, GL_UNSIGNED_BYTE, left.imageData);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, top.width, top.height, 0, GL_RGB, GL_UNSIGNED_BYTE, top.imageData);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, bottom.width, bottom.height, 0, GL_RGB, GL_UNSIGNED_BYTE, bottom.imageData);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, back.width, back.height, 0, GL_RGB, GL_UNSIGNED_BYTE, back.imageData);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, front.width, front.height, 0, GL_RGB, GL_UNSIGNED_BYTE, front.imageData);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);



	//plane init
	glUseProgram(prog_plane);
	cirrus = LoadModelPlus("res/models/Cirrus.obj");
	ScaleModel(cirrus, scale, scale, scale);
	projectionMatrix = frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 50.0);
	glUniformMatrix4fv(glGetUniformLocation(prog_plane, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);

	min_y = 100;
	for (int i = 0; i < (cirrus->numVertices-1); i++) {
		if (cirrus->vertexArray[(i * 3 + 1)] < min_y) {
			min_y = cirrus->vertexArray[(i * 3 + 1)];
		}
	}
	float min_x = 100;
	for (int i = 0; i < (cirrus->numVertices-1); i++) {
		if (cirrus->vertexArray[(i * 3 + 0)] < min_x) {
			min_x = cirrus->vertexArray[(i * 3 + 0)];
		}
	}




	printError("init terrain");
}
void camera(float alpha, float zoom, float beta) {
	if (fps) {
		cam = (vec3){plane_coord.x+0.3*cosf(yaw),plane_coord.y,plane_coord.z+0.3*sinf(yaw)};
		lookAtPoint = (vec3){plane_coord.x+10*cosf(yaw),plane_coord.y+10*sinf(pitch),plane_coord.z+10*sinf(yaw)};
	}else{
		float l = zoom*cosf(alpha);
		float h = zoom*sinf(alpha);
		cam = (vec3){plane_coord.x-l*cosf(beta),h + plane_coord.y,plane_coord.z-l*sinf(beta)};
		lookAtPoint = (vec3){plane_coord.x,plane_coord.y,plane_coord.z};
		upVector = (vec3){0,1,0};
	}
}


void startGame(){
	//set startpos
	if (debug) {
		cam = (vec3){start_x,1+calculateHeight(tm,start_x,start_z,ttex.width),start_z};
		lookAtPoint = (vec3){start_x+10,calculateHeight(tm,start_x,start_z,ttex.width),start_z};
		camMatrix = lookAtv(cam,lookAtPoint,upVector);
		glUniformMatrix4fv(glGetUniformLocation(program, "cameraMatrix"), 1, GL_TRUE, camMatrix.m);
	}
	else {

		initPlane(&plane_coord,&yaw,start_x,-min_y+calculateHeight(tm,start_x,start_z,ttex.width),start_z,0);
		plane_speed = 0;
		engine_speed = 0;
		pitch = 0;
		roll = 0;
		camera(alpha,zoom,yaw);



	}


}




void display(void)
{
	glUseProgram(prog_sky);
	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (debug) {
		keyboardFunc(&cam,&lookAtPoint,&upVector,camera_speed);
	} else if (!start) {
		startGameCheck(&cam,&lookAtPoint,&upVector,camera_speed,&start); //om s så start true
	}
	else if(start && init_flag){
		startGame(); // sätter ingen flagga start fort true
		init_flag = false;
	}
	else
	{
		planeController(&engine_speed,&elevator, &rudder, &right_flap, &left_flap, &start, &init_flag, &collision, &fps); //sätter init true, start false om q eller collision
		updatePlane(&plane_speed, &plane_coord, &engine_speed, &pitch, &yaw, &roll, elevator, rudder, tm, &collision, &lift, &trees, &right_flap, &left_flap);
		camera(alpha, zoom, yaw);
	}


	camMatrix = lookAtv(cam,lookAtPoint,upVector);
	modelView = IdentityMatrix();
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	camMatrix.m[3] = 0;
	camMatrix.m[7] = 0;
	camMatrix.m[11] = 0;
	glUniformMatrix4fv(glGetUniformLocation(prog_sky, "cameraMatrix"), 1, GL_TRUE, camMatrix.m);
	glUniformMatrix4fv(glGetUniformLocation(prog_sky, "mdlMatrix"), 1, GL_TRUE, modelView.m);
	DrawModel(skybox, prog_sky, "inPosition", NULL, NULL);

	glUseProgram(prog_plane);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	if (!start) {
		sfDrawString(50, 50, "To start press p");
		sfDrawString(35, 35, "To quit press esc");
	}
	if (collision) {
		sfDrawString(200, 200, "Crash");
	}
	projectionMatrix = frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 150);
	glUniformMatrix4fv(glGetUniformLocation(prog_plane, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
	mat4 camPlaneMatrix = lookAtv(cam,lookAtPoint,upVector);


	mat4 planeView = Mult(Mult(Mult(Mult(T(plane_coord.x,plane_coord.y,plane_coord.z),S(scale,scale,scale)),Ry(PI/2-yaw)),Rx(-pitch)),Rz(roll));
	glUniformMatrix4fv(glGetUniformLocation(prog_plane, "cameraMatrix"), 1, GL_TRUE, camPlaneMatrix.m);
	glUniformMatrix4fv(glGetUniformLocation(prog_plane, "mdlMatrix"), 1, GL_TRUE, planeView.m);
	DrawModel(cirrus, prog_plane, "inPosition", "inNormal", "inTexCoord");


	//terrain
	glUseProgram(program);
	modelView = IdentityMatrix();
	camMatrix = lookAtv(cam,lookAtPoint,upVector);
	glUniformMatrix4fv(glGetUniformLocation(program, "cameraMatrix"), 1, GL_TRUE, camMatrix.m);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, modelView.m);
	glUniform3f(glGetUniformLocation(program, "cameraPos"), cam.x, cam.y, cam.z);

	// Bind Our Texture tex1
	DrawModel(tm, program, "inPosition", "inNormal", "inTexCoord");

	glUseProgram(prog_obj);
	glUniformMatrix4fv(glGetUniformLocation(prog_obj, "cameraMatrix"), 1, GL_TRUE, camMatrix.m);
	glUniform3f(glGetUniformLocation(prog_obj, "cameraPos"), cam.x, cam.y, cam.z);
	for (int i = 0; i < number_of_trees; i++) {
		modelView = Mult(T(trees[i*3+0],trees[i*3+1],trees[i*3+2]),S(0.1,0.1,0.1));
		glUniformMatrix4fv(glGetUniformLocation(prog_obj, "mdlMatrix"), 1, GL_TRUE, modelView.m);
		DrawModel(lowpoly_tree, prog_obj, "inPosition", "inNormal", "inTexCoord");
	}


printError("display 2");

glutSwapBuffers();
}

void timer(int i)
{
	glutTimerFunc(20, &timer, i);
	glutPostRedisplay();
}

void mouseHelp(int x, int y){
	if (debug) {
		mouse(x, y, &cam, &lookAtPoint ,&upVector, &pitch, &yaw, &firstMouse);
		camMatrix = lookAtv(cam,lookAtPoint,upVector);
	}
}
void reshape(GLsizei w, GLsizei h)
{
	glViewport(0, 0, w, h);
	sfSetRasterSize(w, h);
}
int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitContextVersion(3, 2);
	glutInitWindowSize (600, 600);
	glutCreateWindow ("Flightsimulator with no gravity :)");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	init ();
	glutTimerFunc(20, &timer, 0);
	glutPassiveMotionFunc(mouseHelp);
	glutFullScreen();
	glutHideCursor();
	sfMakeRasterFont(); // init font
	sfSetRasterSize(600, 200);

	glutMainLoop();
	exit(0);
}

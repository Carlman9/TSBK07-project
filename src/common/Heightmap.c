//Skapar Heightmap från tga fil, räknar ut normaler och allt



#include "Heightmap.h"


vec3 calculateNormal(GLfloat *vertice,int x,int z,int texWidth,int texHeight)
{
	if (x == 0 || x == texWidth) {
		vec3 n = {0,1,0};
		return n;
	}
	if (z == 0 || z == texHeight) {
		vec3 n = {0,1,0};
		return n;
	}
	float hl = vertice[(x-1 + z * texWidth)*3 + 1];
	float hr = vertice[(x+1 + z * texWidth)*3 + 1];
	float hd = vertice[(x + (z+1) * texWidth)*3 + 1];
	float hu = vertice[(x + (z-1) * texWidth)*3 + 1];
	vec3 n = {hl - hr, 2.0f, hd - hu};
	n = Normalize(n);
	return n;

}

Model* GenerateTerrain(TextureData *tex)
{
	int vertexCount = tex->width * tex->height;
	int triangleCount = (tex->width-1) * (tex->height-1) * 2;
	int x, z;
	GLfloat *vertexArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
	GLfloat *normalArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
	GLfloat *texCoordArray = malloc(sizeof(GLfloat) * 2 * vertexCount);
	GLuint *indexArray = malloc(sizeof(GLuint) * triangleCount*3);
	printf("bpp %d\n", tex->width);
	for (x = 0; x < tex->width; x++)
	for (z = 0; z < tex->height; z++)
	{
		// Vertex array. You need to scale this properly
		vertexArray[(x + z * tex->width)*3 + 0] = x / 1.0;
		vertexArray[(x + z * tex->width)*3 + 1] = tex->imageData[(x + z * tex->width) * (tex->bpp/8)] / 10.0;
		vertexArray[(x + z * tex->width)*3 + 2] = z / 1.0;


		// Texture coordinates. You may want to scale them.
		texCoordArray[(x + z * tex->width)*2 + 0] = x;//(float)x / tex->width;
		texCoordArray[(x + z * tex->width)*2 + 1] = z;//(float)z / tex->height;
	}
	for (x = 0; x < tex->width; x++)
	for (z = 0; z < tex->height; z++)
	{
		vec3 normal;
		normal = calculateNormal(vertexArray,x,z,tex->width,tex->height);

		// Normal vectors. You need to calculate these.
		normalArray[(x + z * tex->width)*3 + 0] = normal.x;
		normalArray[(x + z * tex->width)*3 + 1] = normal.y;
		normalArray[(x + z * tex->width)*3 + 2] = normal.z;
	}

	for (x = 0; x < tex->width-1; x++)
	for (z = 0; z < tex->height-1; z++)
	{
		// Triangle 1
		indexArray[(x + z * (tex->width-1))*6 + 0] = x + z * tex->width;
		indexArray[(x + z * (tex->width-1))*6 + 1] = x + (z+1) * tex->width;
		indexArray[(x + z * (tex->width-1))*6 + 2] = x+1 + z * tex->width;
		// Triangle 2
		indexArray[(x + z * (tex->width-1))*6 + 3] = x+1 + z * tex->width;
		indexArray[(x + z * (tex->width-1))*6 + 4] = x + (z+1) * tex->width;
		indexArray[(x + z * (tex->width-1))*6 + 5] = x+1 + (z+1) * tex->width;
	}
	// End of terrain generation
	// Create Model and upload to GPU:
	Model* model = LoadDataToModel(
		vertexArray,
		normalArray,
		texCoordArray,
		NULL,
		indexArray,
		vertexCount,
		triangleCount*3);
		return model;
	}
  float barryCentric(vec3 p1, vec3 p2, vec3 p3, vec3 pos) {
		float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
		float l1 = ((p2.z - p3.z) * (pos.x - p3.x) + (p3.x - p2.x) * (pos.z - p3.z)) / det;
		float l2 = ((p3.z - p1.z) * (pos.x - p3.x) + (p1.x - p3.x) * (pos.z - p3.z)) / det;
		float l3 = 1.0f - l1 - l2;
		return l1 * p1.y + l2 * p2.y + l3 * p3.y;
	}

  float calculateHeight(Model* model,float x, float z,texWidth) {
    int intX = (int)x;
    int intZ = (int)z;
    float deltax = x - intX;
    float deltaz = z - intZ;


    if (intX > texWidth || intZ > texWidth || intX < 0 || intZ < 0) {
      return 0;
    }

    vec3 p1, p2, p3;
    //triangel 2
    if (deltax+deltaz > 1) {
      p1 = (vec3){1,model->vertexArray[(intX+1 + (intZ+1) * texWidth)*3 + 1],1};
      p2 = (vec3){1,model->vertexArray[(intX+1 + intZ * texWidth)*3 + 1],0};
      p3 = (vec3){0,model->vertexArray[(intX + (intZ+1) * texWidth)*3 + 1],1};
    }
    //triangel 1
    else{
      p1 = (vec3){0,model->vertexArray[(intX + intZ * texWidth)*3 + 1],0};
      p2 = (vec3){1,model->vertexArray[(intX+1 + intZ * texWidth)*3 + 1],0};
      p3 = (vec3){0,model->vertexArray[(intX + (intZ+1) * texWidth)*3 + 1],1};
    }
    vec3 pos = {deltax,0,deltaz};
    return barryCentric(p1, p2, p3, pos);

  }

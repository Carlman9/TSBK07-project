commondir = src/common/

all : main

main : src/main.c $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)loadobj.c $(commondir)LoadTGA.c $(commondir)Heightmap.c $(commondir)controller.c $(commondir)plane.c $(commondir)trees.c $(commondir)simplefont.c $(commondir)Mac/MicroGlut.m
	gcc -Wall -o main -I$(commondir) -I../common/Mac -DGL_GLEXT_PROTOTYPES src/main.c $(commondir)GL_utilities.c $(commondir)loadobj.c $(commondir)Heightmap.c $(commondir)controller.c $(commondir)plane.c $(commondir)VectorUtils3.c $(commondir)LoadTGA.c $(commondir)trees.c $(commondir)simplefont.c $(commondir)Mac/MicroGlut.m -framework OpenGL -framework Cocoa -lm

clean :
	rm main

# set this variable to the director in which you saved the common files
commondir = ../common/

all: $(commondir)LoadTGA.c SpriteLight.c SpriteLightDemo.c $(commondir)VectorUtils3.c $(commondir)GL_utilities.c $(commondir)MicroGlut.c
	gcc -Wall -o SpriteLightDemo -I$(commondir) -lGL $(commondir)LoadTGA.c $(commondir)VectorUtils3.c $(commondir)GL_utilities.c $(commondir)MicroGlut.c SpriteLight.c SpriteLightDemo.c -DGL_GLEXT_PROTOTYPES

clean:
	rm -f SpriteLightDemo


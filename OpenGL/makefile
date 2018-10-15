DEPSROOT=../Dependencies/

FREETYPE=$(DEPSROOT)FreeType/include/
BOX2DHEADER=$(DEPSROOT)Box2D/include/
BOX2DLIB=$(DEPSROOT)Box2D/lib/Linux/release/

HEADER=./header/
SOURCE=src/*.cpp

INCLUDES=-I$(DEPSROOT) -I$(FREETYPE) -I$(BOX2DHEADER) -I$(HEADER) 
LIBDIR=-L$(BOX2DLIB) 

LIBS=-lGL -lGLEW -lglfw3 -lrt -lm -ldl -lXrandr -lXinerama -lXxf86vm -lXext -lXcursor -lXrender -lXfixes -lX11 -lpthread -lxcb -lXau -lXdmcp -lsfml-system -lsfml-audio -lfreetype -lBox2D

CC=g++
FLAGS=-std=c++11
TARGET=../bin/

opengl: $(SOURCE)
	$(CC) $(FLAGS) -o $(TARGET)$@ $(SOURCE) $(INCLUDES) $(LIBDIR) $(LIBS)

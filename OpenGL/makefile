DEPROOT=../Dependencies/

FREETYPE=$(DEPROOT)FreeType/include/
BOX2DHEADER=$(DEPROOT)Box2D/include/
BOX2DLIB=$(DEPROOT)Box2D/lib/Linux/release/
BOOSTHEADER=$(DEPROOT)boost/include/
BOOSTLIB=$(DEPROOT)boost/lib/Linux/

HEADER=./header/
SOURCE=src/*.cpp

INCLUDES=-I$(DEPROOT) -I$(FREETYPE) -I$(BOX2DHEADER) -I$(BOOSTHEADER) -I$(HEADER)
LIBDIR=-L$(BOX2DLIB) -L$(BOOSTLIB)

LIBS=-lGL -lGLEW -lglfw3 -lrt -lm -ldl -lXrandr -lXinerama -lXxf86vm -lXext -lXcursor -lXrender -lXfixes -lX11 -lpthread -lxcb -lXau -lXdmcp -lsfml-system -lsfml-audio -lfreetype -lBox2D -lboost_filesystem -lboost_system

CC=g++
FLAGS=-std=c++11
TARGET=../bin/

opengl: $(SOURCE)
	$(CC) $(FLAGS) -o $(TARGET)$@ $(SOURCE) $(INCLUDES) $(LIBDIR) $(LIBS)

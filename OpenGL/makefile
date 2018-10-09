DEPS=../Dependencies/
FREETYPE=$(DEPS)FreeType/include/
HEADER=header/
SOURCE=src/*.cpp

FLAG1=-I$(DEPS)
FLAG2=-I$(HEADER)
FLAG3=-I$(FREETYPE)

LIBS=-lGL -lGLEW -lglfw3 -lrt -lm -ldl -lXrandr -lXinerama -lXxf86vm -lXext -lXcursor -lXrender -lXfixes -lX11 -lpthread -lxcb -lXau -lXdmcp -lsfml-system -lsfml-audio -lfreetype

CC=g++ -std=c++11
TARGET=../bin/

opengl: $(SOURCE)
	$(CC) -o $(TARGET)$@ $(SOURCE) $(FLAG1) $(FLAG2) $(FLAG3) $(LIBS)

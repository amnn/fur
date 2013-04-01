CC         = g++-4.7 --std=c++0x
FRAMEWORKS = -framework OpenGL
LIBRARIES  = -lglew -lglfw
LOADPATHS  = -L/usr/local/Cellar/glew/1.9.0/lib/ -L/usr/local/Cellar/glfw/2.7.7/lib/
DEST 	   = bin/fur
INCLUDES   = -Iincludes
SRCFILES   = src/*
#SRCFILES   = src/main.cpp src/RenderGroup.cpp src/GLFWScr.cpp src/ShaderProgram.cpp


all:
	$(CC) $(FRAMEWORKS) $(LOADPATHS) $(LIBRARIES) $(INCLUDES) -o $(DEST) $(SRCFILES)
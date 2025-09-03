all:
	c++ -Wall -Werror -Wextra main.cpp opengl/src/glad.cpp -o scop -I./opengl/include -L./opengl/library opengl/library/libglfw.3.dylib  -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo

try:
	c++ main2.cpp opengl/src/glad.cpp -I./opengl/include -L./opengl/library opengl/library/libglfw.3.dylib  -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo
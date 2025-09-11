NAME	= scop

CXX		= c++
CXXFLAGS= -std=c++98
CXXFLAGS+= -Wall -Wextra -Werror
OPENGL	= -L./opengl/library opengl/library/libglfw.3.dylib -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo
INCLUDE	= -I./ -I./opengl/include
SRCS	= main.cpp math.cpp opengl/src/glad.cpp
OBJS	= $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) $(INCLUDE) $(OPENGL) -o $(NAME)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re

NAME	= scop

CXX		= c++
CXXFLAGS= -std=c++98
CXXFLAGS+= -Wall -Wextra -Werror
OPENGL	= -lglfw
INCLUDE	= -I./ -I./opengl/include
SRCS	= main.cpp math.cpp operation.cpp texture.cpp gl.cpp obj.cpp opengl/src/glad.cpp
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

# DEFAULT DEV MAKEFILE

LINKING:=
CC:=gcc
CXX:=g++
LIBS:=-lX11 -lXrandr -lXcursor -lsfml-graphics -lsfml-window -lsfml-system
CXX_STANDARD:=-std=c++17
WFLAGS:=-Wall -Wextra -Wpedantic

DFLAGS:=-g3 -fsanitize=address
# DFLAGS:=-O3 -D EIGEN_NO_DEBUG

SRC:=$(wildcard *.cpp)
# OBJ:=$(SRC:.cpp=.o)
OBJ:=ffnn.o pipe.o bird.o collision.o genetic_algo.o gamestats.o
BIN_OUT:=flappy.out

run: $(BIN_OUT)
	./$(BIN_OUT)

$(BIN_OUT): main.cc $(OBJ)
	$(strip $(CXX) $(CXX_STANDARD) $(WFLAGS) main.cc $(OBJ) -o $(BIN_OUT) $(LIBS) $(LINKING) $(DFLAGS))

######################## dependent targets ########################

gamestats.o : gamestats.cpp gamestats.hpp bird.o
	$(strip $(CXX) $(CXX_STANDARD) $(WFLAGS) -c $< -o $@ $(DFLAGS))

genetic_algo.o : genetic_algo.cpp genetic_algo.hpp bird.o pipe.o
	$(strip $(CXX) $(CXX_STANDARD) $(WFLAGS) -c $< -o $@ $(DFLAGS))

collision.o : collision.cpp collision.hpp bird.o pipe.o
	$(strip $(CXX) $(CXX_STANDARD) $(WFLAGS) -c $< -o $@ $(DFLAGS))

bird.o : bird.cpp bird.hpp config.hpp ffnn.o
	$(strip $(CXX) $(CXX_STANDARD) $(WFLAGS) -c $< -o $@ $(DFLAGS))

######################## independent targets ########################

ffnn.o : ffnn.cpp ffnn.hpp config.hpp
	$(strip $(CXX) $(CXX_STANDARD) $(WFLAGS) -c $< -o $@ $(DFLAGS))

pipe.o : pipe.cpp pipe.hpp config.hpp
	$(strip $(CXX) $(CXX_STANDARD) $(WFLAGS) -c $< -o $@ $(DFLAGS))

######################## other targets ########################

clean:
	rm *.out *.o *.nn

style:
	@echo fixing code format and code style...
	@clang-format -i -style=file main.cc *.cpp *.hpp
	@echo done fixing style format.
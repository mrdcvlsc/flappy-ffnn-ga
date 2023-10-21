# DEFAULT DEV MAKEFILE

CC:=gcc
CXX:=g++
LIBS:=-lX11 -lXrandr -lXcursor -lsfml-graphics -lsfml-window -lsfml-system
CXX_STANDARD:=-std=c++20
WFLAGS:=-Wall -Wextra -Wpedantic
DFLAGS:=-g3 -fsanitize=address

SRC:=$(wildcard *.cpp)
OBJ=$(SRC:.cpp=.o)
BIN_OUT:=flappy.out

run: $(BIN_OUT)
	./$(BIN_OUT)

$(BIN_OUT): main.o config.hpp $(OBJ)
	$(strip $(CXX) $(CXX_STANDARD) $(WFLAGS) main.o $(OBJ) -o $(BIN_OUT) $(LIBS) $(DFLAGS))

main.o : main.cc
	$(strip $(CXX) $(CXX_STANDARD) $(WFLAGS) -c main.cc -o main.o $(DFLAGS))

$(OBJ): %.o : %.cpp %.hpp
	$(strip $(CXX) $(CXX_STANDARD) $(WFLAGS) -c $< -o $@ $(DFLAGS))

clean:
	rm *.out *.o *.exe

style:
	@echo fixing code format and code style...
	@clang-format -i -style=file main.cc *.cpp *.hpp
	@echo done fixing style format.
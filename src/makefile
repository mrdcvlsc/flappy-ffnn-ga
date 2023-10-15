# DEFAULT DEV MAKEFILE

CC:=gcc
CXX:=g++
LIBS:=-lX11 -lXrandr -lXcursor -lsfml-graphics -lsfml-window -lsfml-system
CXX_STANDARD:=-std=c++17
WFLAGS:=-Wall -Wextra -Wpedantic
DFLAGS:=-g3 -fsanitize=address

BIN_OUT:=flappy.out

run: $(BIN_OUT)
	./$(BIN_OUT)

$(BIN_OUT): main.cpp
	$(strip $(CXX) $(CXX_STANDARD) $(WFLAGS) main.cpp -o $(BIN_OUT) $(LIBS) $(DFLAGS))
	
clean:
	rm *.out

style:
	@echo fixing code format and code style...
	@clang-format -i -style=file *.cpp *.hpp
	@echo done fixing style format.
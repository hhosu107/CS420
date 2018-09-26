CC=g++
CCFLAGS=--std=c++0x -g -O0

SRC_DIR=src
OBJ_DIR=obj
BIN_DIR=bin

DEPS=arith-scanner.hpp \
		 arith-parser.hpp \
		 arith-ast.hpp
SCANNER=arith-scanner.cpp
PARSER=arith-parser.cpp \
			 arith-ast.cpp

DEPS_=$(patsubst %,$(SRC_DIR)/%,$(DEPS))
OBJ_SCANNER=$(patsubst %.cpp,$(OBJ_DIR)/%.o,$(SCANNER))
OBJ_PARSER=$(patsubst %.cpp,$(OBJ_DIR)/%.o,$(PARSER) $(SCANNER))

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(DEPS_)
	$(CC) $(CCFLAGS) -c -o $@ $<

all: arith-main

arith-main: $(OBJ_DIR)/arith-main.o $(OBJ_PARSER)
	$(CC) $(CCFLAGS) -o $(BIN_DIR)/$@ $(OBJ_DIR)/arith-main.o $(OBJ_PARSER)

clean:
	rm -rf $(OBJ_DIR)/*.o $(BIN_DIR)/arith-main

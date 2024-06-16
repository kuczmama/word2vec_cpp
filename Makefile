CXX = g++
CXXFLAGS = -std=c++11 -Wall -Iinclude
LDFLAGS = 

# Directory for compiled binaries
BIN = bin/
# Source directory
SRC = src/
# Include directory
INCLUDE = include/
# Object files directory
OBJ = obj/

TARGET = $(BIN)word2vec_app

SOURCES = $(wildcard $(SRC)*.cpp)
OBJECTS = $(SOURCES:$(SRC)%.cpp=$(OBJ)%.o)

all: directories $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(LDFLAGS) $^ -o $@

$(OBJ)%.o: $(SRC)%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

directories:
	mkdir -p $(BIN)
	mkdir -p $(OBJ)

clean:
	rm -f $(BIN)* $(OBJ)*

rebuild: clean all

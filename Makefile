CXX = g++-14
CXXFLAGS = -std=c++17 -Wall -Iinclude
LDFLAGS = -lstdc++fs

# Directory for compiled binaries
BIN = bin/
# Source directory
SRC = src/
# Include directory
INCLUDE = include/
# Object files directory
OBJ = obj/

# Targets for training and inference
TRAIN_TARGET = $(BIN)train_word2vec
INFERENCE_TARGET = $(BIN)inference_word2vec

# Object files for each program
TRAIN_OBJECTS = $(OBJ)train_main.o $(OBJ)word2vec.o $(OBJ)vector.o
INFERENCE_OBJECTS = $(OBJ)inference_main.o $(OBJ)word2vec.o $(OBJ)vector.o

all: directories $(TRAIN_TARGET) $(INFERENCE_TARGET)

# Training binary
$(TRAIN_TARGET): $(TRAIN_OBJECTS)
	$(CXX) $^ -o $@ $(LDFLAGS)

# Inference binary
$(INFERENCE_TARGET): $(INFERENCE_OBJECTS)
	$(CXX) $^ -o $@ $(LDFLAGS)

# General rule for object files
$(OBJ)%.o: $(SRC)%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

directories:
	mkdir -p $(BIN)
	mkdir -p $(OBJ)

clean:
	rm -f $(BIN)*
	rm -f $(OBJ)*

rebuild: clean all

.PHONY: all directories clean rebuild
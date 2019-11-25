# @file Makefile
#
# @author Gabriel F P Araujo
#

#########
# Setup #
#########

# Default driver

# Compiler & flags
CC       = g++
# CXXFLAGS = -Wall -std=c++11 -g
CXXFLAGS = -Wall -std=c++11 -g

# Target [folder(s)]
BIN_DIR = bin
OBJ_DIR = obj
TEST_DIR = test
TARGET  = $(BIN_DIR)/assembler
SIM	    = $(BIN_DIR)/simulator

# Include directories
INC_DIR    = include/

# Source
SRC         = assembler.cpp
SRCS        = simulator.cpp
MAIN        = main
MAINS       = main_sim
SRC_DIR     = src
MAIN_SRC    = main_asm.cpp
MAIN_SRCS   = main_sim.cpp
# SRC         = $(notdir $(wildcard $(SRC_DIR)/*.cpp))

############
# Organize #
############
OBJ = $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(SRC))
OBJS= $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(SRCS))
MAIN_OBJ = $(OBJ_DIR)/$(MAIN).o
MAIN_OBJS= $(OBJ_DIR)/$(MAINS).o

FLAGS      = $(CXXFLAGS) $(EXTFLAGS)
HEADERS    = $(addprefix -I,$(INC_DIR))
OBJECTS    = $(OBJ)
OBJECTSS    = $(OBJS)

###########
# Targets #
###########
all: $(TARGET) $(SIM)

dirs:
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(BIN_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) -c -o $@ $< $(CXXFLAGS) -I$(INC_DIR)

# $(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
# 	$(CC) -c -o $@ $< $(CXXFLAGS) -I$(INC_DIR)

$(OBJ): $(SRC_DIR)/$(SRC) $(OBJ_DIR)
	$(CC) -c -o $@ $< $(CXXFLAGS) $(HEADERS)

$(MAIN_OBJ): $(SRC_DIR)/$(MAIN_SRC) 
	$(CC) -c -o $@ $< $(CXXFLAGS) -I$(INC_DIR)

$(TARGET): dirs $(OBJ) $(MAIN_OBJ) $(BIN_DIR)
	$(CC) $(FLAGS) -o $(TARGET) $(MAIN_OBJ) $(HEADERS) $(OBJECTS)

$(SIM): dirs $(OBJS) $(MAIN_OBJS) $(BIN_DIR)
	$(CC) $(FLAGS) -o $(SIM) $(MAIN_OBJS) $(HEADERS) $(OBJECTSS)

clean:
	@rm -rf $(OBJ_DIR)/*.o $(BIN_DIR)/* *.txt

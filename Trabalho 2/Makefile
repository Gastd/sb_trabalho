# @file Makefile
#
# @author Gabriel F P Araujo
# @author Gustavo Costa Crispim De Souza
#

#########
# Setup #
#########

# Default driver

# Compiler & flags
CC       = g++
CXXFLAGS = -Wall -std=c++11 -g -O3

# Target [folder(s)]
BIN_DIR = bin
OBJ_DIR = obj
TEST_DIR = test
TARGET  = $(BIN_DIR)/assembler
SIM	    = $(BIN_DIR)/simulator

# Include directories
# INC_DIR    = include/

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
LIG = ligador
MOT = montador
LIG_SRC = ligador.cpp
MOT_SRC = montador.cpp

FLAGS      = $(CXXFLAGS) $(EXTFLAGS)

###########
# Targets #
###########
all: $(LIG) $(MOT)

MOT:
	$(CC) $(FLAGS) -o $(MOT) $(MOT_SRC)
LIG:
	$(CC) $(FLAGS) -o $(LIG) $(LIG_SRC)
clean:
	@rm -rf *.obj *.txt montador ligador a.out

# @file Makefile
#
# @author Gabriel F P Araujo
#

#########
# Setup #
#########

# Default driver
# DRIVER ?= FSMDriver3

# Compiler & flags
CC       = g++
CXXFLAGS = -Wall -std=c++11 -g

# Target [folder(s)]
BIN_DIR = bin
OBJ_DIR = obj
TEST_DIR = test
EXPCT_DIR= $(TEST_DIR)/expected
TARGET  = $(BIN_DIR)/simulator

# Include directories
INC_DIR    = include/

# Source
# CLIENT_SRC_DIR = src/
# CLIENT_SRC     = WrapperBaseDriver.cpp SimpleParser.cpp CarState.cpp CarControl.cpp
SRC         = simulator.cpp
MAIN        = main
SRC_DIR     = src
MAIN_SRC    = main.cpp
# SRC         = $(notdir $(wildcard $(SRC_DIR)/*.cpp))

############
# Organize #
############
OBJ = $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(SRC))
MAIN_OBJ = $(OBJ_DIR)/$(MAIN).o

FLAGS      = $(CXXFLAGS) $(EXTFLAGS)
HEADERS    = $(addprefix -I,$(INC_DIR))
OBJECTS    = $(OBJ)

###########
# Targets #
###########
all: $(TARGET)

dirs:
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(BIN_DIR)
	@mkdir -p $(EXPCT_DIR)

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

clean:
	@rm -rf $(OBJ_DIR)/*.o $(BIN_DIR)/* *.txt

.PHONY = test_simulator
test_simulator: all test_prime test_fibonacci test_testador

test_prime:
	@echo "Rodando teste de numeros primos"
	./$(TARGET) $(TEST_DIR)/primos.text.bin $(TEST_DIR)/primos.data.bin 1 > primos_output.txt
	diff $(EXPCT_DIR)/primos_correct.txt primos_output.txt
	@echo "\033[92mTESTE numeros primos concluido!\033[0m"

test_fibonacci:
	@echo "Rodando teste sequencia de Fibonacci"
	./$(TARGET) $(TEST_DIR)/fibonacci.text.bin $(TEST_DIR)/fibonacci.data.bin 1 > fibonacci_output.txt
	diff $(EXPCT_DIR)/fibonacci_correct.txt fibonacci_output.txt
	@echo "\033[92mTESTE sequencia de Fibonacci concluido!\033[0m"

test_testador:
	@echo "Rodando teste de instruções"
	./$(TARGET) $(TEST_DIR)/testador.text.bin $(TEST_DIR)/testador.data.bin 1 > testador_output.txt
	diff $(EXPCT_DIR)/testador_correct.txt testador_output.txt
	@echo "\033[92mTESTE de instruções concluido!\033[0m"

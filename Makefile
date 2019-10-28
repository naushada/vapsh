CC = g++
FLAGS = -g -Wall
CPP = 
SRC_DIR = src
HDR_DIR = inc
OBJ_DIR = obj
BIN_DIR = bin
SRC = $(SRC_DIR)/readlineIF.cc $(SRC_DIR)/hostapdIF.cc $(SRC_DIR)/vapsh.cc
HDR = $(HDR_DIR)/readlineIF.h $(HDR_DIR)/hostapdIF.h $(HDR_DIR)/vapsh.h
OBJ = $(OBJ_DIR)/readlineIF.o $(OBJ_DIR)/hostapdIF.o $(OBJ_DIR)/vapsh.o
LIB_DIR = -L/usr/local/readline-8.0-rc1/lib -L/home/naushada/sw-packages/ACE_wrappers/lib
LIB = -lreadline -lACE -lpthread -ltinfo
INC = -Iinc -I/home/naushada/sw-packages/ACE_wrappers -I/usr/local/readline-8.0-rc1/include
BIN = $(BIN_DIR)/vapsh

all: build bin $(BIN)
.PHONY: build bin $(BIN)

build:
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(BIN_DIR)

bin: $(OBJ)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cc
	$(CC) $(FLAGS) -c $< -o $@ $(INC)

$(BIN): $(OBJ)
	$(CC) $(FLAGS) -o $@ $^ $(LIB_DIR) $(LIB)

	
.PHONY: clean

clean:
	@rm -f $(OBJ)
	@rm -f $(BIN)
	@rm -fr $(OBJ_DIR)
	@rm -fr $(BIN_DIR)

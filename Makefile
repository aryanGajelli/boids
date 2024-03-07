# Makefile for SDL 2.0.3

PROJ ?= boids
SRC_DIR ?= ./src
INC_DIR ?= ./inc
WORKING_DIR ?= ./build
BUILD_DIR := $(WORKING_DIR)/obj
BIN_DIR	:= $(WORKING_DIR)/bin

SRC_EXT := c
OBJ_EXT	:= o
DEFINES := -DSDL_MAIN_HANDLED -D_USE_MATH_DEFINES 

CC := gcc
CFLAGS := -std=c2x -g -Wall
LDLIBS := -L/mingw64/lib -lmingw32 -lSDL2main -lSDL2  -lSDL2_image -lSDL2_gfx -lgsl #-lSDL2_ttf -lSDL2_mixer


INC := -I$(INC_DIR) -I/mingw64/include/SDL2 #-I/usr/include/SDL2_image -I/usr/include/SDL2_ttf -I/usr/include/SDL2_mixer
SRC := $(wildcard $(SRC_DIR)/*.c)
OBJ := $(SRC:$(SRC_DIR)/%.$(SRC_EXT)=$(BUILD_DIR)/%.$(OBJ_EXT))

BIN := $(BIN_DIR)/$(PROJ)
COMPILE = $(CC) $(CFLAGS) $(INC) $(DEFINES)

all: $(BIN)

$(BUILD_DIR)/%.$(OBJ_EXT): $(SRC_DIR)/%.$(SRC_EXT)
	@echo 'Building project file: $<'
	@mkdir -p $(dir $@)
	@$(COMPILE) -c -o "$@" "$<"

$(BIN): $(OBJ)
	@echo 'Building target: $@'
	@mkdir -p $(BIN_DIR)
	$(CC) -o $(BIN) $(OBJ) $(LDFLAGS) ${LDLIBS}

clean:
	rm -rf $(WORKING_DIR)

run: $(BIN)
	$(BIN)
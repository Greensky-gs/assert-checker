CC=gcc
FLAGS=-Wall -Wextra -Wundef -Wpedantic -g
SANTS=-fsanitize=address,undefined
CLEANBUILD_FLAGS=-O3
LIBS_FLAG=

SRC_DIR=src
BUILD_DIR=build
BIN_DIR=bin

NAME=main.uwu

SOURCES_REG=$(wildcard $(SRC_DIR)/*.c)
SOURCES_DIR=$(wildcard $(SRC_DIR)/*/)
SOURCES_IND=$(wildcard $(SRC_DIR)/*/*.c)

SOURCES=$(SOURCES_REG) $(SOURCES_IND)

BUILD_DIRS=$(SOURCES_DIR:$(SRC_DIR)/%/=$(BUILD_DIR)/%/)
OBJECTS=$(SOURCES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

INT_VARS :=
VARS :=

MACRO_FLAGS := $(strip $(foreach v,$(VARS),\
  $(if $(strip $($(v))),-D$(v)="\"$($(v))\"")\
)) $(strip $(foreach v, $(INT_VARS), \
  $(if $(strip $($(v))),-D$(v)=$($(v)))\
))
all: $(BUILD_DIR) $(BUILD_DIRS) $(BIN_DIR)/$(NAME)

$(BIN_DIR)/$(NAME): $(BIN_DIR) $(OBJECTS)
	$(CC) $(OBJECTS) $(FLAGS) $(LIBS_FLAG) $(SANTS) $(MACRO_FLAGS) -o $(BIN_DIR)/$(NAME)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(FLAGS) $(LIBS_FLAG) $(MACRO_FLAGS) $^ -o $@ -c

$(BUILD_DIRS): $(BUILD_DIR)
	mkdir -p "$@"

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(BIN_DIR)

launch:
	./$(BIN_DIR)/$(NAME)

cleanbuild:
	make clean
	make FLAGS="$(CLEANBUILD_FLAGS) -DDISABLE_DEBUG=$(DISABLE_DEBUG)" SANTS= $(foreach v,$(VARS),$(v)="$($(v))")

full:
	make clean
	make
	make launch

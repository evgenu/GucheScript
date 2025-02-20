SRC_DIR := ../src
OBJ_DIR := obj
BIN_DIR := bin
CC := gcc
CFLAGS := -Wall -Wextra -O2 -MMD -MP
LDFLAGS :=

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
DEPS := $(OBJS:.o=.d)
TARGET := $(BIN_DIR)/guchec

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(LDFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

-include $(DEPS)

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all clean
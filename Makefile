# Makefile to compile C file into a .so file

# Name of module
NAME := flip_horizontally

# Compiler and compiler flags
CC := gcc
CFLAGS := -fPIC -Wall -Iinclude -I/usr/include/protobuf-c
LDFLAGS := -shared
LIBS := -lm -lpthread -lprotobuf-c # Link against the protobuf-c library

# Dependencies
DEPENDENCIES := src/include/module_config.pb-c.h src/include/config_util.h src/include/types.h

# Source files
SRC := src/module.c src/module_config.pb-c.c

# Object files
OBJS := $(SRC:.c=.o)

# Target library
TARGET_LIB := $(NAME).so

# Default target
all: $(TARGET_LIB)

$(TARGET_LIB): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)

%.o: %.c $(DEPENDENCIES)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET_LIB) $(OBJS)
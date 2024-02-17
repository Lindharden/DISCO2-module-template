# # Makefile to compile C file into a .so file

# # Name of module
# NAME := flip_horizontally

# # Compiler and compiler flags
# CC := gcc
# CFLAGS := -fPIC -Wall -Isrc/include -I/usr/include/protobuf-c
# LDFLAGS := -shared
# LIBS := -lm -lpthread -lprotobuf-c # Add external libraries

# # Dependencies
# DEPENDENCIES := src/include/module_config.pb-c.h src/include/config_util.h src/include/types.h src/include/module.h

# # Source files
# SRC := src/module.c src/module_config.pb-c.c

# # Object files
# OBJS := $(SRC:.c=.o)

# # Target library
# TARGET_LIB := $(NAME).so

# # Default target
# all: $(TARGET_LIB)

# $(TARGET_LIB): $(OBJS)
# 	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)

# %.o: %.c $(DEPENDENCIES)
# 	$(CC) $(CFLAGS) -c $< -o $@

# # Testing target
# test: src/test.c $(OBJS) $(DEPENDENCIES)
# 	$(CC) -g $(CFLAGS) -o test src/test.c $(OBJS) $(LIBS)

# clean:
# 	rm -f $(TARGET_LIB) $(OBJS) test



# Makefile to build and run a C program with dependencies

# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -std=c11

# Include directories
INCLUDES = -Isrc/include -I/usr/include/protobuf-c

# Source files
SRC = src/test.c src/module.c src/module_config.pb-c.c

# Object files
OBJ = $(SRC:.c=.o)

# External library
LIBS = -lm -lpthread -lprotobuf-c

# Executable file
EXEC = test

# Default target
all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -g $(CFLAGS) $^ -o $@ $(LIBS)

# Compile C source files into object files
%.o: %.c
	$(CC) -g $(CFLAGS) $(INCLUDES) -c $< -o $@

# Target to run the program
run: $(EXEC)
	./$(EXEC)

# Target to clean up
clean:
	rm -f $(EXEC) $(OBJ)

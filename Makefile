# Name of the module and executable
LIB_NAME := flip_horizontally
EXE_NAME := test

# Compiler and compiler flags
CC := gcc
CFLAGS := -Wall -Isrc/include -I/usr/include/protobuf-c
LDFLAGS := -L. # Specify the local directory for library lookup during linking
LIBS := -lm -lpthread -lprotobuf-c

# Source files for the library and executable
LIB_SRC := src/module.c src/module_config.pb-c.c
EXE_SRC := src/test.c

# Object files for the library
LIB_OBJS := $(LIB_SRC:.c=.o)

# Shared library target
LIB_TARGET := $(LIB_NAME).so

# Default target to build both the library and the executable
all: $(LIB_TARGET) $(EXE_NAME)

# Rule to compile the shared library
$(LIB_TARGET): $(LIB_OBJS)
	$(CC) $(CFLAGS) -shared -o $@ $^ $(LIBS)

# Rule to compile the executable
$(EXE_NAME): $(EXE_SRC) $(LIB_TARGET)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS) ./$(LIB_TARGET) $(LIBS)

# Rule to compile object files from source
%.o: %.c
	$(CC) $(CFLAGS) -fPIC -c $< -o $@

clean:
	rm -f $(LIB_TARGET) $(LIB_OBJS) $(EXE_NAME)
	
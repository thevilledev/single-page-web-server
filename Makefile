# A single-page web server.
# Threaded.
# HTTP/1.0.
# Simple as that.
#
# Makefile
#
# Author:  Ville Törhönen <ville@torhonen.fi>
# Date:    01-24-2015
# License: MIT License

# Directory structure and the output binary file.
BIN=single-page-web-server
BIN_DIR=bin
SRC_DIR=src
OBJ_DIR=obj

# Compiler and linker flags.
CC=gcc
CCFLAGS=-lpthread -Os -s
LDFLAGS=$(CCFLAGS)

# Gather a list of our sources and objects.
sources := $(wildcard src/*.c)
objects := $(addprefix obj/,$(notdir $(sources:.c=.o)))

###########
# Targets #
###########

default: $(BIN_DIR)/$(BIN)

$(BIN_DIR)/$(BIN): $(objects)
	@mkdir -p $(@D)
	$(CC) $(LDFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CCFLAGS) -c -o $@ $<

clean:
	rm -f $(objects)

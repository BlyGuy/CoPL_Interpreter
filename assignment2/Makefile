# Makefile for building the Interpreter binary
# Credits: 
#    - Marc Hoek(s3157520), 
#    - Joshua J. Lelipaly(s3329941)
#    - Original Makefile template from slides "Programmeertechnieken Week 13" (2022)  by J.K. Vis
# Date: 2023-3-12

#workspace settings
INC_DIR = include
SRC_DIR = src
SOURCES = $(shell find $(SRC_DIR)/ -name '*.cc')
OBJECTS = $(SOURCES:.cc=.o)
DEPS = $(OBJECTS:.o=.d)
TARGET = main

# CXX compiler settings
CC = g++
CFLAGS = -march=native -I$(INC_DIR) -Wall -Wextra -Wpedantic

.PHONY: all exec clean debug release

#release build
release: CFLAGS += -O3 -DNDEBUG
release: all

#debugging build
debug: CFLAGS += -O0 -DDEBUG -ggdb3
debug: all

#builds the TARGET binary by linking all compiled object-files
all: $(TARGET)
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

#compiles every .cc file in SRC_DIR
-include $(DEPS)
%.o: %.cc
	$(CC) $(CFLAGS) -MMD -o $@ -c $<

#utility commands
exec:
	./${TARGET}
clean:
	rm -f ${OBJECTS} ${DEPS} ${TARGET}

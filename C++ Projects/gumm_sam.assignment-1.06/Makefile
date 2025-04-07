# Compiler settings
CXX      = g++
CC       = gcc
CXXFLAGS = -Wall -Wextra -std=c++11 -O2
CFLAGS   = -Wall -Wextra -O2
LDFLAGS  = -lncurses

# Source files
CPP_SRC  = dungeon.cpp
C_SRC    = heap.c
CPP_OBJ  = $(CPP_SRC:.cpp=.o)
C_OBJ    = $(C_SRC:.c=.o)
TARGET   = dungeon

all: $(TARGET)

$(TARGET): $(CPP_OBJ) $(C_OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(CPP_OBJ) $(C_OBJ) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(CPP_OBJ) $(C_OBJ) $(TARGET)

clobber: clean

.PHONY: all clean clobber

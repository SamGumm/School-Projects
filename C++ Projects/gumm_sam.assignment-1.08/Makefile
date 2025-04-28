# Compiler and flags
CXX := g++
CC := gcc
CXXFLAGS := -std=c++17 -Wall -Wextra -I.
CFLAGS = -Wall -Wextra -O2
LDFLAGS := -lncurses

# Source, object, and header files
CPP_SRCS := $(wildcard *.cpp)
C_SRCS := $(wildcard *.c)
CPP_OBJS := $(CPP_SRCS:.cpp=.o)
C_OBJS := $(C_SRCS:.c=.o)
CPP_HDRS := $(wildcard *.hpp)
C_HDRS := $(wildcard *.h)

# Default target
all: dungeon

# Link step
dungeon: $(CPP_OBJS) $(C_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Compile step
%.o: %.cpp $(HDRS)
	$(CXX) $(CXXFLAGS) -c $< -o $@
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f $(CPP_OBJS) $(C_OBJS) dungeon

.PHONY: all clean

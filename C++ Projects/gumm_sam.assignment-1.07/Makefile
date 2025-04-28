# Compiler settings
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11 -O2

OBJS = parser.cpp parser.h

TARGET = parser

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

clean:
	rm -f *.o $(TARGET)

clobber: clean

.PHONY: all clean clobber

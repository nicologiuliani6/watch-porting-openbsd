# Makefile for C++ watch porting

CXX = c++
CXXFLAGS = -std=c++17 -Wall -Wextra -pthread
TARGET = watch
SRC = watch.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)


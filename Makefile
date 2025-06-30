# Simple Makefile for CustomPreter

CXX = g++
CXXFLAGS = -std=c++17 -Iinclude
SRC = src/Token.cpp src/Scanner.cpp src/Parser.cpp src/AstPrinter.cpp src/Interpreter.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = custompreter

all: $(TARGET)

$(TARGET): $(SRC) main.cpp
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC) main.cpp

clean:
	rm -f $(TARGET) $(OBJ)

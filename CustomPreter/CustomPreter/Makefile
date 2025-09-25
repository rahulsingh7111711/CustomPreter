CXX = g++
CXXFLAGS = -std=c++17 -Wall -g

SRCS = main.cpp lexer.cpp parser.cpp ast.cpp interpreter.cpp environment.cpp
OBJS = $(SRCS:.cpp=.o)

TARGET = mini_lang

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f $(OBJS) $(TARGET)

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -Iinclude -Ilib


# Source files and output binary
SOURCES = main.cpp src/JpegDecoder.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXEC = JpegDecoder

# Default target
all: $(EXEC)

# Rule to build the executable
$(EXEC): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(EXEC)

# Rule to build object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule to remove object files and the executable
clean:
	rm -f $(OBJECTS) $(EXEC)

# Run the program
run: $(EXEC)
	./$(EXEC)


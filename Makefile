# Compiler
CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -Werror -pedantic

# Executable
TARGET = myrobot

# Source files
SRCS = main.cpp Vacuum.cpp CleaningAlgo.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Build the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Compile source files to object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build files
.PHONY: clean
clean:
	rm -f $(TARGET) $(OBJS)

# Run the program with an input file
.PHONY: run
run:
	@echo "Usage: ./$(TARGET) <input_file>"
	@echo "Example: ./$(TARGET) input_a.txt"

# Example usage to create output file
run-example:
	./$(TARGET) input_a.txt
	./$(TARGET) input_b.txt
	./$(TARGET) input_c.txt

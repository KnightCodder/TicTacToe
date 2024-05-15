# Compiler
CXX = g++

# Compilation flags
CXXFLAGS = -O0

# Main target
all: engine.o
	$(CXX) engine.o -o output
	./output

# Rule to compile individual source files into object files
engine.o: engine.cpp
	$(CXX) -c $< $(CXXFLAGS) -o $@

createApp:
	$(CXX) engine.o -o TicTacToe -static-libgcc -static-libstdc++

# Rule to clean all the .o and executables
clean:
	rm -f *.o output TicTacToe

CXX = g++
INCLUDE_FLAGS = -I ext/libelfin -I ext/linenoise # '-I' flag tells g++ which dir to look into for header files
CXXFLAGS = -std=c++14 -O2 -Wall $(INCLUDE_FLAGS)

debugger: LinuxDebug.o linenoise.o
	$(CXX) $(CXXFLAGS) LinuxDebug.o linenoise.o -o debugger

LinuxDebug.o: src/LinuxDebug.cpp
	$(CXX) $(CXXFLAGS) -c src/LinuxDebug.cpp


# Linking linenoise submodule 
linenoise.o: ext/linenoise/linenoise.c 
	gcc -Wall -W -Os -g -c ext/linenoise/linenoise.c


all: debugger

run: debugger
	./debugger

clean:
	rm -rf debugger *.o
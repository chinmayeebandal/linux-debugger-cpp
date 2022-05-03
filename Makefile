CXX = g++
CXXFLAGS = -std=c++14 -O2 -Wall

debugger: LinuxDebug.o 
	$(CXX) $(CXXFLAGS) LinuxDebug.o -o debugger

LinuxDebug.o: src/LinuxDebug.cpp
	$(CXX) $(CXXFLAGS) -c src/LinuxDebug.cpp

all: debugger

run: debugger
	./debugger

clean:
	rm -rf debugger *.o
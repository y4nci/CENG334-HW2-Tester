CC=g++
CFLAGS=-ansi -Wall -pedantic-errors -O0 -std=c++11
LDFLAGS=

INTERFACES=src/tools/matrix.cpp src/tools/TesterModule.cpp src/lib/arguments.cpp src/lib/Fork.cpp
HEADERS=src/tools/matrix.h src/tools/TesterModule.h src/lib/arguments.h src/lib/Fork.h

all: tester-hw2

tester-hw2: src/tester-hw2.cpp $(INTERFACES) $(HEADERS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< $(INTERFACES)

clean:
	rm -f tester-hw2

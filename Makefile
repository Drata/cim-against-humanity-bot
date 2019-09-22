# Compiler
CC = g++

# Create
all: cim_against_humanity clean

cim_against_humanity: cim_against_humanity.o
	rm -f bin/cim_against_humanity
	$(CC) -o bin/cim_against_humanity main.o

cim_against_humanity.o: src/main.cpp
	$(CC) -c src/main.cpp

clean:
	$(RM) count *.o *~
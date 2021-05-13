CC=g++
CFLAGS=-c

all: main

main: main.o Actor.o
	$(CC) main.o -o main Actor.o -L/SFML-2.5.1/include -lsfml-graphics -lsfml-window -lsfml-system

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp

Actor.o: Actor.cpp
	$(CC) $(CFLAGS) Actor.cpp

clean:
	rm -rf *.o main
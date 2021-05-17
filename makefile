CC=g++
CFLAGS=-c
OFLAGS=-o
LDFLAGS=-LSFML-2.5.1/lib
INCLUDE=-ISFML-2.5.1/include
# it's needed to be fixed
LIBFLAGES=-lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lopengl32 -lwinmm -lgdi32 -DSFML_STATIC
RM=rm

# if OS = Windows
ifdef OS
	RM=del
	LIBFLAGES=-lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lopengl32 -lwinmm -lgdi32 -DSFML_STATIC
endif

all: main

main: main.o Actor.o
	$(CC) $(OFLAGS) main main.o Actor.o $(INCLUDE) $(LDFLAGS) $(LIBFLAGES)

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp $(INCLUDE) $(LIBFLAGES)

Actor.o: Actor.cpp
	$(CC) $(CFLAGS) Actor.cpp $(INCLUDE) $(LIBFLAGES)

clean:
	$(RM) *.o main
CC=g++
CFLAGS=-c
OFLAGS=-o
LDFLAGS=-LSFML/Linux/lib
INCLUDE=-ISFML/Linux/include
# it's needed to be fixed
LIBFLAGES=-lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lGL -lfreetype -lX11 -DSFML_STATIC
RM=rm -f

# if OS = Windows
ifdef OS
	RM=del
	LIBFLAGES=-DSFML_STATIC -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lopengl32 -lwinmm -lgdi32
endif

all: main

main: main.o Actor.o
	$(CC) $(OFLAGS) main main.o Actor.o $(INCLUDE) $(LDFLAGS) -Wall -Wl,--start-group $(LIBFLAGES) -Wl,--end-group -ldl

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp $(INCLUDE)

Actor.o: Actor.cpp
	$(CC) $(CFLAGS) Actor.cpp $(INCLUDE)

clean:
	$(RM) *.o main
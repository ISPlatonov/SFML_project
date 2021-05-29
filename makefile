CC=g++ -static
CFLAGS=-c
OFLAGS=-o
LDFLAGS=-LSFML/Linux/lib
INCLUDE=-ISFML/Linux/include
# it's needed to be fixed
LIBFLAGES=-lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lGL -lfreetype -lX11 -DSFML_STATIC
RM=rm

# if OS = Windows
ifdef OS
	RM=del
	LIBFLAGES=-lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lopengl32 -lwinmm -lgdi32 -mwindows -DSFML_STATIC
	LDFLAGS=-LSFML/Windows/lib
	INCLUDE=-ISFML/Windows/include
endif

all: main

main: main.o Actor.o
	$(CC) $(OFLAGS) main main.o Actor.o $(INCLUDE) $(LDFLAGS) $(LIBFLAGES)

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp $(INCLUDE) $(LIBFLAGES)

Actor.o: Actor.cpp
	$(CC) $(CFLAGS) Actor.cpp $(INCLUDE) $(LIBFLAGES)

clean:
	$(RM) *.o main main.exe
CC=gcc
SDL_FLAGS=-lSDL -lSDL_image -lSDL_ttf
CFLAGS=${SDL_FLAGS} --std=c99 -Wall -pedantic -ggdb3

main: main.o game.o
	$(CC) $(CFLAGS) -o main main.o game.o

main.o: main.c
	$(CC) $(CFLAGS) -c main.c game.h

game.o: game.c
	$(CC) $(CFLAGS) -c game.c game.h

clean:
	rm main main.o game.o *.gch

test:
	./main & sleep 1 && killall -9 main

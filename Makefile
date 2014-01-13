CFLAGS=-std=c99 -O2

all:
	gcc $(CFLAGS) main.c -o clonopoly -lSDL -lSDL_image -lSDL_gfx

tags:
	ctags -R -f tags /usr/include/SDL/*.h *.c *.h

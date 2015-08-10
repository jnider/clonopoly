CFLAGS=-std=c99 -O2

#libsdl-gfx1.2-dev
#libsdl-image1.2-dev
#libsdl-ttf2.0-dev

C_SRC=main.c ui.c list.c options.c status.c

CFLAGS += -DDEBUG

.PHONY:
	tags

all:
	gcc $(CFLAGS) $(C_SRC) -o clonopoly -Wall -lSDL -lSDL_image -lSDL_gfx -lSDL_ttf


clean:
	rm -f *.o
	rm -f clonopoly

tags:
	ctags -R -f tags . /usr/include/SDL/

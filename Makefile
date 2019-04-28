CFLAGS=-O2 -std=c++11

#libsdl-gfx1.2-dev
#libsdl-image1.2-dev
#libsdl-ttf2.0-dev
#fonts-freefont-ttf


CPP_SRC=main.c ui.c ui_game_sdl.cpp list.c options.c board.cpp game.cpp player.cpp

CFLAGS += -DDEBUG

.PHONY:
	tags

all:
	g++ $(CFLAGS) $(CPP_SRC) -o clonopoly -Wall -lSDL2 -lSDL2_image -lSDL2_gfx -lSDL2_ttf


clean:
	rm -f *.o
	rm -f clonopoly

tags:
	ctags -R -f tags . /usr/include/SDL2

prereq:
	apt install libsdl2-dev libsdl2-ttf-dev libsdl2-net-dev libsdl2-image-dev libsdl2-gfx-dev

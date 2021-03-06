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
	g++ $(CFLAGS) $(CPP_SRC) -o clonopoly -Wall -lSDL -lSDL_image -lSDL_gfx -lSDL_ttf


clean:
	rm -f *.o
	rm -f clonopoly

tags:
	ctags -R -f tags . /usr/include/SDL/

CFLAGS=-O2 -std=c++11

#libsdl-gfx1.2-dev
#libsdl-image1.2-dev
#libsdl-ttf2.0-dev
#fonts-freefont-ttf


CPP_SRC=main.cpp list.c board.cpp game.cpp player.cpp
LIBS=

ifeq ($(DEBUG), y)
CFLAGS += -DDEBUG
endif

# enable SDL 1.2
ifeq ($(UI_SDL12), y)
	CPP_SRC += ui_sdl12.cpp
endif

# enable SDL 2.0
ifeq ($(UI_SDL20), y)
	CPP_SRC += ui_game_sdl.cpp
	LIBS += -lSDL2 -lSDL2_image -lSDL2_gfx -lSDL2_ttf
endif

# always allow text UI
CPP_SRC += ui_text.cpp
CFLAGS += -DUI_TEXT

.PHONY: tags

all:
	g++ $(CFLAGS) $(CPP_SRC) -o clonopoly -Wall $(LIBS)

clean:
	rm -f *.o
	rm -f clonopoly

tags:
	ctags -R -f tags . /usr/include/SDL2

prereq:
	apt install libsdl2-dev libsdl2-ttf-dev libsdl2-net-dev libsdl2-image-dev libsdl2-gfx-dev

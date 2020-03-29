#include <cstdio>
#include "game.h"

#ifdef UI_TEXT
#include "ui_text.h"
#endif // UI_TEXT

#ifdef DEBUG
#define DBG_PRINT fprintf
#else
#define DBG_PRINT(...)
#endif // DEBUG

int main(int argc, char** argv)
{
   CGame *game = new CGame(new CTextUI);

	game->Initialize();

   // start the game
	game->NewGame();
   DBG_PRINT(stderr, "Running\n");
   int ret = game->Play();
	switch (ret)
	{
	case ERR_NO_PLAYERS:
		DBG_PRINT(stderr, "Not enough players defined\n");
		break;
	}

   DBG_PRINT(stderr, "done\n");

   delete game;

   return 0;
}

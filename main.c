#include <stdlib.h>
#include <string.h>
#include <time.h>
//#include "SDL/SDL.h"
//#include "SDL/SDL_image.h"
//#include "SDL/SDL_gfxPrimitives.h"
//#include "SDL/SDL_ttf.h"
#include "game.h"
#include "id.h"
#include "ui_game_sdl.h"

static void OnMouseClick(Ifel* i);
static CGame *game;
static CGameUI *gameUI;

/* A generic function to handle all mouse clicks */
/* Most likely this should be broken up for more modular logic */
static void OnMouseClick(Ifel* i)
{
   //fprintf(stderr, "OnMouseClick in %i\n", i->id);
   switch(i->id)
   {
   case ID_BTN_OPTIONS:
      //fprintf(stderr, "Click in options button\n");
      gameUI->ShowOptionsMenu();
      break;

   case ID_MSGBOX_ROLL:
   case ID_BTN_ROLL:
      game->RollDice();
      game->MovePlayer(ACCORDING_TO_DICE);
      break;
   }
}

void OnKeyPressed(Ifel* el, int key)
{
   switch(key)
   {
   case 'q':
      fprintf(stderr, "Quit\n");
      if (ModalMessageBox(ID_MB_YESNO, "Would you like to quit the current game?") == MB_YES)
      {
         game->Over();
         Quit();
      }
      break;

   case 'y':
      break;

   case 'n':
      game->NewGame();
      break;
   }
}

int main(int argc, char* args[])
{
   gameUI = new CSDLGameUI();
   game = new CGame(*gameUI);

   gameUI->Init(OnKeyPressed, OnMouseClick);

   // start from a clean slate
   game->Over();

   // start the game
   Run();

   fprintf(stderr, "Save game\n");
   if (!game->IsOver())
   {
      if (ModalMessageBox(ID_MB_YESNO, "Would you like to save the current game?") == MB_YES)
      {
         //SaveGame("game1.clonopoly");
      }
   }

   fprintf(stderr, "Cleanup\n");

   delete gameUI;
   delete game;

   return 0;
}


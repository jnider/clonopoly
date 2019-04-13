#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_gfxPrimitives.h"
#include "SDL/SDL_ttf.h"
#include "game.h"
//#include "ui.h"
#include "id.h"
//#include "board.h"
#include "ui_game_sdl.h"

static void OnMouseClick(Ifel* i);
static CGame *game;
static CGameUI *gameUI;


void GameOver(void)
{
   game->Over();
}

void PayMoney(int from, int to, int amount)
{
   game->PayMoney(from, to, amount);
}

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

/*
   if (messageBox)
   {
      DeleteMessageBox(messageBox);
      messageBox = NULL;
   }
*/
   //fprintf(stderr, "OnMouseClick complete\n");
}

void OnKeyPressed(Ifel* el, int key)
{
   switch(key)
   {
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

   if (InitUI() != 0)
   {
      fprintf(stderr, "Can't init UI\n");
      return 1;
   }

   gameUI->Init(OnKeyPressed, OnMouseClick);

   // start from a clean slate
   GameOver();

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
/*
   if (messageBox)
   {
      DeleteMessageBox(messageBox);
      messageBox = NULL; // not completely necessary, but a good habit
   }
*/

   //DeleteStatusArea();

   delete gameUI;
   delete game;

   return 0;
}


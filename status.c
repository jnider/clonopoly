#include "ui.h"
#include "id.h"
#include "options.h"
#include "SDL/SDL_gfxPrimitives.h"
#include "game.h"

struct StatusArea
{
   Ifel* ifel;
   SDL_Surface* surface;
   Player* player;
};

// our 'singleton'
static struct StatusArea* status;

static void DrawStatusArea(Ifel* i)
{
   struct StatusArea* s = (struct StatusArea*)i->data;
   SDL_Surface* name;
   SDL_Surface* money;
   SDL_Rect loc;

   // background
   roundedBoxRGBA(s->surface, 0, 0, s->surface->w, s->surface->h, 10, 100, 100, 0, 100);
   roundedBoxRGBA(s->surface, 6, 6, s->surface->w-10, s->surface->h-10, 10, 0, 80, 0, 255);

   // current player's name
   SDL_Color textColor;
   textColor.r = 0x8F;
   textColor.g = 0x80;
   textColor.b = 0xFF;
   loc.x = 10;
   loc.y = 6;
   name = TTF_RenderText_Solid(font, status->player->name, textColor);
   if (!name)
   {
      fprintf(stderr, "Error rendering name\n");
   }
   SDL_BlitSurface(name, NULL, s->surface, &loc);
   SDL_FreeSurface(name);

   // remaining money
   char tempText[64];
   sprintf(tempText, "$%i.00", status->player->money);
   money = TTF_RenderText_Solid(font, tempText, textColor);
   loc.y += 26;
   if (money)
   {
      SDL_BlitSurface(money, NULL, s->surface, &loc);
      SDL_FreeSurface(money);
   }

   SDL_BlitSurface(s->surface, NULL, i->surface, NULL);
}

int CreateStatusArea(const SDL_Rect* loc)
{
   // if it doesn't exist yet, create it
   if (!status)
   {
      status = malloc(sizeof(struct StatusArea));
      if (!status)
         return 1;

      // create the surface
      status->surface = SDL_CreateRGBSurface(SDL_SWSURFACE, loc->w, loc->h, 32, 0, 0, 0, 0);
      if (!status->surface)
      {
         fprintf(stderr, "Error creating status area surface\n");
         free(status);
         status = NULL;
         return 2;
      }

      // create the ifel
      status->ifel = malloc(sizeof(struct Ifel));
      if (!status->ifel)
      {
         free(status);
         SDL_FreeSurface(status->surface);
         return 3;
      }
      status->ifel->loc.x = loc->x;
      status->ifel->loc.y = loc->y;
      status->ifel->loc.w = loc->w;
      status->ifel->loc.h = loc->h;
      CreateIfel(ID_MENU_OPTIONS, NULL, status->ifel, IFEL_CUSTOM, DrawStatusArea);
      status->ifel->data = status;
   }

   // set it active
   status->ifel->active = 1;

   return 0;
}

void DeleteStatusArea(void)
{
   if (status)
   {
      DeleteIfel(NULL, status->ifel);
      free(status->ifel);
      free(status);
      status = NULL;
   }
}

void SetCurrentPlayerStatusArea(Player* p)
{
   status->player = p;   
}

void DisableStatusArea(void)
{
   status->ifel->active = 0;
}

void EnableStatusArea(void)
{
   status->ifel->active = 1;
}

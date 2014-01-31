#include "ui.h"
#include "id.h"
#include "options.h"
#include "SDL/SDL_gfxPrimitives.h"

struct OptionsMenu
{
   Ifel* i;
   SDL_Surface* surface;
};

// our 'singleton'
static struct OptionsMenu* menu;

static void DrawOptionsMenu(SDL_Surface* s, Ifel* i)
{
   struct OptionsMenu* menu = (struct OptionsMenu*)i->data;

   // draw it
   roundedBoxRGBA(menu->surface, 0, 0, 100, 200, 10, 0, 255, 0, 100);

   SDL_BlitSurface(menu->surface, NULL, s, NULL);
}

void ShowOptionsMenu(void)
{
   // if it doesn't exist yet, create it
   if (!menu)
   {
      menu = malloc(sizeof(struct OptionsMenu));
      if (!menu)
         return;

      // create the surface
      menu->surface = SDL_CreateRGBSurface(SDL_SWSURFACE, 100, 200, 32, 0, 0, 0, 0);
      if (!menu->surface)
      {
         fprintf(stderr, "Error creating options menu surface\n");
         free(menu);
         menu = NULL;
         return;
      }

      // create the ifel
      menu->i = CreateIfel(ID_MENU_OPTIONS, NULL, DrawOptionsMenu);
      menu->i->data = menu;
   }

   // set it active
   menu->i->active = 1;
}

void DeleteOptionsMenu(void)
{
   if (menu)
   {
      RemoveIfel(NULL, menu->i);
      free(menu);
      menu = NULL;
   }
}


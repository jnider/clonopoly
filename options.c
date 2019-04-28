#include "ui.h"
#include "id.h"
#include "options.h"
#include "SDL2/SDL2_gfxPrimitives.h"

#define WIDTH 300
#define HEIGHT 200

struct OptionsMenu
{
   Ifel* ifel;
   //SDL_Surface* surface;
   SDL_Renderer *renderer;
   Button* cancelBtn;
};

// our 'singleton'
static struct OptionsMenu* menu;

static void DrawOptionsMenu(Ifel* i)
{
   struct OptionsMenu* menu = (struct OptionsMenu*)i->data;

   //fprintf(stderr, "DrawOptionsMenu\n");

   // draw it
   roundedBoxRGBA(menu->renderer, 0, 0, WIDTH, HEIGHT, 10, 0, 255, 0, 100);

   //SDL_BlitSurface(menu->surface, NULL, i->surface, NULL);
}

void ShowOptionsMenu(SDL_Renderer *renderer)
{
   // if it doesn't exist yet, create it
   if (!menu)
   {
      menu = (struct OptionsMenu*)malloc(sizeof(struct OptionsMenu));
      if (!menu)
         return;

/*
      // create the surface
      menu->surface = SDL_CreateRGBSurface(SDL_SWSURFACE, WIDTH, HEIGHT, 32, 0, 0, 0, 0);
      if (!menu->surface)
      {
         fprintf(stderr, "Error creating options menu surface\n");
         free(menu);
         menu = NULL;
         return;
      }
*/

      // create the ifel
      menu->ifel = (struct Ifel*)malloc(sizeof(struct Ifel));
      if (!menu->ifel)
      {
         free(menu);
         //SDL_FreeSurface(menu->surface);
         return;
      }
      menu->ifel->loc.x = 100;
      menu->ifel->loc.y = 100;
      menu->ifel->loc.w = WIDTH;
      menu->ifel->loc.h = HEIGHT;
      CreateIfel(ID_MENU_OPTIONS, NULL, menu->ifel, IFEL_CUSTOM, DrawOptionsMenu);
      menu->ifel->data = menu;

      // add a cancel button 
      menu->cancelBtn = CreatePngButton(ID_BTN_CANCEL, menu->ifel, 50, 60, "graphics/cancel1.png", "graphics/cancel2.png", "graphics/cancel3.png");
   }

   // set it active
   menu->ifel->active = 1;
}

void DeleteOptionsMenu(void)
{
   if (menu)
   {
      DeleteIfel(NULL, menu->ifel);
      free(menu->ifel);
      free(menu);
      menu = NULL;
   }
}


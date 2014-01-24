#include "SDL/SDL_image.h"
#include "SDL/SDL_gfxPrimitives.h"
#include "SDL/SDL_ttf.h"
#include "ui.h"
#include "list.h"

#define MESSAGE_BOX_MARGIN 10

extern void OnKeyPressed(int key);

// SDL stuff
static Ifel* currentEl;
static SDL_Surface* screen;
static TTF_Font* font;
static list* ifels;

static int AddIfel(Ifel* parent, Ifel* child)
{
   if (!parent)
   {
      // top level window
      ListAddNode(ifels, child);
   }
   else
   {
      ListAddNode(&parent->ifels, child);
   }

   return 0;
}

static int RemoveIfel(Ifel* parent, Ifel* child)
{
   list* l;

   if (!parent)
      l = ifels;
   else
      l = &parent->ifels;

   ListRemoveNode(l, child);
   return 0;
}

Ifel* GetFirstIfel(Ifel* parent, iterator* iter)
{
   if (!parent)
   {
      *iter = ifels->first;
      if (ifels->size == 0)
         return NULL;
      return (Ifel*)ListGetData(ifels->first);
   }
}

Ifel* GetNextIfel(iterator* item)
{
   node* n = *item;
   *item = n->next;
   if (n->next)
      return (Ifel*)ListGetData(n->next);
   else
      return NULL;
}

void DrawMessageBox(Ifel* i)
{
   MessageBox* mb = (MessageBox*)i;
   fprintf(stderr, "DrawMessageBox\n");
   SDL_BlitSurface(mb->surface, NULL, screen, &mb->el.loc);
}

MessageBox* CreateMessageBox(int id, const char* msg)
{
   SDL_Surface* text;
   MessageBox* mb = malloc(sizeof(MessageBox));
   fprintf(stderr, "Creating messagebox\n");
   if (!mb)
   {
      fprintf(stderr, "Error creating messagebox\n");
      return NULL;
   }
   memset(mb, 0, sizeof(MessageBox));
   
   // render text to determine its size
   if (font)
   {
      SDL_Color textColor;
      textColor.r = 0xFF;
      textColor.g = 0;
      textColor.b = 0;
      text = TTF_RenderText_Solid(font, msg, textColor);
   }

   // calculate windows size based on text & icons
   mb->el.loc.x = 0;
   mb->el.loc.y = 0;

   if (text)
   {
      mb->el.loc.w = text->w + (MESSAGE_BOX_MARGIN * 2);
      mb->el.loc.h = text->h + (MESSAGE_BOX_MARGIN * 2);
   }
   
   // create the drawing surface
   mb->surface = SDL_CreateRGBSurface(SDL_SWSURFACE, mb->el.loc.w, mb->el.loc.h, 32, 0, 0, 0, 0);
   if (!mb->surface)
   {
      fprintf(stderr, "Error creating window surface\n");
      free(mb);
      return NULL;
   }
   SDL_SetColorKey(mb->surface, SDL_SRCCOLORKEY, 0);

   // draw the message and the box
   roundedBoxRGBA(mb->surface, 0, 0, mb->el.loc.w, mb->el.loc.h, 10, 0xF0, 0, 0, 0x30);
   if (text)
   {
      SDL_Rect loc;
      loc.x = (mb->el.loc.w - text->w) / 2;
      loc.y = (mb->el.loc.h - text->h) / 2;
      loc.w = text->w;
      loc.h = text->h;
      SDL_BlitSurface(text, NULL, mb->surface, &loc);
      SDL_FreeSurface(text);
   }
      //stringColor(mb->surface, 0, (wnd->size.h+10)/2, msg, 0xFFF0F0FF);

   // set the location of the window on the screen
   mb->el.loc.x = (screen->w - mb->el.loc.w) / 2;
   mb->el.loc.y = (screen->h - mb->el.loc.h) / 2;

   // set the internal attributes
   mb->el.draw = DrawMessageBox;
   mb->el.id = id;
   mb->el.type = IFEL_MESSAGEBOX;
   mb->el.active = 1;
   
   // add it to the list of windows to draw
   AddIfel(NULL, (Ifel*)mb);

   return mb;
}

void DeleteMessageBox(MessageBox* mb)
{
   RemoveIfel(NULL, (Ifel*)mb);
   free(mb);
}

void DestroyMessageBox(MessageBox* mb)
{
   SDL_FreeSurface(mb->surface);
   free(mb);
}

void DrawImage(Ifel* i)
{
   Image* img = (Image*)i;
   SDL_BlitSurface(img->surface, NULL, screen, &img->el.loc);
}

Image* CreateImage(int id, const char* bitmap)
{
   fprintf(stderr, "Creating image id %i\n", id);

   // create the object
   Image* img = malloc(sizeof(Image));
   if (!img)
   {
      fprintf(stderr, "Can't create image %s\n", bitmap);
      return NULL;
   }
   memset(img, 0, sizeof(Image));

   // load the image from disk
   img->surface = IMG_Load(bitmap);
   if (!img->surface)
   {
      fprintf(stderr, "Can't load image %s\n", bitmap);
      free(img);
      return NULL;
   }

   fprintf(stderr, "Loaded\n");

   // set the internal options
   img->el.active = 1;
   img->el.id = id;
   img->el.type = IFEL_IMAGE;
   img->el.draw = DrawImage;

   // add it to the list of windows to draw
   fprintf(stderr, "Adding ifel\n");
   AddIfel(NULL, (Ifel*)img);

   fprintf(stderr, "CreateImage done\n");
   return img;
}

void DeleteImage(Image* img)
{
   fprintf(stderr, "Unloading image\n");
   if (!img)
      return;

   RemoveIfel(NULL, (Ifel*)img);
   SDL_FreeSurface(img->surface);
   free(img);
}

void DrawButton(Ifel* i)
{
   Button* btn = (Button*)i;
   //fprintf(stderr, "DrawButton\n");
   switch(btn->state)
   {
   case BUTTON_NORMAL:
      SDL_BlitSurface(btn->up, NULL, screen, &btn->el.loc);
      break;
   case BUTTON_PRESSED:
      SDL_BlitSurface(btn->down, NULL, screen, &btn->el.loc);
      break;
   case BUTTON_HOVER:
      SDL_BlitSurface(btn->hover, NULL, screen, &btn->el.loc);
      break;
   }
}

Button* CreateButton(int id, int x, int y, const char* img_up, const char* img_down, const char* img_hover)
{
   fprintf(stderr, "CreateButton\n");
   Button* button = malloc(sizeof(Button));
   if (!button)
   {
      fprintf(stderr, "Error allocating button\n");
      return NULL;
   }

   memset(button, 0, sizeof(Button));

   fprintf(stderr, "Loading %s\n", img_up);
   button->up = IMG_Load(img_up);
   if (!button->up)
   {
      fprintf(stderr, "Can't load image %s\n", img_up);
      goto img_up_fail;
   }
   fprintf(stderr, "Loading %s\n", img_down);
   button->down = IMG_Load(img_down);
   if (!button->down)
   {
      fprintf(stderr, "Can't load image %s\n", img_down);
      goto img_down_fail;
   }
   fprintf(stderr, "Loading %s\n", img_hover);
   button->hover = IMG_Load(img_hover);
   if (!button->hover)
   {
      fprintf(stderr, "Can't load image %s\n", img_hover);
      goto img_hover_fail;
   }
   button->el.loc.x = 0;
   button->el.loc.y = 708;
   button->el.loc.w = button->up->w;
   button->el.loc.h = button->up->h;

   // set the internal variables
   button->el.draw = DrawButton;
   button->el.id = id;
   button->el.type = IFEL_BUTTON;
   button->el.active = 1;
   button->state = BUTTON_NORMAL;

   // add it to the list of windows to draw
   AddIfel(NULL, (Ifel*)button);

   return button;

img_hover_fail:
   SDL_FreeSurface(button->down);
img_down_fail:
   SDL_FreeSurface(button->up);
img_up_fail:
   free(button);
   return NULL;
}

void DeleteButton(Button* btn)
{
   RemoveIfel(NULL, (Ifel*)btn);

   SDL_FreeSurface(btn->up);
   SDL_FreeSurface(btn->down);
   SDL_FreeSurface(btn->hover);
   free(btn);
}

int InitUI(void)
{
   int returnCode = 0;
   const char* fontName = "/usr/share/fonts/truetype/freefont/FreeSans.ttf";

   fprintf(stderr, "InitUI\n");

   //Start SDL 
   SDL_Init(SDL_INIT_EVERYTHING);

   // Set up screen
   screen = SDL_SetVideoMode(1024, 800, 32, SDL_SWSURFACE | SDL_SRCCOLORKEY);
   if (!screen)
   {
      fprintf(stderr, "Error setting video mode\n");
      returnCode = 1;
   }

   // TTF is 'true type fonts'
   if (TTF_Init() != 0)
   {
      fprintf(stderr, "Error initializing TTF\n");
      returnCode = 2;
   }

   font = TTF_OpenFont(fontName, 24);
   if (!font)
   {
      fprintf(stderr, "Can't load font %s\n", fontName);
      returnCode = 3;
   }

   ifels = ListCreate();
   if (!ifels)
   {
      fprintf(stderr, "Can't create list\n");
      returnCode = 4;
   }
      
   return returnCode;
}

int DestroyUI(void)
{
   iterator i;
   Ifel* el;

   //fprintf(stderr, "DestroyUI\n");

   // see if there are any ifels that weren't properly released
   el = GetFirstIfel(NULL, &i);
   while (el)
   {
      fprintf(stderr, "ifel %i (type %i) was not released properly\n", el->id, el->type);
      el = GetNextIfel(&i);
   }
   
   // stop TTF
   TTF_Quit();

   //Quit SDL
   SDL_Quit();

}


// the main event loop (dispatcher) function
void Run(void)
{
   SDL_Event event;
   SDL_Rect dest;
   iterator i;
   Ifel* el;

   fprintf(stderr, "Run\n");

   while (1)
   {
      // first handle any events
      while (SDL_PollEvent(&event))
      {
         switch (event.type)
         {
         case SDL_KEYDOWN:
            fprintf(stderr, "key down\n");
            OnKeyPressed(event.key.keysym.sym);
            break;

         case SDL_MOUSEMOTION:
            //fprintf(stderr, "mouse move\n");
            el = GetFirstIfel(NULL, &i);
            while (el)
            {
               // mark the new element
               if (event.button.x >= el->loc.x &&
                   event.button.x <= el->loc.x + el->loc.w &&
                   event.button.y >= el->loc.y &&
                   event.button.y <= el->loc.y + el->loc.y)
               {
                  if (el->type == IFEL_BUTTON)
                  {
                     // if the button doesn't already have a button down state, mark it
                     if (((Button*)el)->state != BUTTON_PRESSED)
                        ((Button*)el)->state = BUTTON_HOVER;
                  }
               }
               else
               {
                  // unmark all other elements
                  if (el->type == IFEL_BUTTON)
                     ((Button*)el)->state = BUTTON_NORMAL;
               }
               el = GetNextIfel(&i);
            }
            break;

         case SDL_MOUSEBUTTONDOWN:
            //fprintf(stderr, "mouse down\n");
            el = GetFirstIfel(NULL, &i);
            while (el)
            {
               if (event.button.x >= el->loc.x &&
                   event.button.x <= el->loc.x + el->loc.w &&
                   event.button.y >= el->loc.y &&
                   event.button.y <= el->loc.y + el->loc.y)
               {
                  currentEl = el; // remember where the button went down
                  if (el->type == IFEL_BUTTON)
                  {
                     ((Button*)el)->state = BUTTON_PRESSED;
                  }
                  break;
               }
               el = GetNextIfel(&i);
            }
            break;

         case SDL_MOUSEBUTTONUP:
            //fprintf(stderr, "mouse up\n");
            el = GetFirstIfel(NULL, &i);
            while (el)
            {
               if (event.button.x >= el->loc.x &&
                   event.button.x <= el->loc.x + el->loc.w &&
                   event.button.y >= el->loc.y &&
                   event.button.y <= el->loc.y + el->loc.y)
               {
                  if (el->type == IFEL_BUTTON)
                  {
                     ((Button*)el)->state = BUTTON_HOVER;
                  }
                  // if the button came up on the same object on which it went down
                  if (currentEl == el)
                  {
                     if(el->OnMouseClick)
                        el->OnMouseClick(el);
                  }
                  currentEl = NULL;
                  break;
               }
               el = GetNextIfel(&i);
            }
            break;

         case SDL_QUIT:
            return;
         }
      }

      // now redraw the screen by running through all Ifels in DFS manner
      Ifel* el = GetFirstIfel(NULL, &i);
      while(el)
      {
         if (el->active)
            el->draw(el);
         el = GetNextIfel(&i);
      }

#if DEBUG
      // show property alignment
      for (int i=0; i < NUM_PROPERTIES; i++)
         rectangleColor(screen, board[i].loc.x, board[i].loc.y,
            board[i].loc.x + board[i].loc.w, board[i].loc.y + board[i].loc.h, 0xFF00FF);
#endif

      SDL_Flip(screen);
   }
}


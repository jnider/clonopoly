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

int DeleteIfel(Ifel* parent, Ifel* child)
{
   list* l;
   iterator iter;

   // first delete all children
   Ifel* el = GetFirstIfel(NULL, &iter);
   while(el)
   {
      //el->delete(el); - JKN must add this
      el = GetNextIfel(&iter);
   }

   // now delete self
   if (!parent)
      l = ifels;
   else
      l = parent->ifels;

   ListRemoveNode(l, child);
   SDL_FreeSurface(child->surface);
   child->surface = NULL;
   
   return 0;
}

// set the iterator and return the first ifel's data
Ifel* GetFirstIfel(Ifel* parent, iterator* iter)
{
   //fprintf(stderr, "GetFirstIfel\n");
   if (!parent)
   {
      if (ifels->size == 0)
         return NULL;
      *iter = ifels->first;
   }
   else
   {
      if (parent->ifels->size == 0)
         return NULL;
      *iter = parent->ifels->first;
   }
   return (Ifel*)ListGetData(*iter);
}

Ifel* GetNextIfel(iterator* item)
{
   //fprintf(stderr, "GetNextIfel\n");
   node* n = *item;
   *item = n->next;
   if (n->next)
      return (Ifel*)ListGetData(n->next);
   else
      return NULL;
}

Ifel* CreateIfel(int id, Ifel* parent, Ifel* me, IfelType type, IfelDrawFn draw)
{
   //fprintf(stderr, "CreateIfel\n");
   me->id = id;
   me->type = type;
   me->draw = draw;
   me->ifels = ListCreate();
   
   // create the drawing surface
   me->surface = SDL_CreateRGBSurface(SDL_SWSURFACE, me->loc.w, me->loc.h, 32, 0, 0, 0, 0);
   if (!me->surface)
   {
      fprintf(stderr, "Error creating window surface\n");
      return NULL;
   }
   SDL_SetColorKey(me->surface, SDL_SRCCOLORKEY, 0);

   // add it to the list of windows to draw
   if (!parent)
      ListAddNode(ifels, me);
   else
      ListAddNode(parent->ifels, me);

   return me;
}

void DrawIfelChildren(Ifel* i)
{
   iterator iter;
   Ifel* el = GetFirstIfel(i, &iter);
   while(el)
   {
      if (el->active && el->draw)
      {
            el->draw(el);
            DrawIfelChildren(el);
            SDL_BlitSurface(el->surface, NULL, i->surface, &el->loc);
      }
      el = GetNextIfel(&iter);
   }
}

void DrawMessageBox(Ifel* i)
{
   //MessageBox* mb = (MessageBox*)i;
   fprintf(stderr, "DrawMessageBox\n");

   // render the text to the surface
}

MessageBox* CreateMessageBox(int id, const char* msg)
{
   SDL_Surface* text;
   if (!font)
   {
      return NULL;
   }

   MessageBox* mb = malloc(sizeof(MessageBox));
   fprintf(stderr, "Creating messagebox\n");
   if (!mb)
   {
      fprintf(stderr, "Error creating messagebox\n");
      return NULL;
   }
   memset(mb, 0, sizeof(MessageBox));
   

   // render text to determine its size
   SDL_Color textColor;
   textColor.r = 0xFF;
   textColor.g = 0;
   textColor.b = 0;
   text = TTF_RenderText_Solid(font, msg, textColor);

   // calculate windows size based on text & icons
   mb->el.loc.x = 0;
   mb->el.loc.y = 0;

   mb->el.loc.w = text->w + (MESSAGE_BOX_MARGIN * 2);
   mb->el.loc.h = text->h + (MESSAGE_BOX_MARGIN * 2);
   
   // draw the message and the box
   roundedBoxRGBA(mb->el.surface, 0, 0, mb->el.loc.w, mb->el.loc.h, 10, 0xF0, 0, 0, 0x30);
   if (text)
   {
      SDL_Rect loc;
      loc.x = (mb->el.loc.w - text->w) / 2;
      loc.y = (mb->el.loc.h - text->h) / 2;
      loc.w = text->w;
      loc.h = text->h;
      SDL_BlitSurface(text, NULL, mb->el.surface, &loc);
      SDL_FreeSurface(text);
   }
      //stringColor(mb->surface, 0, (wnd->size.h+10)/2, msg, 0xFFF0F0FF);

   // set the location of the window on the screen
   mb->el.loc.x = (screen->w - mb->el.loc.w) / 2;
   mb->el.loc.y = (screen->h - mb->el.loc.h) / 2;

   // add it to the list of windows to draw
   CreateIfel(id, NULL, (Ifel*)mb, IFEL_MESSAGEBOX, DrawMessageBox);
   mb->el.active = 1;
   
   return mb;
}

void DeleteMessageBox(MessageBox* mb)
{
   DeleteIfel(NULL, (Ifel*)mb);
   free(mb);
}

void DrawImage(Ifel* i)
{
   Image* img = (Image*)i;
   SDL_BlitSurface(img->surface, NULL, i->surface, NULL);
}

Image* CreateImage(int id, Ifel* parent, const char* bitmap)
{
   //fprintf(stderr, "Creating image id %i\n", id);

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

   img->el.loc.x = 0;
   img->el.loc.y = 0;
   img->el.loc.w = img->surface->w;
   img->el.loc.h = img->surface->h;

   // add it to the list of windows to draw
   CreateIfel(id, parent, &img->el, IFEL_IMAGE, DrawImage);
   img->el.active = 1;

   return img;
}

void DeleteImage(Image* img)
{
   fprintf(stderr, "Unloading image\n");
   if (!img)
      return;

   DeleteIfel(NULL, (Ifel*)img);
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
      SDL_BlitSurface(btn->up, NULL, i->surface, NULL);
      break;
   case BUTTON_PRESSED:
      SDL_BlitSurface(btn->down, NULL, i->surface, NULL);
      break;
   case BUTTON_HOVER:
      SDL_BlitSurface(btn->hover, NULL, i->surface, NULL);
      break;
   }
}

Button* CreateButton(int id, Ifel* parent, int x, int y, const char* img_up, const char* img_down, const char* img_hover)
{
   //fprintf(stderr, "CreateButton\n");
   Button* button = malloc(sizeof(Button));
   if (!button)
   {
      fprintf(stderr, "Error allocating button\n");
      return NULL;
   }

   memset(button, 0, sizeof(Button));

   button->up = IMG_Load(img_up);
   if (!button->up)
   {
      fprintf(stderr, "Can't load image %s\n", img_up);
      goto img_up_fail;
   }
   button->down = IMG_Load(img_down);
   if (!button->down)
   {
      fprintf(stderr, "Can't load image %s\n", img_down);
      goto img_down_fail;
   }
   button->hover = IMG_Load(img_hover);
   if (!button->hover)
   {
      fprintf(stderr, "Can't load image %s\n", img_hover);
      goto img_hover_fail;
   }
   // set the internal variables
   button->state = BUTTON_NORMAL;
   button->el.loc.x = x;
   button->el.loc.y = y;
   button->el.loc.w = button->up->w;
   button->el.loc.h = button->up->h;

   // add it to the list of windows to draw
   CreateIfel(id, parent, &button->el, IFEL_BUTTON, DrawButton);
   button->el.active = 1;

   return button;

   SDL_FreeSurface(button->hover);
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
   if (!btn)
      return;

   DeleteIfel(NULL, (Ifel*)btn);

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

   return 0;
}


// the main event loop (dispatcher) function
void Run(void)
{
   SDL_Event event;
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
      //fprintf(stderr, "drawing ifels\n");
      Ifel* el = GetFirstIfel(NULL, &i);
      while(el)
      {
         if (el->active && el->draw)
         {
            el->draw(el);
            DrawIfelChildren(el);
            //fprintf(stderr, "blting from ifel to screen\n");
            SDL_BlitSurface(el->surface, NULL, screen, &el->loc);
         }
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


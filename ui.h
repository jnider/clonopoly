#ifndef _UI__H
#define _UI__H

#include "list.h"
#include "SDL/SDL.h"


typedef enum IfelType
{
   IFEL_GENERIC,
   IFEL_IMAGE,
   IFEL_BUTTON,
   IFEL_MESSAGEBOX
} IfelType;

typedef enum ButtonState
{
   BUTTON_NORMAL,
   BUTTON_PRESSED,
   BUTTON_HOVER
} ButtonState;

struct Ifel;
typedef void(*IfelDrawFn)(struct Ifel* el);
typedef void(*IfelOnMouseClickFn)(struct Ifel* el);

typedef struct Ifel
{
   IfelType id;            // what kind of ifel is this
   int active;             // is this interface element visible & active
   //SDL_Surface* surface;   // what to draw
   SDL_Rect loc;           // relative boundaries on the owner ifel
   //SDL_Rect size;          // absolute size of the element (x=0,y=0,h>0,y>0)
   list ifels;             // each element may contain other elements

   // internal callbacks
   IfelDrawFn draw;

   // some event callbacks
   IfelOnMouseClickFn OnMouseClick;
} Ifel;


typedef struct Image
{
   Ifel el;
   SDL_Surface* surface;
} Image;

typedef struct Button
{
   Ifel el;
   SDL_Surface* up;
   SDL_Surface* down;      // extend the normal interface by adding two more bitmaps (one for each state)
   SDL_Surface* hover;
   ButtonState state;      // remember the state of the button
   //SDL_Rect loc;           // boundaries of the button on the board
} Button;

typedef struct MessageBox
{
   Ifel el;
   SDL_Surface* surface;
   int flags;
} MessageBox;

void Run(void);
int InitUI(void);
int DestroyUI(void);

Image* CreateImage(const char* bitmap);
void DeleteImage(Image* img);

Button* CreateButton(int x, int y, const char* img_up, const char* img_down, const char* img_hover);
void DeleteButton(Button* btn);

MessageBox* CreateMessageBox(const char* msg);
void DeleteMessageBox(MessageBox* mb);

#endif // _UI__H


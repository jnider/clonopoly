#ifndef _UI__H
#define _UI__H

#include "list.h"
#include "SDL/SDL.h"

#define MB_NO     0
#define MB_YES    1
#define MB_UNDEFINED 0xFF
#define ID_MB_YESNO  (-10)

typedef enum IfelType
{
   IFEL_CUSTOM,
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
typedef void(*IfelOnKeyPressedFn)(struct Ifel* el, int key);

typedef struct Ifel
{
   int id;
   IfelType type;          // what kind of ifel is this
   SDL_Surface* surface;   // every ifel must have a default surface
   int active;             // is this interface element visible & active
   SDL_Rect loc;           // relative boundaries on the owner ifel
   void* data;             // can keep a pointer to user-defined data here
   list* ifels;            // each element may contain other elements

   // internal callbacks
   IfelDrawFn draw;

   // some event callbacks
   IfelOnMouseClickFn OnMouseClick;
   IfelOnKeyPressedFn OnKeyPressed;
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
} Button;

typedef struct MessageBox
{
   Ifel el;
   int flags;
} MessageBox;

typedef enum MessageBoxStyle
{
   MSGBOX_PLAIN,
   MSGBOX_YESNO
} MessageBoxStyle;

void Run(void);
int InitUI(void);
int DestroyUI(void);

void SetFocus(Ifel* el);

Ifel* CreateIfel(int id, Ifel* parent, Ifel* me, IfelType type, IfelDrawFn draw);
int DeleteIfel(Ifel* parent, Ifel* child);
Ifel* GetFirstIfel(Ifel* parent, iterator* iter);
Ifel* GetNextIfel(iterator* item);
void DrawIfelChildren(Ifel* i);

Image* CreateImage(int id, Ifel* parent, const char* bitmap);
void DeleteImage(Image* img);

Button* CreateButton(int id, Ifel* parent, int x, int y, const char* img_up, const char* img_down, const char* img_hover);
void DeleteButton(Button* btn);

MessageBox* CreateMessageBox(int id, const char* msg);
void DeleteMessageBox(MessageBox* mb);
int ModalMessageBox(int id, const char* msg);

#endif // _UI__H


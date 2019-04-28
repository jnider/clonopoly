#ifndef _UI__H
#define _UI__H

#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "list.h"

#define MB_NO     0
#define MB_YES    1
#define MB_UNDEFINED 0xFF
#define ID_MB_YESNO  (-10)

extern TTF_Font* font;

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
   BUTTON_HOVER,
   BUTTON_DISABLED
} ButtonState;

struct Ifel;
typedef void(*IfelDrawFn)(struct Ifel* el);
typedef void(*IfelOnMouseClickFn)(struct Ifel* el);
typedef void(*IfelOnKeyPressedFn)(struct Ifel* el, int key);

typedef struct Ifel
{
   int id;
   IfelType type;          // what kind of ifel is this
//   SDL_Surface* surface;   // every ifel must have a default surface
   SDL_Renderer* renderer; // every ifel points to the screen renderer
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
   //SDL_Surface* surface;
   SDL_Texture* texture;
   SDL_Rect loc;
} Image;

typedef struct Button
{
   Ifel el;
   SDL_Texture* up;
   SDL_Texture* down;      // extend the normal interface by adding two more bitmaps (one for each state)
   SDL_Texture* hover;
   ButtonState state;      // remember the state of the button
   char* text;
   int w;
   int h;
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

Button* CreatePngButton(int id, Ifel* parent, int x, int y, const char* img_up, const char* img_down, const char* img_hover);
Button* CreateTextButton(int id, Ifel* parent, SDL_Rect* size, const char* msg);
void EnableButton(Button* btn);
void DisableButton(Button* btn);
void DeleteButton(Button* btn);

MessageBox* CreateMessageBox(int id, const char* msg);
void DeleteMessageBox(MessageBox* mb);
int ModalMessageBox(int id, const char* msg);

void Quit(void);

#endif // _UI__H


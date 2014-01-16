#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_gfxPrimitives.h"
#include "SDL/SDL_ttf.h"

#define MAX_PLAYERS 4
#define MAX_HOUSES 5 // how many houses can be on each property (house #5 = hotel)
#define NUM_PROPERTIES (10 * 4)

#define PROPERTY_HEIGHT 92
#define PROPERTY_WIDTH 56
#define PROPERTY_TOP_ROW 6
#define PROPERTY_BOTTOM_ROW 604
#define PROPERTY_LEFT_COLUMN 7
#define MESSAGE_BOX_MARGIN 10

// indices of all pieces
enum
{
   PIECE_HORSE,
   PIECE_SHOE,
   PIECE_COUNT
};

typedef enum ButtonId
{
   BTN_OPTIONS,
   BTN_COUNT
} ButtonId;

typedef enum ButtonState
{
   BUTTON_NORMAL,
   BUTTON_PRESSED,
   BUTTON_HOVER
} ButtonState;

typedef struct Player
{
   int piece;              // which piece am I using?
   int active;             // still in the game?
   int location;           // which square am I currently on?
   int money;              // how much cash do I have
} Player;

typedef struct Property
{
   char* name;             // printable property name
   int value;              // original cost
   int rent[MAX_HOUSES];   // how much visitors have to pay
   SDL_Rect loc;           // boundaries of the square on the board
} Property;

typedef struct Button
{
   SDL_Surface* up;
   SDL_Surface* down;
   SDL_Surface* over;
   ButtonState state;
   SDL_Rect loc;           // boundaries of the button on the board
} Button;

typedef struct Window
{
   int active;             // is this window visible & active
   SDL_Surface* surface;   // what to draw
   SDL_Rect loc;           // boundaries of the window on the screen
   SDL_Rect size;          // absolute size of the window (x=0,y=0,h>0,y>0)
} Window;

static const char* piece_name[PIECE_COUNT] =
{
   "piece_horse",
   "piece_shoe"
};

// SDL stuff
static SDL_Surface* piece[PIECE_COUNT];
static SDL_Surface* board_img;
static SDL_Surface* screen;
static TTF_Font* font;

// game stuff
static Window* messageBox;
static Button button[BTN_COUNT];
static Button* currentButton;
static Player player[MAX_PLAYERS]; // the actual players
static Property board[NUM_PROPERTIES] =
{
   { "Go",                    0, 0, 0, 0, 0, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*9, PROPERTY_BOTTOM_ROW, PROPERTY_HEIGHT, PROPERTY_HEIGHT}},
   { "Community Chest",       0, 0, 0, 0, 0, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*8, PROPERTY_BOTTOM_ROW, PROPERTY_WIDTH, PROPERTY_HEIGHT}},
   { "Mediterranean",         0, 0, 0, 0, 0, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*7, PROPERTY_BOTTOM_ROW, PROPERTY_WIDTH, PROPERTY_HEIGHT}},
   { "Baltic",                0, 0, 0, 0, 0, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*6, PROPERTY_BOTTOM_ROW, PROPERTY_WIDTH, PROPERTY_HEIGHT}},
   { "Income Tax",            0, 0, 0, 0, 0, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*5, PROPERTY_BOTTOM_ROW, PROPERTY_WIDTH, PROPERTY_HEIGHT}},
   { "Reading Railroad",      0, 0, 0, 0, 0, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*4, PROPERTY_BOTTOM_ROW, PROPERTY_WIDTH, PROPERTY_HEIGHT}},
   { "Oriental Ave",          0, 0, 0, 0, 0, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*3, PROPERTY_BOTTOM_ROW, PROPERTY_WIDTH, PROPERTY_HEIGHT}},
   { "Chance",                0, 0, 0, 0, 0, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*2, PROPERTY_BOTTOM_ROW, PROPERTY_WIDTH, PROPERTY_HEIGHT}},
   { "Vermont Ave",           0, 0, 0, 0, 0, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*1, PROPERTY_BOTTOM_ROW, PROPERTY_WIDTH, PROPERTY_HEIGHT}},
   { "Connecticut Ave",       0, 0, 0, 0, 0, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT, PROPERTY_BOTTOM_ROW, PROPERTY_WIDTH, PROPERTY_HEIGHT}},

   { "Jail",                  0, 0, 0, 0, 0, 0, {PROPERTY_LEFT_COLUMN, PROPERTY_BOTTOM_ROW, PROPERTY_HEIGHT, PROPERTY_HEIGHT}},
   { "St. Charles",           0, 0, 0, 0, 0, 0, {PROPERTY_LEFT_COLUMN, PROPERTY_TOP_ROW+PROPERTY_HEIGHT+PROPERTY_WIDTH*8, PROPERTY_HEIGHT, PROPERTY_WIDTH}},
   { "Electric Company",      0, 0, 0, 0, 0, 0, {PROPERTY_LEFT_COLUMN, PROPERTY_TOP_ROW+PROPERTY_HEIGHT+PROPERTY_WIDTH*7, PROPERTY_HEIGHT, PROPERTY_WIDTH}},
   { "States Ave",            0, 0, 0, 0, 0, 0, {PROPERTY_LEFT_COLUMN, PROPERTY_TOP_ROW+PROPERTY_HEIGHT+PROPERTY_WIDTH*6, PROPERTY_HEIGHT, PROPERTY_WIDTH}},
   { "Virginia Ave",          0, 0, 0, 0, 0, 0, {PROPERTY_LEFT_COLUMN, PROPERTY_TOP_ROW+PROPERTY_HEIGHT+PROPERTY_WIDTH*5, PROPERTY_HEIGHT, PROPERTY_WIDTH}},
   { "Pennsylvania Railroad", 0, 0, 0, 0, 0, 0, {PROPERTY_LEFT_COLUMN, PROPERTY_TOP_ROW+PROPERTY_HEIGHT+PROPERTY_WIDTH*4, PROPERTY_HEIGHT, PROPERTY_WIDTH}},
   { "St. James Place",       0, 0, 0, 0, 0, 0, {PROPERTY_LEFT_COLUMN, PROPERTY_TOP_ROW+PROPERTY_HEIGHT+PROPERTY_WIDTH*3, PROPERTY_HEIGHT, PROPERTY_WIDTH}},
   { "Community Chest",       0, 0, 0, 0, 0, 0, {PROPERTY_LEFT_COLUMN, PROPERTY_TOP_ROW+PROPERTY_HEIGHT+PROPERTY_WIDTH*2, PROPERTY_HEIGHT, PROPERTY_WIDTH}},
   { "Tennessee Ave",         0, 0, 0, 0, 0, 0, {PROPERTY_LEFT_COLUMN, PROPERTY_TOP_ROW+PROPERTY_HEIGHT+PROPERTY_WIDTH*1, PROPERTY_HEIGHT, PROPERTY_WIDTH}},
   { "New York Ave",          0, 0, 0, 0, 0, 0, {PROPERTY_LEFT_COLUMN, PROPERTY_TOP_ROW+PROPERTY_HEIGHT, PROPERTY_HEIGHT, PROPERTY_WIDTH}},

   { "Free Parking",          0, 0, 0, 0, 0, 0, {PROPERTY_LEFT_COLUMN, PROPERTY_TOP_ROW, PROPERTY_HEIGHT, PROPERTY_HEIGHT}},
   { "Kentucky Ave",          0, 0, 0, 0, 0, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT, PROPERTY_TOP_ROW, PROPERTY_WIDTH, PROPERTY_HEIGHT}},
   { "Chance",                0, 0, 0, 0, 0, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*1, PROPERTY_TOP_ROW, PROPERTY_WIDTH, PROPERTY_HEIGHT}},
   { "Indiana",               0, 0, 0, 0, 0, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*2, PROPERTY_TOP_ROW, PROPERTY_WIDTH, PROPERTY_HEIGHT}},
   { "Illinois",              0, 0, 0, 0, 0, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*3, PROPERTY_TOP_ROW, PROPERTY_WIDTH, PROPERTY_HEIGHT}},
   { "B & O Railroad",        0, 0, 0, 0, 0, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*4, PROPERTY_TOP_ROW, PROPERTY_WIDTH, PROPERTY_HEIGHT}},
   { "Atlantic Ave",          0, 0, 0, 0, 0, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*5, PROPERTY_TOP_ROW, PROPERTY_WIDTH, PROPERTY_HEIGHT}},
   { "Ventnor Ave",           0, 0, 0, 0, 0, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*6, PROPERTY_TOP_ROW, PROPERTY_WIDTH, PROPERTY_HEIGHT}},
   { "Water Works",           0, 0, 0, 0, 0, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*7, PROPERTY_TOP_ROW, PROPERTY_WIDTH, PROPERTY_HEIGHT}},
   { "Marvin Gardens",        0, 0, 0, 0, 0, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*8, PROPERTY_TOP_ROW, PROPERTY_WIDTH, PROPERTY_HEIGHT}},

   { "Go To Jail",            0, 0, 0, 0, 0, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*9, PROPERTY_TOP_ROW, PROPERTY_HEIGHT, PROPERTY_HEIGHT}},
   { "Pacific Ave",           0, 0, 0, 0, 0, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*9, PROPERTY_TOP_ROW+PROPERTY_HEIGHT, PROPERTY_HEIGHT, PROPERTY_WIDTH}},
   { "North Carolina Ave",    0, 0, 0, 0, 0, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*9, PROPERTY_TOP_ROW+PROPERTY_HEIGHT+PROPERTY_WIDTH*1, PROPERTY_HEIGHT, PROPERTY_WIDTH}},
   { "Community Chest",       0, 0, 0, 0, 0, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*9, PROPERTY_TOP_ROW+PROPERTY_HEIGHT+PROPERTY_WIDTH*2, PROPERTY_HEIGHT, PROPERTY_WIDTH}},
   { "Pennsylvania Ave",      0, 0, 0, 0, 0, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*9, PROPERTY_TOP_ROW+PROPERTY_HEIGHT+PROPERTY_WIDTH*3, PROPERTY_HEIGHT, PROPERTY_WIDTH}},
   { "Short Line",            0, 0, 0, 0, 0, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*9, PROPERTY_TOP_ROW+PROPERTY_HEIGHT+PROPERTY_WIDTH*4, PROPERTY_HEIGHT, PROPERTY_WIDTH}},
   { "Chance",                0, 0, 0, 0, 0, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*9, PROPERTY_TOP_ROW+PROPERTY_HEIGHT+PROPERTY_WIDTH*5, PROPERTY_HEIGHT, PROPERTY_WIDTH}},
   { "Park Place",            0, 0, 0, 0, 0, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*9, PROPERTY_TOP_ROW+PROPERTY_HEIGHT+PROPERTY_WIDTH*6, PROPERTY_HEIGHT, PROPERTY_WIDTH}},
   { "Luxury Tax",            0, 0, 0, 0, 0, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*9, PROPERTY_TOP_ROW+PROPERTY_HEIGHT+PROPERTY_WIDTH*7, PROPERTY_HEIGHT, PROPERTY_WIDTH}},
   { "Boardwalk",             0, 0, 0, 0, 0, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*9, PROPERTY_TOP_ROW+PROPERTY_HEIGHT+PROPERTY_WIDTH*8, PROPERTY_HEIGHT, PROPERTY_WIDTH}}
};

Window* CreateMessageBox(const char* msg)
{
   SDL_Surface* text;
   Window* wnd = malloc(sizeof(Window));
   fprintf(stderr, "Creating messagebox\n");
   if (!wnd)
   {
      fprintf(stderr, "Error creating window\n");
      return NULL;
   }
   
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
   wnd->size.x = 0;
   wnd->size.y = 0;

   if (text)
   {
      wnd->size.w = text->w + (MESSAGE_BOX_MARGIN * 2);
      wnd->size.h = text->h + (MESSAGE_BOX_MARGIN * 2);
   }
   
   // create the drawing surface
   wnd->surface = SDL_CreateRGBSurface(SDL_SWSURFACE, wnd->size.w, wnd->size.h, 32, 0, 0, 0, 0);
   if (!wnd->surface)
   {
      fprintf(stderr, "Error creating window surface\n");
      free(wnd);
      return NULL;
   }
   SDL_SetColorKey(wnd->surface, SDL_SRCCOLORKEY, 0);

   // draw the message and the box
   roundedBoxRGBA(wnd->surface, 0, 0, wnd->size.w, wnd->size.h, 10, 0xF0, 0, 0, 0x30);
   if (text)
   {
      SDL_Rect loc;
      loc.x = (wnd->size.w - text->w) / 2;
      loc.y = (wnd->size.h - text->h) / 2;
      loc.w = text->w;
      loc.h = text->h;
      SDL_BlitSurface(text, NULL, wnd->surface, &loc);
   }
   else
      stringColor(wnd->surface, 0, (wnd->size.h+10)/2, msg, 0xFFF0F0FF);

   // set the location of the window on the screen
   wnd->loc.x = (screen->w - wnd->size.w) / 2;
   wnd->loc.y = (screen->h - wnd->size.h) / 2;
   
   // finally, set it active
   wnd->active = 1;
   
   return wnd;
}

void DrawMessageBox(Window* wnd)
{
   SDL_BlitSurface(wnd->surface, NULL, screen, &wnd->loc);
}

void DestroyMessageBox(Window* wnd)
{
   SDL_FreeSurface(wnd->surface);
   free(wnd);
}

static void OnKeyPressed(int key)
{
   switch(key)
   {
   case 'n':
      if (!messageBox)
         messageBox = CreateMessageBox("key press");
      break;
   }
}

static void OnMouseDown(int x, int y)
{
   for (int i=0; i < BTN_COUNT; i++)
   {
      if (x >= button[i].loc.x &&
          x <= button[i].loc.x + button[i].loc.w &&
          y >= button[i].loc.y &&
          y <= button[i].loc.y + button[i].loc.y)
         {
            button[i].state = BUTTON_PRESSED;
            currentButton = &button[i]; // save this for 'unpressing' later
            break;
         }
   }
   if (messageBox && messageBox->active)
   {
      DestroyMessageBox(messageBox);
      messageBox = NULL;
   }
   else
   {
      player[0].location++;
      player[0].location %= NUM_PROPERTIES;
   }
}

static void OnMouseUp(int x, int y)
{
   if (currentButton)
   {
      currentButton->state = BUTTON_NORMAL;
      currentButton = NULL;
   }
}

// the main event loop (dispatcher) function
static void Run(void)
{
   SDL_Event event;
   SDL_Rect dest;

   while (1)
   {
      // first handle any events
      while (SDL_PollEvent(&event))
      {
         switch (event.type)
         {
         case SDL_KEYDOWN:
            OnKeyPressed(event.key.keysym.sym);
            break;

         case SDL_MOUSEMOTION:
            break;

         case SDL_MOUSEBUTTONDOWN:
            OnMouseDown(event.button.x, event.button.y);
            break;

         case SDL_MOUSEBUTTONUP:
            OnMouseUp(event.button.x, event.button.y);
            break;

         case SDL_QUIT:
            return;
         }
      }

      // now redraw the screen

      // the board_img
      SDL_BlitSurface(board_img, NULL, screen, NULL);

      // draw the buttons
      for (int i=0; i < BTN_COUNT; i++)
      {
         switch (button[i].state)
         {
         case BUTTON_PRESSED:
            SDL_BlitSurface(button[i].down, NULL, screen, &button[i].loc);
            break;
         case BUTTON_NORMAL:
            SDL_BlitSurface(button[i].up, NULL, screen, &button[i].loc);
            break;
         case BUTTON_HOVER:
            SDL_BlitSurface(button[i].over, NULL, screen, &button[i].loc);
            break;
         }
      }

      // draw the player icons
      for (int i=0; i < MAX_PLAYERS; i++)
      {
         if (player[i].active)
         {
            SDL_Rect pos;
            pos.w = board[player[i].location].loc.w;
            pos.h = board[player[i].location].loc.h;
            pos.x = board[player[i].location].loc.x + ((pos.w - piece[player[i].piece]->w) >> 1); // without the brackets on the >> this doesn't calculate right
            pos.y = board[player[i].location].loc.y + pos.h - piece[player[i].piece]->h;
            SDL_BlitSurface(piece[player[i].piece], NULL, screen, &pos);
         }
      }

      if (messageBox && messageBox->active)
         DrawMessageBox(messageBox);

#if DEBUG
      // show property alignment
      for (int i=0; i < NUM_PROPERTIES; i++)
         rectangleColor(screen, board[i].loc.x, board[i].loc.y,
            board[i].loc.x + board[i].loc.w, board[i].loc.y + board[i].loc.h, 0xFF00FF);
#endif

      SDL_Flip(screen);
   }
}

int main(int argc, char* args[])
{
   int returnCode = 0;

   //Start SDL 
   SDL_Init(SDL_INIT_EVERYTHING);

   // Set up screen
   screen = SDL_SetVideoMode(1024, 800, 32, SDL_SWSURFACE | SDL_SRCCOLORKEY);

   // TTF is 'true type fonts'
   if (TTF_Init() != 0)
   {
      fprintf(stderr, "Error initializing TTF\n");
      returnCode = 1;
      goto TTFFail;
   }
   const char* fontName = "/usr/share/fonts/truetype/freefont/FreeSans.ttf";
   font = TTF_OpenFont(fontName, 24);
   if (!font)
   {
      fprintf(stderr, "Can't load font %s\n", fontName);
      returnCode = 2;
      goto TTFFail;
   }
   
   // Load board_img image
   fprintf(stderr, "Loading board\n");
   board_img = IMG_Load("graphics/board.png");
   if (!board_img)
   {
      fprintf(stderr, "Can't load board image\n");
      return 1;
   }

   // load player piece images
   for (int i=0; i < PIECE_COUNT; i++)
   {
      char filename[64];
      fprintf(stderr, "Loading %s\n", piece_name[i]);
      sprintf(filename, "graphics/%s.png", piece_name[i]);
      piece[i] = IMG_Load(filename);
      if (!piece[i])
      {
         fprintf(stderr, "Can't load %s\n", piece_name[i]);

         // unload all successfully loaded pieces
         for (int j=i-1; j >=0; j--)
         {
            fprintf(stderr, "Unloading %s\n", piece_name[j]);
            SDL_FreeSurface(piece[j]);
         }
         return 1;
      }
   }
   
   // load buttons
   button[BTN_OPTIONS].up = IMG_Load("graphics/btn_options_1.png");
   if (!button[BTN_OPTIONS].up)
   {
      fprintf(stderr, "Can't load btn_options_1.png\n");
      return 2;
   }
   button[BTN_OPTIONS].down = IMG_Load("graphics/btn_options_2.png");
   if (!button[BTN_OPTIONS].down)
   {
      fprintf(stderr, "Can't load btn_options_2.png\n");
      return 2;
   }
   button[BTN_OPTIONS].loc.x = 0;
   button[BTN_OPTIONS].loc.y = 708;
   button[BTN_OPTIONS].loc.w = button[BTN_OPTIONS].up->w;
   button[BTN_OPTIONS].loc.h = button[BTN_OPTIONS].up->h;

   // set up the player info
   player[0].active = 1;

   Run();
   
   if (messageBox)
   {
      DestroyMessageBox(messageBox);
      messageBox = NULL; // not completely necessary, but a good habit
   }

   // free surfaces
   fprintf(stderr, "Unloading board_img\n");
   SDL_FreeSurface(board_img);
   for (int i=0; i < PIECE_COUNT; i++)
   {
      char filename[64];
      fprintf(stderr, "Unloading %s\n", piece_name[i]);
      SDL_FreeSurface(piece[i]);
   }

TTFFail:
   // stop TTF
   TTF_Quit();

   //Quit SDL
   SDL_Quit();
   return returnCode;
}


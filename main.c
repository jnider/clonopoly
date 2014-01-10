#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

#define MAX_PLAYERS 4

// indices of all pieces
enum
{
   PIECE_HORSE,
   PIECE_SHOE,
   PIECE_COUNT
};

typedef struct Player
{
   int piece;  // which piece am I using?
   int active; // still in the game?
   int location; // which square am I currently on?
   int money;  // how much cash do I have
} Player;

static const char* piece_name[PIECE_COUNT] =
{
   "piece_horse",
   "piece_shoe"
};

// global list of available player pieces
static SDL_Surface* piece[PIECE_COUNT];

static SDL_Surface* board;
static SDL_Surface* screen;

// the actual players
static Player player[MAX_PLAYERS];

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
            break;

         case SDL_MOUSEBUTTONDOWN:
            return;
            break;

         case SDL_QUIT:
            return;
         }
      }

      // now redraw the screen

      // the board
      SDL_BlitSurface(board, NULL, screen, NULL);

   // draw the player icon
   dest.x = 10;
   dest.y = 10;
   //SDL_BlitSurface(shoe, NULL, screen, &dest);

   SDL_Flip(screen);
   }
}

int main(int argc, char* args[])
{

   //Start SDL 
   SDL_Init(SDL_INIT_EVERYTHING);

   // Set up screen
   screen = SDL_SetVideoMode(1024, 800, 32, SDL_SWSURFACE | SDL_SRCCOLORKEY);

   // Load board image
   fprintf(stderr, "Loading board\n");
   board = IMG_Load("graphics/board.png");
   if (!board)
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

   Run();

   // free surfaces
   fprintf(stderr, "Unloading board\n");
   SDL_FreeSurface(board);
   for (int i=0; i < PIECE_COUNT; i++)
   {
      char filename[64];
      fprintf(stderr, "Unloading %s\n", piece_name[i]);
      SDL_FreeSurface(piece[i]);
   }

   //Quit SDL
   SDL_Quit();
   return 0;
}


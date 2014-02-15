#include <stdlib.h>
#include <time.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_gfxPrimitives.h"
#include "SDL/SDL_ttf.h"
#include "ui.h"
#include "options.h"
#include "id.h"

#define MAX_PLAYERS 4
#define MAX_HOUSES 5 // how many houses can be on each property (house #5 = hotel)
#define NUM_PROPERTIES (10 * 4)
#define STARTING_CASH 1500
#define ACCORDING_TO_DICE -1

#define PROPERTY_HEIGHT 92
#define PROPERTY_WIDTH 56
#define PROPERTY_TOP_ROW 6
#define PROPERTY_BOTTOM_ROW 604
#define PROPERTY_LEFT_COLUMN 7

#define SQUARE_GO                      0
#define SQUARE_INCOME_TAX              4
#define SQUARE_READING_RR              5
#define SQUARE_JUST_VISITING           10
#define SQUARE_ELECTRIC_CO             12
#define SQUARE_PENNSYLVANIA_RR         15
#define SQUARE_FREE_PARKING            20
#define SQUARE_BO_RR                   25
#define SQUARE_WATER_WORKS             28
#define SQUARE_GO_TO_JAIL              30
#define SQUARE_SHORT_LINE_RR           35
#define SQUARE_LUXURY_TAX              38
#define SQUARE_IN_JAIL                 40


typedef struct Player
{
   int token;              // which piece am I using?
   int active;             // still in the game?
   int location;           // which square am I currently on?
   int money;              // how much cash do I have
   int inJail;             // am I in jail?
   int turnsLeftInJail;    // how long until I have to pay
   int doublesCount;       // can only roll doubles 3 times before going to jail
} Player;

typedef struct Property
{
   char* name;             // printable property name
   int value;              // original cost
   int rent[MAX_HOUSES];   // how much visitors have to pay
   int numHouses;          // how many houses are on the property
   int owner;              // who owns it?
   int mortgaged;          // is it mortgaged?
   SDL_Rect loc;           // boundaries of the square on the board
} Property;

static const char* piece_name[] =
{
   "piece_horse",
   "piece_shoe",
   "piece_dog",
   "piece_bag"
};

static MessageBox* messageBox;
static Image* image[ID_IMG_COUNT];
static Button* button[ID_BTN_COUNT];
static int players;
static Player player[MAX_PLAYERS]; // the actual players
static int currentPlayer;
static int dice[2];
static Property board[NUM_PROPERTIES+1] =
{
   { "Go",                    0,   {0, 0, 0, 0, 0}, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*9, PROPERTY_BOTTOM_ROW, PROPERTY_HEIGHT, PROPERTY_HEIGHT}},
   { "Mediterranean",         40,  {0, 0, 0, 0, 0}, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*8, PROPERTY_BOTTOM_ROW, PROPERTY_WIDTH, PROPERTY_HEIGHT}},
   { "Community Chest",       0,   {0, 0, 0, 0, 0}, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*7, PROPERTY_BOTTOM_ROW, PROPERTY_WIDTH, PROPERTY_HEIGHT}},
   { "Baltic",                60,  {0, 0, 0, 0, 0}, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*6, PROPERTY_BOTTOM_ROW, PROPERTY_WIDTH, PROPERTY_HEIGHT}},
   { "Income Tax",            0,   {0, 0, 0, 0, 0}, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*5, PROPERTY_BOTTOM_ROW, PROPERTY_WIDTH, PROPERTY_HEIGHT}},
   { "Reading Railroad",      200, {0, 0, 0, 0, 0}, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*4, PROPERTY_BOTTOM_ROW, PROPERTY_WIDTH, PROPERTY_HEIGHT}},
   { "Oriental Ave",          100, {0, 0, 0, 0, 0}, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*3, PROPERTY_BOTTOM_ROW, PROPERTY_WIDTH, PROPERTY_HEIGHT}},
   { "Chance",                0,   {0, 0, 0, 0, 0}, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*2, PROPERTY_BOTTOM_ROW, PROPERTY_WIDTH, PROPERTY_HEIGHT}},
   { "Vermont Ave",           100, {0, 0, 0, 0, 0}, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*1, PROPERTY_BOTTOM_ROW, PROPERTY_WIDTH, PROPERTY_HEIGHT}},
   { "Connecticut Ave",       120, {0, 0, 0, 0, 0}, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT, PROPERTY_BOTTOM_ROW, PROPERTY_WIDTH, PROPERTY_HEIGHT}},

   { "Just Visiting",         0,   {0, 0, 0, 0, 0}, 0, -1, 0, {PROPERTY_LEFT_COLUMN, PROPERTY_BOTTOM_ROW, PROPERTY_HEIGHT, PROPERTY_HEIGHT}},
   { "St. Charles",           140, {0, 0, 0, 0, 0}, 0, -1, 0, {PROPERTY_LEFT_COLUMN, PROPERTY_TOP_ROW+PROPERTY_HEIGHT+PROPERTY_WIDTH*8, PROPERTY_HEIGHT, PROPERTY_WIDTH}},
   { "Electric Company",      150, {0, 0, 0, 0, 0}, 0, -1, 0, {PROPERTY_LEFT_COLUMN, PROPERTY_TOP_ROW+PROPERTY_HEIGHT+PROPERTY_WIDTH*7, PROPERTY_HEIGHT, PROPERTY_WIDTH}},
   { "States Ave",            140, {0, 0, 0, 0, 0}, 0, -1, 0, {PROPERTY_LEFT_COLUMN, PROPERTY_TOP_ROW+PROPERTY_HEIGHT+PROPERTY_WIDTH*6, PROPERTY_HEIGHT, PROPERTY_WIDTH}},
   { "Virginia Ave",          160, {0, 0, 0, 0, 0}, 0, -1, 0, {PROPERTY_LEFT_COLUMN, PROPERTY_TOP_ROW+PROPERTY_HEIGHT+PROPERTY_WIDTH*5, PROPERTY_HEIGHT, PROPERTY_WIDTH}},
   { "Pennsylvania Railroad", 200, {0, 0, 0, 0, 0}, 0, -1, 0, {PROPERTY_LEFT_COLUMN, PROPERTY_TOP_ROW+PROPERTY_HEIGHT+PROPERTY_WIDTH*4, PROPERTY_HEIGHT, PROPERTY_WIDTH}},
   { "St. James Place",       180, {0, 0, 0, 0, 0}, 0, -1, 0, {PROPERTY_LEFT_COLUMN, PROPERTY_TOP_ROW+PROPERTY_HEIGHT+PROPERTY_WIDTH*3, PROPERTY_HEIGHT, PROPERTY_WIDTH}},
   { "Community Chest",       0, {0, 0, 0, 0, 0}, 0, -1, 0, {PROPERTY_LEFT_COLUMN, PROPERTY_TOP_ROW+PROPERTY_HEIGHT+PROPERTY_WIDTH*2, PROPERTY_HEIGHT, PROPERTY_WIDTH}},
   { "Tennessee Ave",         180, {0, 0, 0, 0, 0}, 0, -1, 0, {PROPERTY_LEFT_COLUMN, PROPERTY_TOP_ROW+PROPERTY_HEIGHT+PROPERTY_WIDTH*1, PROPERTY_HEIGHT, PROPERTY_WIDTH}},
   { "New York Ave",          200, {0, 0, 0, 0, 0}, 0, -1, 0, {PROPERTY_LEFT_COLUMN, PROPERTY_TOP_ROW+PROPERTY_HEIGHT, PROPERTY_HEIGHT, PROPERTY_WIDTH}},

   { "Free Parking",          0, {0, 0, 0, 0, 0}, 0, -1, 0, {PROPERTY_LEFT_COLUMN, PROPERTY_TOP_ROW, PROPERTY_HEIGHT, PROPERTY_HEIGHT}},
   { "Kentucky Ave",          220, {0, 0, 0, 0, 0}, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT, PROPERTY_TOP_ROW, PROPERTY_WIDTH, PROPERTY_HEIGHT}},
   { "Chance",                0, {0, 0, 0, 0, 0}, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*1, PROPERTY_TOP_ROW, PROPERTY_WIDTH, PROPERTY_HEIGHT}},
   { "Indiana",               220, {0, 0, 0, 0, 0}, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*2, PROPERTY_TOP_ROW, PROPERTY_WIDTH, PROPERTY_HEIGHT}},
   { "Illinois",              240, {0, 0, 0, 0, 0}, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*3, PROPERTY_TOP_ROW, PROPERTY_WIDTH, PROPERTY_HEIGHT}},
   { "B & O Railroad",        200, {0, 0, 0, 0, 0}, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*4, PROPERTY_TOP_ROW, PROPERTY_WIDTH, PROPERTY_HEIGHT}},
   { "Atlantic Ave",          260, {0, 0, 0, 0, 0}, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*5, PROPERTY_TOP_ROW, PROPERTY_WIDTH, PROPERTY_HEIGHT}},
   { "Ventnor Ave",           260, {0, 0, 0, 0, 0}, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*6, PROPERTY_TOP_ROW, PROPERTY_WIDTH, PROPERTY_HEIGHT}},
   { "Water Works",           150, {0, 0, 0, 0, 0}, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*7, PROPERTY_TOP_ROW, PROPERTY_WIDTH, PROPERTY_HEIGHT}},
   { "Marvin Gardens",        280, {0, 0, 0, 0, 0}, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*8, PROPERTY_TOP_ROW, PROPERTY_WIDTH, PROPERTY_HEIGHT}},

   { "Go To Jail",            0, {0, 0, 0, 0, 0}, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*9, PROPERTY_TOP_ROW, PROPERTY_HEIGHT, PROPERTY_HEIGHT}},
   { "Pacific Ave",           300, {0, 0, 0, 0, 0}, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*9, PROPERTY_TOP_ROW+PROPERTY_HEIGHT, PROPERTY_HEIGHT, PROPERTY_WIDTH}},
   { "North Carolina Ave",    300, {0, 0, 0, 0, 0}, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*9, PROPERTY_TOP_ROW+PROPERTY_HEIGHT+PROPERTY_WIDTH*1, PROPERTY_HEIGHT, PROPERTY_WIDTH}},
   { "Community Chest",       0, {0, 0, 0, 0, 0}, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*9, PROPERTY_TOP_ROW+PROPERTY_HEIGHT+PROPERTY_WIDTH*2, PROPERTY_HEIGHT, PROPERTY_WIDTH}},
   { "Pennsylvania Ave",      320, {0, 0, 0, 0, 0}, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*9, PROPERTY_TOP_ROW+PROPERTY_HEIGHT+PROPERTY_WIDTH*3, PROPERTY_HEIGHT, PROPERTY_WIDTH}},
   { "Short Line",            200, {0, 0, 0, 0, 0}, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*9, PROPERTY_TOP_ROW+PROPERTY_HEIGHT+PROPERTY_WIDTH*4, PROPERTY_HEIGHT, PROPERTY_WIDTH}},
   { "Chance",                0, {0, 0, 0, 0, 0}, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*9, PROPERTY_TOP_ROW+PROPERTY_HEIGHT+PROPERTY_WIDTH*5, PROPERTY_HEIGHT, PROPERTY_WIDTH}},
   { "Park Place",            350, {0, 0, 0, 0, 0}, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*9, PROPERTY_TOP_ROW+PROPERTY_HEIGHT+PROPERTY_WIDTH*6, PROPERTY_HEIGHT, PROPERTY_WIDTH}},
   { "Luxury Tax",            0, {0, 0, 0, 0, 0}, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*9, PROPERTY_TOP_ROW+PROPERTY_HEIGHT+PROPERTY_WIDTH*7, PROPERTY_HEIGHT, PROPERTY_WIDTH}},
   { "Boardwalk",             400, {0, 0, 0, 0, 0}, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*9, PROPERTY_TOP_ROW+PROPERTY_HEIGHT+PROPERTY_WIDTH*8, PROPERTY_HEIGHT, PROPERTY_WIDTH}},

   { "In Jail",               0, {0, 0, 0, 0, 0}, 0, -1, 0, {PROPERTY_LEFT_COLUMN, PROPERTY_BOTTOM_ROW, PROPERTY_HEIGHT, PROPERTY_HEIGHT}}
};

static void OnMouseClick(Ifel* i);

void StartGame(void)
{
   // assumes all players have been added already

   fprintf(stderr, "New game\n");

   srand(time(NULL));
   currentPlayer = rand() % players;
   fprintf(stderr, "player %i starts\n", currentPlayer);
   if (!messageBox)
   {
      messageBox = CreateMessageBox(ID_MSGBOX_ROLL, "Click to roll the dice");
      messageBox->el.OnMouseClick = OnMouseClick;
   }
}

void DoneTurn(void)
{
   fprintf(stderr, "DoneTurn\n");
   int count = 0;

   // check to see if we are down to one player
   for (int i=0; i < players; i++)
   {
      if (player[i].active)
         count++;
   }

   if (count == 1)
   {
      printf("Game over!\n");
      return;
   }

   // otherwise, go to the next active player's turn
   currentPlayer = (currentPlayer + 1) % players;
   while (!player[currentPlayer].active)
      currentPlayer = (currentPlayer + 1) % players;

   fprintf(stderr, "player %i's turn\n", currentPlayer);
   if (!messageBox)
   {
      messageBox = CreateMessageBox(ID_MSGBOX_ROLL, "Click to roll the dice");
      messageBox->el.OnMouseClick = OnMouseClick;
   }
}

void SetPlayerSquare(int id, int square)
{
   fprintf(stderr, "player %i now on %i\n", id, square);
   player[id].location = square;
   image[player[id].token]->el.loc.w = board[square].loc.w;
   image[player[id].token]->el.loc.h = board[square].loc.h;
   image[player[id].token]->el.loc.x = board[square].loc.x + ((image[player[id].token]->el.loc.w - image[player[id].token]->el.loc.w) >> 1); // without the brackets on the >> this doesn't calculate right
   image[player[id].token]->el.loc.y = board[square].loc.y + image[player[id].token]->el.loc.h - image[player[id].token]->el.loc.h;
}

int GetPlayerSquare(int id)
{
   return player[id].location;
}

int AddPlayer(int token, char* name)
{
   if (players >= MAX_PLAYERS)   
   {
      fprintf(stderr, "Maximum players %i is reached\n", MAX_PLAYERS);
      return -1;
   }

   fprintf(stderr, "Adding player %s\n", name);
   player[players].token = token;
   player[players].active = 1;
   player[players].inJail = 0;
   player[players].doublesCount = 0;
   SetPlayerSquare(players, 0);
   player[players].money = STARTING_CASH;
   players++;

   
   image[token]->el.active = 1;

   return players;
}

void RollDice(void)
{
   dice[0] = (rand() % 6) + 1;
   dice[1] = (rand() % 6) + 1;
   fprintf(stderr, "dice: %i %i\n", dice[0], dice[1]);
}

void MovePlayer(int square)
{
   int startSquare = GetPlayerSquare(currentPlayer);

   if (square == ACCORDING_TO_DICE)
   {
      if (dice[0] == dice[1])
         player[currentPlayer].doublesCount++;
      else
         player[currentPlayer].doublesCount = 0;

      // if the player is in jail, its a different set of rules
      if (player[currentPlayer].inJail)
      {
         player[currentPlayer].doublesCount = 0;
         SetPlayerSquare(currentPlayer, SQUARE_JUST_VISITING);
         startSquare = GetPlayerSquare(currentPlayer);
         // from here, join the normal flow as if we had been in 'just visiting'
      }

      if (player[currentPlayer].doublesCount == 3)
      {
         printf("Too many doubles - going to jail\n");
         player[currentPlayer].doublesCount++;
         SetPlayerSquare(currentPlayer, SQUARE_IN_JAIL);
      }
      else
      {
         SetPlayerSquare(currentPlayer, (startSquare + dice[0] + dice[1]) % NUM_PROPERTIES);
      }
   }
   else
   {
      SetPlayerSquare(currentPlayer, square);
   }

   int endSquare = GetPlayerSquare(currentPlayer);
   printf("You landed on %s\n", board[endSquare].name);

   // did you pass go? (and not go to jail)
   if ((endSquare != SQUARE_IN_JAIL) && (endSquare < startSquare))
   {
      printf("Passed GO - got $200\n");
      player[currentPlayer].money += 200;
   }

   // check the special squares first
   int handled = 0;
   switch (endSquare)
   {
   case SQUARE_GO:
      printf("Got $200\n");
      player[currentPlayer].money += 200;
      handled = 1;
      break;

   case SQUARE_INCOME_TAX:
      printf("Pay income tax\n");
      player[currentPlayer].money -= 200;
      handled = 1;
      break;

   case SQUARE_JUST_VISITING:
   case SQUARE_FREE_PARKING:
      printf("Nothing happens - this time\n");
      handled = 1;
      break;
      
   case SQUARE_GO_TO_JAIL:
      printf("That's it - off to jail\n");
      SetPlayerSquare(currentPlayer, SQUARE_IN_JAIL);
      handled = 1;
      break;

   case SQUARE_LUXURY_TAX:
      printf("Pay luxury tax\n");
      player[currentPlayer].money -= 75;
      handled = 1;
      break;

   // community chest
   case 2:
   case 17:
   case 33:
      printf("Pick a CC card\n");
      handled = 1;
      break;

   case 7:
   case 22:
   case 36:
      printf("Pick a Chance card\n");
      handled = 1;
      break;
   }

   if (!handled)
   {
      // otherwise, it is a property. Is it unowned?
      if (board[endSquare].owner == -1)
      {
         printf("unowned\n");
         char msg[100];
         sprintf(msg, "Would you like to buy %s? It costs $%i", board[endSquare].name, board[endSquare].value);
         if (ModalMessageBox(ID_MSGBOX_BUY_PROPERTY, msg) == MB_YES)
         {
            player[currentPlayer].money -= board[endSquare].value; 
            board[endSquare].owner = currentPlayer;
            printf("%i bought %s\n", currentPlayer, board[endSquare].name);
         }
         else
         {
            printf("auctions not implemented yet\n");
         }
      }
      else
      {
         int rent;
         printf("owned\n");
         if (board[endSquare].mortgaged)
         {
            printf("mortgaged\n");
         }
         else
         {
            if (  endSquare == SQUARE_READING_RR 
               || endSquare == SQUARE_PENNSYLVANIA_RR
               || endSquare == SQUARE_BO_RR
               || endSquare == SQUARE_SHORT_LINE_RR)
            {
               int rrCount = 0;

               // count how many other railroads are owned
               if (board[SQUARE_READING_RR].owner == currentPlayer)
                  rrCount++;
               if (board[SQUARE_PENNSYLVANIA_RR].owner == currentPlayer)
                  rrCount++;
               if (board[SQUARE_BO_RR].owner == currentPlayer)
                  rrCount++;
               if (board[SQUARE_SHORT_LINE_RR].owner == currentPlayer)
                  rrCount++;

               // multiply the count by 50
               rent = 50 * rrCount;
            }
            else if ( endSquare == SQUARE_ELECTRIC_CO
               || endSquare == SQUARE_WATER_WORKS)
            {
               // one utility is x4, both utilities is x10
               if (board[SQUARE_ELECTRIC_CO].owner == currentPlayer
                  && board[SQUARE_WATER_WORKS].owner == currentPlayer)
                  rent = (dice[0] + dice[1]) * 10;
               else
                  rent = (dice[0] + dice[1]) * 4;
            }
            else
            {
               rent = board[endSquare].rent[board[endSquare].numHouses];
            }
            printf("You owe %i in rent\n", rent);
   
            player[currentPlayer].money -= rent; 
            player[board[endSquare].owner].money += rent;
         }
      }
   }
   DoneTurn();
}

static void OnMouseClick(Ifel* i)
{
   fprintf(stderr, "OnMouseClick in %i\n", i->id);
   switch(i->id)
   {
   case ID_BTN_OPTIONS:
      fprintf(stderr, "Click in options button\n");
      //ShowOptionsMenu();
      break;

   case ID_MSGBOX_NEW_GAME:
      DeleteMessageBox(messageBox);
      messageBox = NULL;
      AddPlayer(ID_IMG_SHOE, "Joel");
      AddPlayer(ID_IMG_DOG, "Merav");
      StartGame();
      break;

   case ID_MSGBOX_ROLL:
      DeleteMessageBox(messageBox);
      messageBox = NULL;
      RollDice();
      MovePlayer(ACCORDING_TO_DICE);
      break;
   }
}

void OnKeyPressed(Ifel* el, int key)
{
   switch(key)
   {
   case 'y':
      break;

   case 'n':
      if (!messageBox)
      {
         messageBox = CreateMessageBox(ID_MSGBOX_NEW_GAME, "New Game");
         messageBox->el.OnMouseClick = OnMouseClick;
      }
      break;
   }
}

int main(int argc, char* args[])
{
   if (InitUI() != 0)
   {
      fprintf(stderr, "Can't init UI\n");
      return 1;
   }

   image[ID_IMG_BOARD] = CreateImage(ID_IMG_BOARD, NULL, "graphics/board.png");
   image[ID_IMG_BOARD]->el.OnKeyPressed = OnKeyPressed;
 
   // load player piece images
   for (int i=ID_IMG_HORSE; i < ID_IMG_COUNT; i++)
   {
      char filename[64];
      sprintf(filename, "graphics/%s.png", piece_name[i-ID_IMG_HORSE]);
      image[i] = CreateImage(i, NULL, filename);
      if (!image[i])
      {
         fprintf(stderr, "Can't load image\n");

         // unload all successfully loaded pieces
         for (int j=i-1; j >=ID_IMG_HORSE; j--)
            DeleteImage(image[j]);

         return 1;
      }
      image[i]->el.active = 0;
   }

   // load buttons
   button[0] = CreateButton(ID_BTN_OPTIONS, NULL, 0, 708, "graphics/btn_options_1.png", "graphics/btn_options_2.png", "graphics/btn_options_3.png");
   if (!button[0])
   {
      fprintf(stderr, "Can't create options button\n");
      return 2;
   }
   button[0]->el.OnMouseClick = OnMouseClick;

   Run();

   fprintf(stderr, "Cleanup\n");
   if (messageBox)
   {
      DeleteMessageBox(messageBox);
      messageBox = NULL; // not completely necessary, but a good habit
   }

   // free UI elements
   for (int i=ID_IMG_BASE+1; i < ID_IMG_BASE+ID_IMG_COUNT; i++)
      DeleteImage(image[i]);

   for (int i=ID_BTN_BASE; i < ID_BTN_BASE+ID_BTN_COUNT; i++)
   {
      DeleteButton(button[i-ID_BTN_BASE]);
   }
   DeleteOptionsMenu();

   DestroyUI();

   return 0;
}


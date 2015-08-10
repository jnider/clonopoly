#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_gfxPrimitives.h"
#include "SDL/SDL_ttf.h"
#include "ui.h"
#include "id.h"
#include "options.h"
#include "status.h"
#include "game.h"

#define IS_RAILROAD(_s) (_s == SQUARE_READING_RR || _s == SQUARE_PENNSYLVANIA_RR || _s == SQUARE_BO_RR || _s == SQUARE_SHORT_LINE_RR)

Image* image[ID_IMG_COUNT];

static const char* piece_name[] =
{
   "piece_horse",
   "piece_shoe",
   "piece_dog",
   "piece_bag"
};

static MessageBox* messageBox;
static Button* button[ID_BTN_COUNT];
static int players;
static Player player[MAX_PLAYERS]; // the actual players
static int currentPlayer;
static int dice[2];
static int gameOver;
static Property board[NUM_PROPERTIES+1] =
{
   { "Go",                    0,   SET_NONE,    { 0,   0,   0,   0,   0,   0},   0,   0, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*9, PROPERTY_BOTTOM_ROW, PROPERTY_HEIGHT, PROPERTY_HEIGHT}},
   { "Mediterranean",         60,  SET_PURPLE,  { 2,  10,  30,  90, 160, 250},  30,  50, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*8, PROPERTY_BOTTOM_ROW, PROPERTY_WIDTH, PROPERTY_HEIGHT}},
   { "Community Chest",       0,   SET_NONE,    { 0,   0,   0,   0,   0,   0},   0,   0, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*7, PROPERTY_BOTTOM_ROW, PROPERTY_WIDTH, PROPERTY_HEIGHT}},
   { "Baltic",                60,  SET_PURPLE,  { 4,  20,  60, 180, 320, 450},  30,  50, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*6, PROPERTY_BOTTOM_ROW, PROPERTY_WIDTH, PROPERTY_HEIGHT}},
   { "Income Tax",            0,   SET_NONE,    { 0,   0,   0,   0,   0,   0},   0,   0, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*5, PROPERTY_BOTTOM_ROW, PROPERTY_WIDTH, PROPERTY_HEIGHT}},
   { "Reading Railroad",      200, SET_RR,      {25,   0,   0,   0,   0,   0}, 100,   0, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*4, PROPERTY_BOTTOM_ROW, PROPERTY_WIDTH, PROPERTY_HEIGHT}},
   { "Oriental Ave",          100, SET_LT_BLUE, { 6,  30,  90, 270, 400, 550},  50,  50, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*3, PROPERTY_BOTTOM_ROW, PROPERTY_WIDTH, PROPERTY_HEIGHT}},
   { "Chance",                0,   SET_NONE,    { 0,   0,   0,   0,   0,   0},   0,   0, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*2, PROPERTY_BOTTOM_ROW, PROPERTY_WIDTH, PROPERTY_HEIGHT}},
   { "Vermont Ave",           100, SET_LT_BLUE, { 6,  30,  90, 270, 400, 550},  50,  50, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*1, PROPERTY_BOTTOM_ROW, PROPERTY_WIDTH, PROPERTY_HEIGHT}},
   { "Connecticut Ave",       120, SET_LT_BLUE, { 8,  40, 100, 300, 450, 600},  60,  50, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT, PROPERTY_BOTTOM_ROW, PROPERTY_WIDTH, PROPERTY_HEIGHT}},

   { "Just Visiting",         0,   SET_NONE,    { 0,   0,   0,   0,   0,    0},   0,   0, 0, -1, 0, {PROPERTY_LEFT_COLUMN, PROPERTY_BOTTOM_ROW, PROPERTY_HEIGHT, PROPERTY_HEIGHT}},
   { "St. Charles",           140, SET_VIOLET,  {10,  50, 150, 450, 625,  750},  70, 100, 0, -1, 0, {PROPERTY_LEFT_COLUMN, PROPERTY_TOP_ROW+PROPERTY_HEIGHT+PROPERTY_WIDTH*8, PROPERTY_HEIGHT, PROPERTY_WIDTH}},
   { "Electric Company",      150, SET_UTIL,    { 0,   0,   0,   0,   0,    0},   0,   0, 0, -1, 0, {PROPERTY_LEFT_COLUMN, PROPERTY_TOP_ROW+PROPERTY_HEIGHT+PROPERTY_WIDTH*7, PROPERTY_HEIGHT, PROPERTY_WIDTH}},
   { "States Ave",            140, SET_VIOLET,  {10,  50, 150, 450, 625,  750},  70, 100, 0, -1, 0, {PROPERTY_LEFT_COLUMN, PROPERTY_TOP_ROW+PROPERTY_HEIGHT+PROPERTY_WIDTH*6, PROPERTY_HEIGHT, PROPERTY_WIDTH}},
   { "Virginia Ave",          160, SET_VIOLET,  {12,  60, 180, 500, 700,  900},  80, 100, 0, -1, 0, {PROPERTY_LEFT_COLUMN, PROPERTY_TOP_ROW+PROPERTY_HEIGHT+PROPERTY_WIDTH*5, PROPERTY_HEIGHT, PROPERTY_WIDTH}},
   { "Pennsylvania Railroad", 200, SET_RR,      {25,   0,   0,   0,   0,    0}, 100,   0, 0, -1, 0, {PROPERTY_LEFT_COLUMN, PROPERTY_TOP_ROW+PROPERTY_HEIGHT+PROPERTY_WIDTH*4, PROPERTY_HEIGHT, PROPERTY_WIDTH}},
   { "St. James Place",       180, SET_GOLD,    {14,  70, 200, 550, 750,  950},  90, 100, 0, -1, 0, {PROPERTY_LEFT_COLUMN, PROPERTY_TOP_ROW+PROPERTY_HEIGHT+PROPERTY_WIDTH*3, PROPERTY_HEIGHT, PROPERTY_WIDTH}},
   { "Community Chest",       0,   SET_NONE,    { 0,   0,   0,   0,   0,    0},   0,   0, 0, -1, 0, {PROPERTY_LEFT_COLUMN, PROPERTY_TOP_ROW+PROPERTY_HEIGHT+PROPERTY_WIDTH*2, PROPERTY_HEIGHT, PROPERTY_WIDTH}},
   { "Tennessee Ave",         180, SET_GOLD,    {14,  70, 200, 550, 750,  950},  90, 100, 0, -1, 0, {PROPERTY_LEFT_COLUMN, PROPERTY_TOP_ROW+PROPERTY_HEIGHT+PROPERTY_WIDTH*1, PROPERTY_HEIGHT, PROPERTY_WIDTH}},
   { "New York Ave",          200, SET_GOLD,    {16,  80, 220, 600, 800, 1000}, 100, 100, 0, -1, 0, {PROPERTY_LEFT_COLUMN, PROPERTY_TOP_ROW+PROPERTY_HEIGHT, PROPERTY_HEIGHT, PROPERTY_WIDTH}},

   { "Free Parking",          0,   SET_NONE,    { 0,   0,   0,   0,    0,    0},   0,   0, 0, -1, 0, {PROPERTY_LEFT_COLUMN, PROPERTY_TOP_ROW, PROPERTY_HEIGHT, PROPERTY_HEIGHT}},
   { "Kentucky Ave",          220, SET_RED,     {18,  90, 250, 700,  875, 1050}, 110, 150, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT, PROPERTY_TOP_ROW, PROPERTY_WIDTH, PROPERTY_HEIGHT}},
   { "Chance",                0,   SET_NONE,    { 0,   0,   0,   0,    0,    0},   0,   0, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*1, PROPERTY_TOP_ROW, PROPERTY_WIDTH, PROPERTY_HEIGHT}},
   { "Indiana",               220, SET_RED,     {18,  90, 250, 700,  875, 1050}, 110, 150, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*2, PROPERTY_TOP_ROW, PROPERTY_WIDTH, PROPERTY_HEIGHT}},
   { "Illinois",              240, SET_RED,     {20, 100, 300, 750,  925, 1100}, 120, 150, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*3, PROPERTY_TOP_ROW, PROPERTY_WIDTH, PROPERTY_HEIGHT}},
   { "B & O Railroad",        200, SET_RR,      {25,   0,   0,   0,    0,    0}, 100,   0, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*4, PROPERTY_TOP_ROW, PROPERTY_WIDTH, PROPERTY_HEIGHT}},
   { "Atlantic Ave",          260, SET_YELLOW,  {22, 110, 330, 800,  975, 1150}, 130, 150, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*5, PROPERTY_TOP_ROW, PROPERTY_WIDTH, PROPERTY_HEIGHT}},
   { "Ventnor Ave",           260, SET_YELLOW,  {22, 110, 330, 800,  975, 1150}, 130, 150, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*6, PROPERTY_TOP_ROW, PROPERTY_WIDTH, PROPERTY_HEIGHT}},
   { "Water Works",           150, SET_UTIL,    { 0,   0,   0,   0,    0,    0},  75,   0, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*7, PROPERTY_TOP_ROW, PROPERTY_WIDTH, PROPERTY_HEIGHT}},
   { "Marvin Gardens",        280, SET_YELLOW,  {24, 120, 360, 850, 1025, 1200}, 140, 150, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*8, PROPERTY_TOP_ROW, PROPERTY_WIDTH, PROPERTY_HEIGHT}},

   { "Go To Jail",            0,   SET_NONE,    { 0,   0,   0,    0,    0,    0},   0,   0, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*9, PROPERTY_TOP_ROW, PROPERTY_HEIGHT, PROPERTY_HEIGHT}},
   { "Pacific Ave",           300, SET_GREEN,   {26, 130, 390,  900, 1100, 1275}, 150, 200, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*9, PROPERTY_TOP_ROW+PROPERTY_HEIGHT, PROPERTY_HEIGHT, PROPERTY_WIDTH}},
   { "North Carolina Ave",    300, SET_GREEN,   {26, 130, 390,  900, 1100, 1275}, 150, 200, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*9, PROPERTY_TOP_ROW+PROPERTY_HEIGHT+PROPERTY_WIDTH*1, PROPERTY_HEIGHT, PROPERTY_WIDTH}},
   { "Community Chest",       0,   SET_NONE,    { 0,   0,   0,    0,    0,    0},   0,   0, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*9, PROPERTY_TOP_ROW+PROPERTY_HEIGHT+PROPERTY_WIDTH*2, PROPERTY_HEIGHT, PROPERTY_WIDTH}},
   { "Pennsylvania Ave",      320, SET_GREEN,   {28, 150, 450, 1000, 1200, 1400}, 160, 200, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*9, PROPERTY_TOP_ROW+PROPERTY_HEIGHT+PROPERTY_WIDTH*3, PROPERTY_HEIGHT, PROPERTY_WIDTH}},
   { "Short Line Railroad",   200, SET_RR,      {25,   0,   0,    0,    0,    0}, 100,   0, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*9, PROPERTY_TOP_ROW+PROPERTY_HEIGHT+PROPERTY_WIDTH*4, PROPERTY_HEIGHT, PROPERTY_WIDTH}},
   { "Chance",                0,   SET_NONE,    { 0,   0,   0,    0,    0,    0},   0,   0, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*9, PROPERTY_TOP_ROW+PROPERTY_HEIGHT+PROPERTY_WIDTH*5, PROPERTY_HEIGHT, PROPERTY_WIDTH}},
   { "Park Place",            350, SET_BLUE,    {35, 175, 500, 1100, 1300, 1500}, 175, 200, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*9, PROPERTY_TOP_ROW+PROPERTY_HEIGHT+PROPERTY_WIDTH*6, PROPERTY_HEIGHT, PROPERTY_WIDTH}},
   { "Luxury Tax",            0,   SET_NONE,    { 0,   0,   0,    0,    0,    0},   0,   0, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*9, PROPERTY_TOP_ROW+PROPERTY_HEIGHT+PROPERTY_WIDTH*7, PROPERTY_HEIGHT, PROPERTY_WIDTH}},
   { "Boardwalk",             400, SET_BLUE,    {50, 200, 600, 1400, 1700, 2000}, 200, 200, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*9, PROPERTY_TOP_ROW+PROPERTY_HEIGHT+PROPERTY_WIDTH*8, PROPERTY_HEIGHT, PROPERTY_WIDTH}},

   { "In Jail",               0,   SET_NONE,    { 0,   0,   0,    0,    0,    0},   0,   0, 0, -1, 0, {PROPERTY_LEFT_COLUMN, PROPERTY_BOTTOM_ROW, PROPERTY_HEIGHT, PROPERTY_HEIGHT}}
};

static void OnMouseClick(Ifel* i);

void StartGame(void)
{
   // assumes all players have been added already

   fprintf(stderr, "New game\n");

   srand(time(NULL));
   currentPlayer = rand() % players;
   fprintf(stderr, "player %s starts\n", player[currentPlayer].name);

   // reset ownership of all properties
   Property* p = GetProperties();
   for (int i=0; i < NUM_PROPERTIES; p++, i++)
      p->owner = -1;
   
   SetCurrentPlayerStatusArea(&player[currentPlayer]);
   EnableStatusArea();
   EnableButton(button[BUTTON_INDEX(ID_BTN_ROLL)]);
}

int HasAssets(int playerID)
{
   // do you have anything to mortgage?

   // do you have any houses you can sell?

   return 0;
}

void GameOver(void)
{
   gameOver = 1;
   DisableStatusArea();
}

static void PlayerOutOfBusiness(int playerID)
{
   player[playerID].active = 0;
   players--;
   
   if (players == 1)
   {
      GameOver();
   }
}

void PayMoney(int from, int to, int amount)
{
   if (from != PLAYER_BANK)
   {
      fprintf(stderr, "current: %i amount: %i\n", player[from].money, amount);
      player[from].money -= amount;

      // check to see if we are out of money
      while (player[from].money < 0 && HasAssets(from))
      {
         fprintf(stderr, "Out of money!\n");
         //LiquidateAssetsDlg();
      }

      if (player[from].money < 0)
      {
         PlayerOutOfBusiness(from);
      }
   }

   if (to != PLAYER_BANK)
      player[to].money += amount;
}

static void StartTurn(void)
{
   SetCurrentPlayerStatusArea(&player[currentPlayer]);

   fprintf(stderr, "%s's turn\n", player[currentPlayer].name);

   if (player[currentPlayer].inJail)
      fprintf(stderr, "%s is in jail\n", player[currentPlayer].name);
}

static void DoneTurn(void)
{

/* this check is performed in PlayerOutOfBusiness
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
*/

   // reset the doubles counter
   player[currentPlayer].doublesCount = 0;

   // otherwise, go to the next active player's turn
   currentPlayer = (currentPlayer + 1) % players;
   while (!player[currentPlayer].active)
      currentPlayer = (currentPlayer + 1) % players;

   StartTurn();
}

void SetPlayerName(int id, const char* name)
{
   if (player[id].name)
   {
      free(player[id].name);
      player[id].name = 0;
   }

   player[id].name = malloc(strlen(name) + 1);
   strcpy(player[id].name, name);

   fprintf(stderr, "set player %i name to %s\n", id, player[id].name);
}

void SetPlayerSquare(int id, int square)
{
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

Property* GetProperties(void)
{
   return board;
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
   SetPlayerName(players, name);
   player[players].index = players;
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

static void MovePlayer(int square)
{
   int startSquare = GetPlayerSquare(currentPlayer);

   if (square == ACCORDING_TO_DICE)
   {
      // first see if the player is in jail
      if (player[currentPlayer].inJail)
      {
         player[currentPlayer].turnsLeftInJail--;
         printf("%s is in jail - %i rolls left\n", player[currentPlayer].name, player[currentPlayer].turnsLeftInJail);
         player[currentPlayer].doublesCount = 0; // can't accumulate doubles in jail

         // if you got doubles, you're out!
         if (dice[0] == dice[1])
         {
            printf("%s is out of jail (rolled doubles)\n", player[currentPlayer].name);
            player[currentPlayer].inJail = 0;
            player[currentPlayer].turnsLeftInJail = 0;
            player[currentPlayer].doublesCount++;
            SetPlayerSquare(currentPlayer, SQUARE_JUST_VISITING);
            startSquare = GetPlayerSquare(currentPlayer);
            // from here, join the normal flow as if we had been in 'just visiting'
         }
         else
         {
            if (player[currentPlayer].turnsLeftInJail == 0)
            { 
               // check if the player has a get out of jail free card
               // ask if the player wants to use a get out of jail free card
               // no? then he has to pay
               printf("%s has to pay to get out of jail\n", player[currentPlayer].name);
               PayMoney(currentPlayer, PLAYER_BANK, 50);
               player[currentPlayer].inJail = 0;
               player[currentPlayer].turnsLeftInJail = 0;
               SetPlayerSquare(currentPlayer, SQUARE_JUST_VISITING);
               startSquare = GetPlayerSquare(currentPlayer);
               printf("%s is out of jail (paid $50)\n", player[currentPlayer].name);
            }
         }
      }
      else
      {
         // check for doubles
         if (dice[0] == dice[1])
            player[currentPlayer].doublesCount++;
         else
            player[currentPlayer].doublesCount = 0;
      }

      // did you roll too many doubles in a row?
      if (player[currentPlayer].doublesCount == 3)
      {
         printf("Too many doubles - going to jail\n");
         SetPlayerSquare(currentPlayer, SQUARE_IN_JAIL);
      }
      else
      {
         SetPlayerSquare(currentPlayer, (startSquare + dice[0] + dice[1]) % NUM_PROPERTIES);
      }
   }
   else
   {
      // jump to a specific square, like 'jail', or something from a 'chance' or 'community chest'
      SetPlayerSquare(currentPlayer, square);
   }

   int endSquare = GetPlayerSquare(currentPlayer);
   printf("%s landed on %s\n", player[currentPlayer].name, board[endSquare].name);

   // did you pass go? (and not go to jail)
   if ((endSquare != SQUARE_IN_JAIL) && (endSquare < startSquare))
   {
      printf("Passed GO - got $200\n");
      PayMoney(PLAYER_BANK, currentPlayer, 200);
   }

   // check the special squares first
   int handled = 0;
   switch (endSquare)
   {
   case SQUARE_GO:
      printf("Got $200\n");
      PayMoney(PLAYER_BANK, currentPlayer, 200);
      handled = 1;
      break;

   case SQUARE_INCOME_TAX:
      printf("Pay income tax\n");
      // give option to calculate 10% of assets - JKN
      PayMoney(currentPlayer, PLAYER_BANK, 200);
      handled = 1;
      break;

   case SQUARE_JUST_VISITING:
   case SQUARE_FREE_PARKING:
      printf("Nothing happens - this time\n");
      handled = 1;
      break;
      
   case SQUARE_LUXURY_TAX:
      printf("Pay luxury tax\n");
      PayMoney(currentPlayer, PLAYER_BANK, 75);
      handled = 1;
      break;

   case SQUARE_GO_TO_JAIL:
      printf("That's it - off to jail\n");
      SetPlayerSquare(currentPlayer, SQUARE_IN_JAIL);
   case SQUARE_IN_JAIL:
      printf("%s is in jail\n", player[currentPlayer].name);
      player[currentPlayer].inJail = 1;
      player[currentPlayer].turnsLeftInJail = 3;
      handled = 1;
      break;

   // community chest
   case 2:
   case 17:
   case 33:
      //printf("Pick a CC card\n");
      handled = 1;
      break;

   case 7:
   case 22:
   case 36:
      //printf("Pick a Chance card\n");
      handled = 1;
      break;
   }

   if (!handled)
   {
      // otherwise, it is a property. Is it unowned?
      if (board[endSquare].owner == -1)
      {
         char msg[100];

         // if you have enough money, do you want to buy the property?
         if (player[currentPlayer].money < board[endSquare].value) 
         {
            printf("You don't have enough money!\n");
            goto nomoney;
         }

         sprintf(msg, "Would you like to buy %s? It costs $%i", board[endSquare].name, board[endSquare].value);
         if (ModalMessageBox(ID_MSGBOX_BUY_PROPERTY, msg) == MB_YES)
         {
            player[currentPlayer].money -= board[endSquare].value; 
            board[endSquare].owner = currentPlayer;
            printf("%s bought %s\n", player[currentPlayer].name, board[endSquare].name);

            // check to see if the player owns a set
         }
         else
         {
nomoney:
            // no money, or no will
            printf("auctions not implemented yet\n");
            // Auction(endSquare); 
         }
      }
      else
      {
         // property is already owned
         int rent;
         int owner = board[endSquare].owner;
         if (owner == currentPlayer)
         {
            printf("owned by you!\n");
         }
         else
         {
            printf("owned by %s\n", player[owner].name);
            if (board[endSquare].mortgaged)
            {
               printf("mortgaged\n");
            }
            else
            {
               if (IS_RAILROAD(endSquare))
               {
                  int rrCount = 0;

                  // count how many railroads are owned
                  if (board[SQUARE_READING_RR].owner == owner)
                     rrCount++;
                  if (board[SQUARE_PENNSYLVANIA_RR].owner == owner)
                     rrCount++;
                  if (board[SQUARE_BO_RR].owner == owner)
                     rrCount++;
                  if (board[SQUARE_SHORT_LINE_RR].owner == owner)
                     rrCount++;

                  // double rent for each additional railroad
                  rent = board[endSquare].rent[0] << (rrCount-1);
               }
               else if ( endSquare == SQUARE_ELECTRIC_CO
                     || endSquare == SQUARE_WATER_WORKS)
               {
                  int owner = board[endSquare].owner;
                  // one utility is x4, both utilities is x10
                  if (board[SQUARE_ELECTRIC_CO].owner == owner
                     && board[SQUARE_WATER_WORKS].owner == owner)
                     rent = (dice[0] + dice[1]) * 10;
                  else
                     rent = (dice[0] + dice[1]) * 4;
               }
               else
               {
                  rent = board[endSquare].rent[board[endSquare].numHouses];
               }
               printf("%s owes %i in rent\n", player[currentPlayer].name, rent);
   
               PayMoney(currentPlayer, board[endSquare].owner, rent);
            }
         }
      }
   }

   // if you got doubles and you are not in jail, roll again
   if (dice[0] == dice[1] && !player[currentPlayer].inJail)
   {
      if (!messageBox)
      {
         messageBox = CreateMessageBox(ID_MSGBOX_ROLL, "Doubles - roll again");
         messageBox->el.OnMouseClick = OnMouseClick;
      }
   }
   else
   {
      DoneTurn();
   }
}

/* A generic function to handle all mouse clicks */
/* Most likely this should be broken up for more modular logic */
static void OnMouseClick(Ifel* i)
{
   //fprintf(stderr, "OnMouseClick in %i\n", i->id);
   switch(i->id)
   {
   case ID_BTN_OPTIONS:
      //fprintf(stderr, "Click in options button\n");
      ShowOptionsMenu();
      break;

   case ID_MSGBOX_ROLL:
   case ID_BTN_ROLL:
      RollDice();
      MovePlayer(ACCORDING_TO_DICE);
      break;
   }

   if (messageBox)
   {
      DeleteMessageBox(messageBox);
      messageBox = NULL;
   }
   //fprintf(stderr, "OnMouseClick complete\n");
}

void OnKeyPressed(Ifel* el, int key)
{
   switch(key)
   {
   case 'y':
      break;

   case 'n':
      if (gameOver || ModalMessageBox(ID_MSGBOX_NEW_GAME, "Start a new game?") == MB_YES)
      {
         AddPlayer(ID_IMG_SHOE, "Joel");
         AddPlayer(ID_IMG_DOG, "Merav");
         StartGame();
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
   SDL_Rect posRect;
   int index = BUTTON_INDEX(ID_BTN_OPTIONS);
   //button[index] = CreatePngButton(ID_BTN_OPTIONS, NULL, 0, image[ID_IMG_BOARD]->el.loc.y + image[ID_IMG_BOARD]->el.loc.h,
   //   "graphics/btn_options_1.png",
   //   "graphics/btn_options_2.png",
   //   "graphics/btn_options_3.png");
   posRect.x = 0;
   posRect.y = image[ID_IMG_BOARD]->el.loc.y + image[ID_IMG_BOARD]->el.loc.h;
   posRect.w = 200;
   posRect.h = 50;
   button[index] = CreateTextButton(ID_BTN_OPTIONS, NULL, &posRect, "OPTIONS");
   if (!button[index])
   {
      fprintf(stderr, "Can't create options button\n");
      return 2;
   }
   button[index]->el.OnMouseClick = OnMouseClick;

   index = BUTTON_INDEX(ID_BTN_ROLL);
   posRect.x = button[BUTTON_INDEX(ID_BTN_OPTIONS)]->el.loc.x + button[BUTTON_INDEX(ID_BTN_OPTIONS)]->el.loc.w + 5;
   button[index] = CreateTextButton(ID_BTN_ROLL, NULL, &posRect, "ROLL");
   if (!button[index])
   {
      fprintf(stderr, "Can't create roll button\n");
      return 2;
   }
   button[index]->el.OnMouseClick = OnMouseClick;
   DisableButton(button[index]);

   index = BUTTON_INDEX(ID_BTN_BUY_SELL);
   posRect.x = button[BUTTON_INDEX(ID_BTN_ROLL)]->el.loc.x + button[BUTTON_INDEX(ID_BTN_ROLL)]->el.loc.w + 5;
   button[index] = CreateTextButton(ID_BTN_ROLL, NULL, &posRect, "BUY/SELL");
   if (!button[index])
   {
      fprintf(stderr, "Can't create buy/sell button\n");
      return 2;
   }
   button[index]->el.OnMouseClick = OnMouseClick;
   DisableButton(button[index]);

   // create status area
   posRect.x = image[ID_IMG_BOARD]->el.loc.x + image[ID_IMG_BOARD]->el.loc.w;
   posRect.y = 0;
   posRect.w = 275;
   posRect.h = 600;
   if (CreateStatusArea(&posRect) != 0)
   {
      fprintf(stderr, "Can't create status area\n");
      return 3;
   }
   StatusAreaSetPlayerIconList(image, ID_IMG_COUNT);

   // start from a clean slate
   GameOver();

   // start the game
   Run();

   fprintf(stderr, "Save game\n");
   if (!gameOver)
   {
      if (ModalMessageBox(ID_MB_YESNO, "Would you like to save the current game?") == MB_YES)
      {
         //SaveGame("game1.clonopoly");
      }
   }

   fprintf(stderr, "Cleanup\n");
   if (messageBox)
   {
      DeleteMessageBox(messageBox);
      messageBox = NULL; // not completely necessary, but a good habit
   }

   DeleteStatusArea();

   // free UI elements
   for (int i=ID_IMG_BASE+1; i < ID_IMG_BASE+ID_IMG_COUNT; i++)
      DeleteImage(image[i]);

   for (int i=ID_BTN_BASE; i < ID_BTN_BASE+ID_BTN_COUNT; i++)
   {
      DeleteButton(button[i-ID_BTN_BASE]);
   }
   DeleteOptionsMenu();

   DestroyUI();
   for (int i=0; i < MAX_PLAYERS; i++)
   {
      if (player[i].name)
      {
         free(player[i].name);
         player[i].name = 0;
      }
   }

   return 0;
}


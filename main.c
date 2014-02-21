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
static int gameOver;
static Property board[NUM_PROPERTIES+1] =
{
   { "Go",                    0,   { 0,   0,   0,   0,   0,   0},   0,   0, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*9, PROPERTY_BOTTOM_ROW, PROPERTY_HEIGHT, PROPERTY_HEIGHT}},
   { "Mediterranean",         60,  { 2,  10,  30,  90, 160, 250},  30,  50, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*8, PROPERTY_BOTTOM_ROW, PROPERTY_WIDTH, PROPERTY_HEIGHT}},
   { "Community Chest",       0,   { 0,   0,   0,   0,   0,   0},   0,   0, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*7, PROPERTY_BOTTOM_ROW, PROPERTY_WIDTH, PROPERTY_HEIGHT}},
   { "Baltic",                60,  { 4,  20,  60, 180, 320, 450},  30,  50, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*6, PROPERTY_BOTTOM_ROW, PROPERTY_WIDTH, PROPERTY_HEIGHT}},
   { "Income Tax",            0,   { 0,   0,   0,   0,   0,   0},   0,   0, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*5, PROPERTY_BOTTOM_ROW, PROPERTY_WIDTH, PROPERTY_HEIGHT}},
   { "Reading Railroad",      200, { 0,   0,   0,   0,   0,   0}, 100,   0, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*4, PROPERTY_BOTTOM_ROW, PROPERTY_WIDTH, PROPERTY_HEIGHT}},
   { "Oriental Ave",          100, { 6,  30,  90, 270, 400, 550},  50,  50, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*3, PROPERTY_BOTTOM_ROW, PROPERTY_WIDTH, PROPERTY_HEIGHT}},
   { "Chance",                0,   { 0,   0,   0,   0,   0,   0},   0,   0, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*2, PROPERTY_BOTTOM_ROW, PROPERTY_WIDTH, PROPERTY_HEIGHT}},
   { "Vermont Ave",           100, { 6,  30,  90, 270, 400, 550},  50,  50, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*1, PROPERTY_BOTTOM_ROW, PROPERTY_WIDTH, PROPERTY_HEIGHT}},
   { "Connecticut Ave",       120, { 8,  40, 100, 300, 450, 600},  60,  50, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT, PROPERTY_BOTTOM_ROW, PROPERTY_WIDTH, PROPERTY_HEIGHT}},

   { "Just Visiting",         0,   { 0,   0,   0,   0,   0,    0},   0,   0, 0, -1, 0, {PROPERTY_LEFT_COLUMN, PROPERTY_BOTTOM_ROW, PROPERTY_HEIGHT, PROPERTY_HEIGHT}},
   { "St. Charles",           140, {10,  50, 150, 450, 625,  750},  70, 100, 0, -1, 0, {PROPERTY_LEFT_COLUMN, PROPERTY_TOP_ROW+PROPERTY_HEIGHT+PROPERTY_WIDTH*8, PROPERTY_HEIGHT, PROPERTY_WIDTH}},
   { "Electric Company",      150, { 0,   0,   0,   0,   0,    0},   0,   0, 0, -1, 0, {PROPERTY_LEFT_COLUMN, PROPERTY_TOP_ROW+PROPERTY_HEIGHT+PROPERTY_WIDTH*7, PROPERTY_HEIGHT, PROPERTY_WIDTH}},
   { "States Ave",            140, {10,  50, 150, 450, 625,  750},  70, 100, 0, -1, 0, {PROPERTY_LEFT_COLUMN, PROPERTY_TOP_ROW+PROPERTY_HEIGHT+PROPERTY_WIDTH*6, PROPERTY_HEIGHT, PROPERTY_WIDTH}},
   { "Virginia Ave",          160, {12,  60, 180, 500, 700,  900},  80, 100, 0, -1, 0, {PROPERTY_LEFT_COLUMN, PROPERTY_TOP_ROW+PROPERTY_HEIGHT+PROPERTY_WIDTH*5, PROPERTY_HEIGHT, PROPERTY_WIDTH}},
   { "Pennsylvania Railroad", 200, { 0,   0,   0,   0,   0,    0}, 100,   0, 0, -1, 0, {PROPERTY_LEFT_COLUMN, PROPERTY_TOP_ROW+PROPERTY_HEIGHT+PROPERTY_WIDTH*4, PROPERTY_HEIGHT, PROPERTY_WIDTH}},
   { "St. James Place",       180, {14,  70, 200, 550, 750,  950},  90, 100, 0, -1, 0, {PROPERTY_LEFT_COLUMN, PROPERTY_TOP_ROW+PROPERTY_HEIGHT+PROPERTY_WIDTH*3, PROPERTY_HEIGHT, PROPERTY_WIDTH}},
   { "Community Chest",       0,   { 0,   0,   0,   0,   0,    0},   0,   0, 0, -1, 0, {PROPERTY_LEFT_COLUMN, PROPERTY_TOP_ROW+PROPERTY_HEIGHT+PROPERTY_WIDTH*2, PROPERTY_HEIGHT, PROPERTY_WIDTH}},
   { "Tennessee Ave",         180, {14,  70, 200, 550, 750,  950},  90, 100, 0, -1, 0, {PROPERTY_LEFT_COLUMN, PROPERTY_TOP_ROW+PROPERTY_HEIGHT+PROPERTY_WIDTH*1, PROPERTY_HEIGHT, PROPERTY_WIDTH}},
   { "New York Ave",          200, {16,  80, 220, 600, 800, 1000}, 100, 100, 0, -1, 0, {PROPERTY_LEFT_COLUMN, PROPERTY_TOP_ROW+PROPERTY_HEIGHT, PROPERTY_HEIGHT, PROPERTY_WIDTH}},

   { "Free Parking",          0,   { 0,   0,   0,   0,    0,    0},   0,   0, 0, -1, 0, {PROPERTY_LEFT_COLUMN, PROPERTY_TOP_ROW, PROPERTY_HEIGHT, PROPERTY_HEIGHT}},
   { "Kentucky Ave",          220, {18,  90, 250, 700,  875, 1050}, 110, 150, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT, PROPERTY_TOP_ROW, PROPERTY_WIDTH, PROPERTY_HEIGHT}},
   { "Chance",                0,   { 0,   0,   0,   0,    0,    0},   0,   0, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*1, PROPERTY_TOP_ROW, PROPERTY_WIDTH, PROPERTY_HEIGHT}},
   { "Indiana",               220, {18,  90, 250, 700,  875, 1050}, 110, 150, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*2, PROPERTY_TOP_ROW, PROPERTY_WIDTH, PROPERTY_HEIGHT}},
   { "Illinois",              240, {20, 100, 300, 750,  925, 1100}, 120, 150, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*3, PROPERTY_TOP_ROW, PROPERTY_WIDTH, PROPERTY_HEIGHT}},
   { "B & O Railroad",        200, { 0,   0,   0,   0,    0,    0}, 100,   0, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*4, PROPERTY_TOP_ROW, PROPERTY_WIDTH, PROPERTY_HEIGHT}},
   { "Atlantic Ave",          260, {22, 110, 330, 800,  975, 1150}, 130, 150, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*5, PROPERTY_TOP_ROW, PROPERTY_WIDTH, PROPERTY_HEIGHT}},
   { "Ventnor Ave",           260, {22, 110, 330, 800,  975, 1150}, 130, 150, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*6, PROPERTY_TOP_ROW, PROPERTY_WIDTH, PROPERTY_HEIGHT}},
   { "Water Works",           150, { 0,   0,   0,   0,    0,    0},  75,   0, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*7, PROPERTY_TOP_ROW, PROPERTY_WIDTH, PROPERTY_HEIGHT}},
   { "Marvin Gardens",        280, {24, 120, 360, 850, 1025, 1200}, 140, 150, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*8, PROPERTY_TOP_ROW, PROPERTY_WIDTH, PROPERTY_HEIGHT}},

   { "Go To Jail",            0,   { 0,   0,   0,    0,    0,    0},   0,   0, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*9, PROPERTY_TOP_ROW, PROPERTY_HEIGHT, PROPERTY_HEIGHT}},
   { "Pacific Ave",           300, {26, 130, 390,  900, 1100, 1275}, 150, 200, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*9, PROPERTY_TOP_ROW+PROPERTY_HEIGHT, PROPERTY_HEIGHT, PROPERTY_WIDTH}},
   { "North Carolina Ave",    300, {26, 130, 390,  900, 1100, 1275}, 150, 200, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*9, PROPERTY_TOP_ROW+PROPERTY_HEIGHT+PROPERTY_WIDTH*1, PROPERTY_HEIGHT, PROPERTY_WIDTH}},
   { "Community Chest",       0,   { 0,   0,   0,    0,    0,    0},   0,   0, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*9, PROPERTY_TOP_ROW+PROPERTY_HEIGHT+PROPERTY_WIDTH*2, PROPERTY_HEIGHT, PROPERTY_WIDTH}},
   { "Pennsylvania Ave",      320, {28, 150, 450, 1000, 1200, 1400}, 160, 200, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*9, PROPERTY_TOP_ROW+PROPERTY_HEIGHT+PROPERTY_WIDTH*3, PROPERTY_HEIGHT, PROPERTY_WIDTH}},
   { "Short Line Railroad",   200, { 0,   0,   0,    0,    0,    0}, 100,   0, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*9, PROPERTY_TOP_ROW+PROPERTY_HEIGHT+PROPERTY_WIDTH*4, PROPERTY_HEIGHT, PROPERTY_WIDTH}},
   { "Chance",                0,   { 0,   0,   0,    0,    0,    0},   0,   0, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*9, PROPERTY_TOP_ROW+PROPERTY_HEIGHT+PROPERTY_WIDTH*5, PROPERTY_HEIGHT, PROPERTY_WIDTH}},
   { "Park Place",            350, {35, 175, 500, 1100, 1300, 1500}, 175, 200, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*9, PROPERTY_TOP_ROW+PROPERTY_HEIGHT+PROPERTY_WIDTH*6, PROPERTY_HEIGHT, PROPERTY_WIDTH}},
   { "Luxury Tax",            0,   { 0,   0,   0,    0,    0,    0},   0,   0, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*9, PROPERTY_TOP_ROW+PROPERTY_HEIGHT+PROPERTY_WIDTH*7, PROPERTY_HEIGHT, PROPERTY_WIDTH}},
   { "Boardwalk",             400, {50, 200, 600, 1400, 1700, 2000}, 200, 200, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*9, PROPERTY_TOP_ROW+PROPERTY_HEIGHT+PROPERTY_WIDTH*8, PROPERTY_HEIGHT, PROPERTY_WIDTH}},

   { "In Jail",               0,   { 0,   0,   0,    0,    0,    0},   0,   0, 0, -1, 0, {PROPERTY_LEFT_COLUMN, PROPERTY_BOTTOM_ROW, PROPERTY_HEIGHT, PROPERTY_HEIGHT}}
};

static void OnMouseClick(Ifel* i);

void StartGame(void)
{
   // assumes all players have been added already

   fprintf(stderr, "New game\n");

   srand(time(NULL));
   currentPlayer = rand() % players;
   fprintf(stderr, "player %s starts\n", player[currentPlayer].name);

   SetCurrentPlayerStatusArea(&player[currentPlayer]);
   EnableStatusArea();

   if (!messageBox)
   {
      messageBox = CreateMessageBox(ID_MSGBOX_ROLL, "Click to roll the dice");
      messageBox->el.OnMouseClick = OnMouseClick;
   }
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

void PlayerOutOfBusiness(int playerID)
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

void DoneTurn(void)
{
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

   // reset the doubles counter
   player[currentPlayer].doublesCount = 0;

   // otherwise, go to the next active player's turn
   currentPlayer = (currentPlayer + 1) % players;
   while (!player[currentPlayer].active)
      currentPlayer = (currentPlayer + 1) % players;
   SetCurrentPlayerStatusArea(&player[currentPlayer]);

   fprintf(stderr, "%s's turn\n", player[currentPlayer].name);
   if (!messageBox)
   {
      messageBox = CreateMessageBox(ID_MSGBOX_ROLL, "Click to roll the dice");
      messageBox->el.OnMouseClick = OnMouseClick;
   }
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
   printf("%s landed on %s\n", player[currentPlayer].name, board[endSquare].name);

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
         if (player[currentPlayer].money < board[endSquare].value)
         {
            printf("auctions not implemented yet\n");
            // Auction(endSquare); 
         }

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
            // Auction(endSquare); 
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
               rent = 25;
               for (int i=1; i < rrCount; i++)
                  rent *= 2;
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
            printf("%s owes %i in rent\n", player[currentPlayer].name, rent);
   
            PayMoney(currentPlayer, board[endSquare].owner, rent);
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

static void OnMouseClick(Ifel* i)
{
   //fprintf(stderr, "OnMouseClick in %i\n", i->id);
   switch(i->id)
   {
   case ID_BTN_OPTIONS:
      //fprintf(stderr, "Click in options button\n");
      //ShowOptionsMenu();
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
      if (ModalMessageBox(ID_MSGBOX_NEW_GAME, "Start a new game?") == MB_YES)
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
   button[0] = CreateButton(ID_BTN_OPTIONS, NULL, 0, image[ID_IMG_BOARD]->el.loc.y + image[ID_IMG_BOARD]->el.loc.h,
      "graphics/btn_options_1.png",
      "graphics/btn_options_2.png",
      "graphics/btn_options_3.png");
   if (!button[0])
   {
      fprintf(stderr, "Can't create options button\n");
      return 2;
   }
   button[0]->el.OnMouseClick = OnMouseClick;

   // create status area
   SDL_Rect statusRect;
   statusRect.x = image[ID_IMG_BOARD]->el.loc.x + image[ID_IMG_BOARD]->el.loc.w;
   statusRect.y = 0;
   statusRect.w = 275;
   statusRect.h = 600;
   if (CreateStatusArea(&statusRect) != 0)
   {
      fprintf(stderr, "Can't create status area\n");
      return 3;
   }

   // start from a clean slate
   GameOver();

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


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
#include "board.h"

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
static void OnMouseClick(Ifel* i);

/* The board keeps most of the game state. The UI updates from the board
   to show the users what's going on. */
static CClonopolyBoard board;

void StartGame(void)
{
   // assumes all players have been added already

   fprintf(stderr, "New game\n");

   srand(time(NULL));
   currentPlayer = rand() % players;
   fprintf(stderr, "player %s starts\n", player[currentPlayer].name);

   board.Reset();
   
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

   player[id].name = (char*)malloc(strlen(name) + 1);
   strcpy(player[id].name, name);

   fprintf(stderr, "set player %i name to %s\n", id, player[id].name);
}

void SetPlayerSquare(int id, int square)
{
   player[id].location = square;
   image[player[id].token]->el.loc.w = board.Property(square).m_loc.w;
   image[player[id].token]->el.loc.h = board.Property(square).m_loc.h;
   image[player[id].token]->el.loc.x = board.Property(square).m_loc.x + ((image[player[id].token]->el.loc.w - image[player[id].token]->el.loc.w) >> 1); // without the brackets on the >> this doesn't calculate right
   image[player[id].token]->el.loc.y = board.Property(square).m_loc.y + image[player[id].token]->el.loc.h - image[player[id].token]->el.loc.h;
}

int GetPlayerSquare(int id)
{
   return player[id].location;
}

int AddPlayer(int token, const char* name)
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
   //printf("%s landed on %s\n", player[currentPlayer].name, board[endSquare].name);

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
      if (board.Property(endSquare).m_owner == -1)
      {
         char msg[100];

         // if you have enough money, do you want to buy the property?
         if (player[currentPlayer].money < board.Property(endSquare).m_value) 
         {
            printf("You don't have enough money!\n");
            goto nomoney;
         }

         sprintf(msg, "Would you like to buy %s? It costs $%i", board.Property(endSquare).m_name.c_str(),
            board.Property(endSquare).m_value);
         if (ModalMessageBox(ID_MSGBOX_BUY_PROPERTY, msg) == MB_YES)
         {
            player[currentPlayer].money -= board.Property(endSquare).m_value; 
            board.Property(endSquare).m_owner = currentPlayer;
            //printf("%s bought %s\n", player[currentPlayer].name, board[endSquare].name);

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
         int owner = board.Property(endSquare).m_owner;
         if (owner == currentPlayer)
         {
            printf("owned by you!\n");
         }
         else
         {
            printf("owned by %s\n", player[owner].name);
            if (board.Property(endSquare).m_mortgaged)
            {
               printf("mortgaged\n");
            }
            else
            {
               if (IS_RAILROAD(endSquare))
               {
                  int rrCount = 0;

                  // count how many railroads are owned
                  if (board.Property(SQUARE_READING_RR).m_owner == owner)
                     rrCount++;
                  if (board.Property(SQUARE_PENNSYLVANIA_RR).m_owner == owner)
                     rrCount++;
                  if (board.Property(SQUARE_BO_RR).m_owner == owner)
                     rrCount++;
                  if (board.Property(SQUARE_SHORT_LINE_RR).m_owner == owner)
                     rrCount++;

                  // double rent for each additional railroad
                  int houses = board.Property(endSquare).m_numHouses;
                  rent = board.Property(endSquare).m_rent[houses] << (rrCount-1);
               }
               else if ( endSquare == SQUARE_ELECTRIC_CO
                     || endSquare == SQUARE_WATER_WORKS)
               {
                  int owner = board.Property(endSquare).m_owner;
                  // one utility is x4, both utilities is x10
                  if (board.Property(SQUARE_ELECTRIC_CO).m_owner == owner
                     && board.Property(SQUARE_WATER_WORKS).m_owner == owner)
                     rent = (dice[0] + dice[1]) * 10;
                  else
                     rent = (dice[0] + dice[1]) * 4;
               }
               else
               {
                  int houses = board.Property(endSquare).m_numHouses;
                  rent = board.Property(endSquare).m_rent[houses];
               }
               printf("%s owes %i in rent\n", player[currentPlayer].name, rent);
   
               PayMoney(currentPlayer, board.Property(endSquare).m_owner, rent);
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
         fprintf(stderr, "Can't load image %s\n", filename);

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


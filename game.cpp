#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "game.h"
#include "id.h"

/*
static void OnKeyPressed(Ifel* el, int key)
{
   switch(key)
   {
   case 'y':
      break;

   case 'n':
      break;
   }
}
*/

static void OnMouseClick(Ifel* i)
{
   //fprintf(stderr, "OnMouseClick in %i\n", i->id);
   switch(i->id)
   {
   case ID_BTN_OPTIONS:
      break;

   case ID_MSGBOX_ROLL:
   case ID_BTN_ROLL:
      break;
   }
}

CGame::CGame(CGameUI& ui) :   m_ui(ui),
                              gameOver(true),
                              players(0),
                              doublesCount(0),
                              currentPlayer(0)
{
}

bool CGame::IsCurrentPlayerInJail()
{
   return player[currentPlayer].inJail;
}

void CGame::NewGame()
{
   fprintf(stderr, "New game\n");

   // reset ownership of all properties
   for (int i=0; i < NUM_PROPERTIES; i++)
      owner[i] = -1;

   AddPlayer(ID_IMG_SHOE, "Joel");
   m_ui.SetTokenActive(ID_IMG_SHOE);   
   AddPlayer(ID_IMG_DOG, "Merav");
   m_ui.SetTokenActive(ID_IMG_SHOE);   

   srand(time(NULL));
   currentPlayer = rand() % players;
   gameOver = false;

   fprintf(stderr, "player %s starts\n", GetCurrentPlayer()->name);
   StartTurn();
}

Player* CGame::GetCurrentPlayer()
{
   return &player[currentPlayer];
}

void CGame::Over()
{
   gameOver = true;
   m_ui.DisableStatusArea();
}

void CGame::PlayerOutOfBusiness(int id)
{
   player[id].active = 0;
   players--;

   if (players == 1)
      Over();
}

unsigned int CGame::ActivePlayers()
{
   return players;
}

int CGame::HasAssets(int playerID)
{
   // do you have anything to mortgage?

   // do you have any houses you can sell?

   return 0;
}

void CGame::PayMoney(int from, int to, int amount)
{
   if (from != PLAYER_BANK)
   {
      //fprintf(stderr, "current: %i amount: %i\n", player[from].money, amount);
      player[from].money -= amount;

      // check to see if he is out of money
      while (player[from].money < 0 && HasAssets(from))
      {
         //fprintf(stderr, "Out of money!\n");
         m_ui.LiquidateAssets();
      }

      if (player[from].money < 0)
         PlayerOutOfBusiness(from);
   }

   if (to != PLAYER_BANK)
      player[to].money += amount;
}

void CGame::FinishTurn()
{
   // reset the doubles counter
   doublesCount = 0;

   // go to the next active player's turn
   currentPlayer = (currentPlayer + 1) % players;
   while (!player[currentPlayer].active)
      currentPlayer = (currentPlayer + 1) % players;
}

void CGame::SetPlayerName(unsigned int id, const char* name)
{
   if (player[id].name)
   {
      free(player[id].name);
      player[id].name = 0;
   }

   //player[id].name = (char*)malloc(strlen(name) + 1);
   //strcpy(player[id].name, name);
   player[id].name = strdup(name);

   //fprintf(stderr, "set player %i name to %s\n", id, player[id].name);
}

bool CGame::AddPlayer(int token, const char* name)
{
   if (players >= MAX_PLAYERS)   
   {
      //fprintf(stderr, "Maximum players %i is reached\n", MAX_PLAYERS);
      return false;
   }

   //fprintf(stderr, "Adding player %s\n", name);
   player[players].token = token;
   player[players].active = 1;
   player[players].inJail = 0;
   player[players].location = 0;
   player[players].money = STARTING_CASH;
   SetPlayerName(players, name);
   player[players].index = players;
   players++;


   return true;
}

CGame::~CGame()
{
   for (int i=0; i < MAX_PLAYERS; i++)
   {
      if (player[i].name)
      {
         free(player[i].name);
         player[i].name = 0;
      }
   }
}

void CGame::DoublesCountInc()
{
   doublesCount++;
}

void CGame::DoublesCountReset()
{
   doublesCount = 0;
}

unsigned int CGame::DoublesCount()
{
   return doublesCount;
}

void CGame::SetCurrentPlayerSquare(int square)
{
   player[currentPlayer].location = square;
}

int CGame::GetCurrentPlayerSquare()
{
   return player[currentPlayer].location;
}

void CGame::MovePlayer(int square)
{
   int startSquare = GetCurrentPlayerSquare();

   if (square == ACCORDING_TO_DICE)
   {
      // first see if the player is in jail
      if (IsCurrentPlayerInJail())
      {
         player[currentPlayer].turnsLeftInJail--;
         //printf("%s is in jail - %i rolls left\n", player[currentPlayer].name, player[currentPlayer].turnsLeftInJail);
         doublesCount = 0; // can't accumulate doubles in jail

         // if you got doubles, you're out!
         if (dice[0] == dice[1])
         {
            //printf("%s is out of jail (rolled doubles)\n", player[currentPlayer].name);
            player[currentPlayer].inJail = 0;
            player[currentPlayer].turnsLeftInJail = 0;
            doublesCount++;
            SetCurrentPlayerSquare(SQUARE_JUST_VISITING);
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
               //printf("%s has to pay to get out of jail\n", player[currentPlayer].name);
               PayMoney(currentPlayer, PLAYER_BANK, 50);
               player[currentPlayer].inJail = 0;
               player[currentPlayer].turnsLeftInJail = 0;
               SetPlayerSquare(currentPlayer, SQUARE_JUST_VISITING);
               startSquare = GetPlayerSquare(currentPlayer);
               //printf("%s is out of jail (paid $50)\n", player[currentPlayer].name);
            }
         }
      }
      else
      {
         // check for doubles
         if (dice[0] == dice[1])
            DoublesCountInc(); //doublesCount++;
         else
            DoublesCountReset();// player[currentPlayer].doublesCount = 0;
      }

      // did you roll too many doubles in a row?
      if (DoublesCount() == 3)
      {
         //printf("Too many doubles - going to jail\n");
         SetCurrentPlayerSquare(SQUARE_IN_JAIL);
      }
      else
      {
         SetCurrentPlayerSquare((startSquare + dice[0] + dice[1]) % NUM_PROPERTIES);
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
      //printf("Passed GO - got $200\n");
      PayMoney(PLAYER_BANK, currentPlayer, 200);
   }

   // check the special squares first
   int handled = 0;
   switch (endSquare)
   {
   case SQUARE_GO:
      //printf("Got $200\n");
      PayMoney(PLAYER_BANK, currentPlayer, 200);
      handled = 1;
      break;

   case SQUARE_INCOME_TAX:
      //printf("Pay income tax\n");
      // give option to calculate 10% of assets - JKN
      PayMoney(currentPlayer, PLAYER_BANK, 200);
      handled = 1;
      break;

   case SQUARE_JUST_VISITING:
   case SQUARE_FREE_PARKING:
      //printf("Nothing happens - this time\n");
      handled = 1;
      break;
      
   case SQUARE_LUXURY_TAX:
      //printf("Pay luxury tax\n");
      PayMoney(currentPlayer, PLAYER_BANK, 75);
      handled = 1;
      break;

   case SQUARE_GO_TO_JAIL:
      //printf("That's it - off to jail\n");
      SetPlayerSquare(currentPlayer, SQUARE_IN_JAIL);
   case SQUARE_IN_JAIL:
      //printf("%s is in jail\n", player[currentPlayer].name);
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
         // if you have enough money, do you want to buy the property?
         if (player[currentPlayer].money < board.Property(endSquare).m_value) 
         {
            //printf("You don't have enough money!\n");
            //goto nomoney;
         }

         if (m_ui.BuyProperty(board.Property(endSquare)))
         {
            player[currentPlayer].money -= board.Property(endSquare).m_value; 
            board.Property(endSquare).m_owner = currentPlayer;
            //printf("%s bought %s\n", player[currentPlayer].name, board[endSquare].name);

            // check to see if the player owns a set
         }
         else
         {
//nomoney:
            // no money, or no will
            //printf("auctions not implemented yet\n");
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
            //printf("owned by you!\n");
         }
         else
         {
            //printf("owned by %s\n", player[owner].name);
            if (board.Property(endSquare).m_mortgaged)
            {
               //printf("mortgaged\n");
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
               //printf("%s owes %i in rent\n", player[currentPlayer].name, rent);
   
               PayMoney(currentPlayer, board.Property(endSquare).m_owner, rent);
            }
         }
      }
   }

   // if you got doubles and you are not in jail, roll again
   if (dice[0] == dice[1] && !player[currentPlayer].inJail)
   {
      m_ui.Doubles(OnMouseClick);
   }
   else
   {
      FinishTurn();
   }
}

void CGame::RollDice(void)
{
   dice[0] = (rand() % 6) + 1;
   dice[1] = (rand() % 6) + 1;
   //fprintf(stderr, "dice: %i %i\n", dice[0], dice[1]);
}

void CGame::SetPlayerSquare(int id, int square)
{
   player[id].location = square;
   m_ui.SetPlayerSquare(player[id].token, board.Property(square).m_loc);
}

int CGame::GetPlayerSquare(int id)
{
   return player[id].location;
}

int CGame::StartTurn()
{
   m_ui.SetCurrentPlayerStatus(player[currentPlayer]);
   return 0;
}


#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "game.h"
#include "id.h"

#ifdef DEBUG
#define DBG_PRINT fprintf
#else
#define DBG_PRINT(...)
#endif // DEBUG

chance_card chance_cards[] =
{
	{ "Go back 3 spaces", 																	ACTION_MOVE_RELATIVE, -3 },
	{ "Take a walk on the boardwalk. Advance token to Boardwalk", 				ACTION_MOVE_TO_SQUARE, SQUARE_BOARDWALK },
	{ "You have been elected chairman of the board. Pay each player $50",	ACTION_PAY_ALL_PLAYERS, 50 },
	{ "Get out of jail free", 																ACTION_GOOJF, 0 },
	{ "Go directly to jail. Do not pass go, do not collect $200",				ACTION_MOVE_TO_SQUARE, SQUARE_IN_JAIL },
	{ "You building and loan matures. Collect $150", 								ACTION_GET_PAID, 150 },
	{ "Pay poor tax of $15",																ACTION_PAY, 15 },
	{ "Take a ride on the Reading. If you pass go collect $200",				ACTION_MOVE_TO_SQUARE, SQUARE_READING_RR },
	{ "Advance token to nearest utility",												ACTION_MOVE_TO_UTILITY, 0 },
};

chance_card community_chest_cards[NUM_CC_CARDS] =
{
	{ "Bank error in your favor. Collect $200",										ACTION_GET_PAID, 200 },
	{ "Advance to go. Collect $200", 													ACTION_MOVE_TO_SQUARE, SQUARE_GO },
	{ "Get out of jail free", 																ACTION_GOOJF, 0 },
	{ "Go directly to jail. Do not pass go, do not collect $200",				ACTION_MOVE_TO_SQUARE, SQUARE_IN_JAIL },
	{ "Pay hospital $100",																	ACTION_PAY, 100 },
	{ "Life insurance matures. Collect $100",											ACTION_GET_PAID, 100 },
	{ "You are assessed for street repairs", 											ACTION_STREET_REPAIRS_CC, 0 },
	{ "Xmas fund matures. Collect $100",												ACTION_GET_PAID, 100 },
};

extern property m_board[NUM_PROPERTIES];
CGame::CGame(CGameUI* ui) :   m_ui(*ui),
                              gameOver(true),
                              m_players(0),
                              doublesCount(0),
                              m_currentPlayer(0),
										rolledDice(false)
{
	ui->Init(this);
}


bool CGame::Initialize()
{
	m_ui.SplashScreen();

	return true;
}

bool CGame::IsCurrentPlayerInJail()
{
   return m_player[m_currentPlayer].inJail;
}

int CGame::Play()
{
	// make sure at least 2 players are defined
	if (m_players < 2)
		return ERR_NO_PLAYERS;

	while (!gameOver)
	{
		bool done = false;
		StartTurn();
		while (!done)
		{
			SetPlayerOptions();
			int action = GetAction();
			switch(action)
			{
			case ACTION_ROLL:
				if (!rolledDice)
				{
					RollDice();
					rolledDice = true;
					MovePlayer(ACCORDING_TO_DICE);
				}
				break;

			case ACTION_BUY:
				break;

			case ACTION_MORTGAGE:
				Mortgage(m_ui.Mortgage(m_player[m_currentPlayer]));
				break;

			case ACTION_TRADE:
				printf("Trading is not implemented yet\n");
				break;
			case ACTION_DONE:
				done = true;
				break;
			}
		}
		FinishTurn();
	}

	return OK;
}

void CGame::NewGame()
{
   DBG_PRINT(stderr, "New game\n");

   // reset ownership of all properties
   for (int i=0; i < NUM_PROPERTIES; i++)
      owner[i] = -1;

	// tell UI to give us some players!
   DBG_PRINT(stderr, "Getting players\n");
	m_ui.GetPlayers();

   srand(time(NULL));
   m_currentPlayer = rand() % m_players;
   gameOver = false;
}

Player* CGame::GetCurrentPlayer()
{
   return &m_player[m_currentPlayer];
}

void CGame::Over()
{
   gameOver = true;
   m_ui.DisableStatusArea();
}

void CGame::PlayerOutOfBusiness(int id)
{
   m_player[id].active = 0;
   m_players--;

   if (m_players == 1)
      Over();
}

unsigned int CGame::ActivePlayers()
{
   return m_players;
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
      //DBG_PRINT(stderr, "current: %i amount: %i\n", player[from].money, amount);
      m_player[from].money -= amount;

      // check to see if he is out of money
      while (m_player[from].money < 0 && HasAssets(from))
      {
         DBG_PRINT(stderr, "Out of money!\n");
         m_ui.LiquidateAssets();
      }

      if (m_player[from].money < 0)
         PlayerOutOfBusiness(from);
   }

   if (to != PLAYER_BANK)
      m_player[to].money += amount;

	m_ui.SetCurrentPlayerStatus(m_player[m_currentPlayer]);
}

void CGame::FinishTurn()
{
	//DBG_PRINT(stderr, "Finishing turn for %s\n", player[m_currentPlayer].name);

   // reset the doubles counter
   doublesCount = 0;

   // go to the next active player's turn
   m_currentPlayer = (m_currentPlayer + 1) % m_players;
   while (!m_player[m_currentPlayer].active)
      m_currentPlayer = (m_currentPlayer + 1) % m_players;
}

void CGame::SetPlayerName(unsigned int id, const char* name)
{
   if (m_player[id].name)
   {
      free(m_player[id].name);
      m_player[id].name = 0;
   }

   m_player[id].name = strdup(name);

   //DBG_PRINT(stderr, "set player %i name to %s\n", id, player[id].name);
}

bool CGame::AddPlayer(int token, const char* name)
{
   if (m_players >= MAX_PLAYERS)   
   {
      DBG_PRINT(stderr, "Maximum players %i is reached\n", MAX_PLAYERS);
      return false;
   }

   DBG_PRINT(stderr, "Adding player %s\n", name);
   m_player[m_players].token = token;
   m_player[m_players].active = 1;
   m_player[m_players].inJail = 0;
   m_player[m_players].location = 0;
   m_player[m_players].money = STARTING_CASH;
   SetPlayerName(m_players, name);
   m_player[m_players].index = m_players;
   m_players++;


   return true;
}

CGame::~CGame()
{
   for (int i=0; i < MAX_PLAYERS; i++)
   {
      if (m_player[i].name)
      {
         free(m_player[i].name);
         m_player[i].name = 0;
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
   m_player[m_currentPlayer].location = square;
	m_ui.SetPlayerSquare(m_currentPlayer, square);
}

int CGame::GetCurrentPlayerSquare()
{
   return m_player[m_currentPlayer].location;
}

void CGame::MovePlayer(int square)
{
   int startSquare = GetCurrentPlayerSquare();
	int endSquare;

   switch (square)
   {
	case ACCORDING_TO_DICE:
      // first see if the player is in jail
      if (IsCurrentPlayerInJail())
      {
         m_player[m_currentPlayer].turnsLeftInJail--;
         //printf("%s is in jail - %i rolls left\n", player[m_currentPlayer].name, player[m_currentPlayer].turnsLeftInJail);
         doublesCount = 0; // can't accumulate doubles in jail

         // if you got doubles, you're out!
         if (dice[0] == dice[1])
         {
            //printf("%s is out of jail (rolled doubles)\n", player[m_currentPlayer].name);
            m_player[m_currentPlayer].inJail = 0;
            m_player[m_currentPlayer].turnsLeftInJail = 0;
            doublesCount++;
            SetCurrentPlayerSquare(SQUARE_JUST_VISITING);
            startSquare = GetPlayerSquare(m_currentPlayer);
            // from here, join the normal flow as if we had been in 'just visiting'
         }
         else
         {
            if (m_player[m_currentPlayer].turnsLeftInJail == 0)
            { 
               // check if the player has a get out of jail free card
               // ask if the player wants to use a get out of jail free card
               // no? then he has to pay
               //printf("%s has to pay to get out of jail\n", player[m_currentPlayer].name);
               PayMoney(m_currentPlayer, PLAYER_BANK, 50);
               m_player[m_currentPlayer].inJail = 0;
               m_player[m_currentPlayer].turnsLeftInJail = 0;
               SetPlayerSquare(m_currentPlayer, SQUARE_JUST_VISITING);
               startSquare = GetPlayerSquare(m_currentPlayer);
               //printf("%s is out of jail (paid $50)\n", player[m_currentPlayer].name);
            }
         }
      }
      else
      {
         // check for doubles
         if (dice[0] == dice[1])
            DoublesCountInc();
         else
            DoublesCountReset();
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
		break;

	case MOVE_TO_NEXT_UTIL:
		if (startSquare < SQUARE_ELECTRIC_CO)
			endSquare = SQUARE_ELECTRIC_CO;
		else if (startSquare < SQUARE_WATER_WORKS)
			endSquare = SQUARE_WATER_WORKS;
		else
			endSquare = SQUARE_ELECTRIC_CO;
		break;

	default:
      // jump to a specific square, like 'jail', or something from a 'chance' or 'community chest'
      SetPlayerSquare(m_currentPlayer, square);
   }

   endSquare = GetPlayerSquare(m_currentPlayer);
   //printf("%s landed on %s\n", player[m_currentPlayer].name, board[endSquare].name);

   // did you pass go? (and not go to jail)
   if ((endSquare != SQUARE_IN_JAIL) && (endSquare < startSquare))
   {
      DBG_PRINT(stderr, "Passed Go - got $200\n");
      PayMoney(PLAYER_BANK, m_currentPlayer, 200);
   }

   // check the special squares first
   int handled = 0;
	int ret = 0;
   switch (endSquare)
   {
   case SQUARE_GO:
      DBG_PRINT(stderr, "Landed on Go - Got $200\n");
      PayMoney(PLAYER_BANK, m_currentPlayer, 200);
      handled = 1;
      break;

   case SQUARE_INCOME_TAX:
      DBG_PRINT(stderr, "Pay income tax\n");
      // give option to calculate 10% of assets - JKN
		ret = m_ui.IncomeTax();
		if (ret == INCOME_TAX_10_PERCENT)
      	PayMoney(m_currentPlayer, PLAYER_BANK, m_player[m_currentPlayer].money * 0.10);
		else
      	PayMoney(m_currentPlayer, PLAYER_BANK, 200);
      handled = 1;
      break;

   case SQUARE_JUST_VISITING:
   case SQUARE_FREE_PARKING:
      //printf("Nothing happens - this time\n");
      handled = 1;
      break;
      
   case SQUARE_LUXURY_TAX:
      DBG_PRINT(stderr, "Pay luxury tax\n");
      PayMoney(m_currentPlayer, PLAYER_BANK, 75);
      handled = 1;
      break;

   case SQUARE_GO_TO_JAIL:
      DBG_PRINT(stderr, "That's it - off to jail\n");
      SetPlayerSquare(m_currentPlayer, SQUARE_IN_JAIL);
   case SQUARE_IN_JAIL:
      DBG_PRINT(stderr, "%s is in jail\n", m_player[m_currentPlayer].name);
      m_player[m_currentPlayer].inJail = 1;
      m_player[m_currentPlayer].turnsLeftInJail = 3;
      handled = 1;
      break;

	case SQUARE_ELECTRIC_CO:
	case SQUARE_WATER_WORKS:
		// if one is owned, dice x4
		//if both are owned, dice x10
		// if sent by a card, x10
		//if (square == MOVE_TO_NEXT_UTIL) // we were sent by a card
      DBG_PRINT(stderr, "Pay utilities\n");
      PayMoney(m_currentPlayer, m_board[endSquare].owner, 11);
      handled = 1;
		break;

   // community chest
   case 2:
   case 17:
   case 33:
      //printf("Pick a CC card\n");
		m_ui.CommunityChestCard(CommunityChestCard());
      handled = 1;
      break;

   case 7:
   case 22:
   case 36:
      //printf("Pick a Chance card\n");
		m_ui.ChanceCard(ChanceCard());
      handled = 1;
      break;
   }

   if (!handled)
   {
      // otherwise, it is a property. Is it unowned?
      if (m_board[endSquare].owner == -1)
      {
         // if you have enough money, do you want to buy the property?
         if (m_player[m_currentPlayer].money < m_board[endSquare].value) 
         {
            printf("You don't have enough money!\n");
            goto nomoney;
         }

         if (m_ui.BuyProperty(m_board[endSquare]))
         {
				BuyProperty(m_currentPlayer, m_board[endSquare].value, endSquare);
				m_ui.SetCurrentPlayerStatus(m_player[m_currentPlayer]);
         }
         else
         {
nomoney:
            // no money, or no will
            Auction(endSquare); 
         }
      }
      else
      {
         // property is already owned
         int rent;
         int owner = m_board[endSquare].owner;
         if (owner == m_currentPlayer)
         {
            DBG_PRINT(stderr, "%s is owned by you!\n", m_board[endSquare].name.c_str());
         }
         else
         {
            DBG_PRINT(stderr, "%s is owned by %s\n", m_board[endSquare].name.c_str(), m_player[owner].name);
            if (m_board[endSquare].mortgaged)
            {
            	DBG_PRINT(stderr, "%s is mortgaged :-)\n", m_board[endSquare].name.c_str());
            }
            else
            {
               if (IS_RAILROAD(endSquare))
               {
                  int rrCount = 0;

                  // count how many railroads are owned
                  if (m_board[SQUARE_READING_RR].owner == owner)
                     rrCount++;
                  if (m_board[SQUARE_PENNSYLVANIA_RR].owner == owner)
                     rrCount++;
                  if (m_board[SQUARE_BO_RR].owner == owner)
                     rrCount++;
                  if (m_board[SQUARE_SHORT_LINE_RR].owner == owner)
                     rrCount++;

                  // double rent for each additional railroad
                  int houses = m_board[endSquare].numHouses;
                  rent = m_board[endSquare].rent[houses] << (rrCount-1);
               }
               else if ( endSquare == SQUARE_ELECTRIC_CO
                     || endSquare == SQUARE_WATER_WORKS)
               {
                  int owner = m_board[endSquare].owner;
                  // one utility is x4, both utilities is x10
                  if (m_board[SQUARE_ELECTRIC_CO].owner == owner
                     && m_board[SQUARE_WATER_WORKS].owner == owner)
                     rent = (dice[0] + dice[1]) * 10;
                  else
                     rent = (dice[0] + dice[1]) * 4;
               }
               else
               {
                  int houses = m_board[endSquare].numHouses;
                  rent = m_board[endSquare].rent[houses];
               }
               DBG_PRINT(stderr, "%s owes %i in rent\n", m_player[m_currentPlayer].name, rent);
   
               PayMoney(m_currentPlayer, m_board[endSquare].owner, rent);
            }
         }
      }
   }

   // if you got doubles and you are not in jail, roll again
   if (dice[0] == dice[1] && !m_player[m_currentPlayer].inJail)
   {
      m_ui.Doubles();
   }
}

void CGame::RollDice(void)
{
   dice[0] = (rand() % 6) + 1;
   dice[1] = (rand() % 6) + 1;
   //DBG_PRINT(stderr, "dice: %i %i\n", dice[0], dice[1]);

	m_ui.RollDice();
}

void CGame::SetPlayerSquare(int id, int square)
{
   m_player[id].location = square;
   m_ui.SetPlayerSquare(m_player[id].token, square);
}

int CGame::GetPlayerSquare(int id)
{
   return m_player[id].location;
}

int CGame::StartTurn()
{
	rolledDice = false;
   m_ui.SetCurrentPlayerStatus(m_player[m_currentPlayer]);
   return 0;
}

void CGame::SetPlayerOptions()
{
	bool mortgage = m_player[m_currentPlayer].HasProperties();
	bool buy = false;
	bool sell = false;
	bool trade = true;
	m_ui.SetPlayerOptions(!rolledDice, mortgage, buy, sell, trade, rolledDice);
}

void CGame::ReadDice(int* d1, int *d2)
{
	*d1 = dice[0];
	*d2 = dice[1];
}

int CGame::GetAction()
{
	return m_ui.GetAction();
}

const std::string CGame::GetSquareName(int square)
{
	return m_board[square].name;
}

void CGame::Auction(int square)
{
	DBG_PRINT(stderr, "Auctioning %s\n", m_board[square].name.c_str());

	int index;
	int currentBid = 0;
	int leadingPlayer = -1;
	int remainingPlayers = 0;
	int auction_players[MAX_PLAYERS];

	for (index = 0; index < MAX_PLAYERS; index++)
	{
		if (m_player[index].active)
		{
			printf("Setting player %i active for the auction\n", index);
			auction_players[index] = 1;
			remainingPlayers++;
		}
		else
		{
			printf("Setting player %i inactive for the auction\n", index);
			auction_players[index] = 0;
		}
	}

	while (remainingPlayers > 1)
	{
		DBG_PRINT(stderr, "There are %i remaining players in the auction\n", remainingPlayers);
		// ask each player in turn how much they want to bid until everyone gives up (except one)
		for (index = 0; index < MAX_PLAYERS; index++)
		{
			if (auction_players[index] == 1)
			{
				printf("Asking bid from %i\n", index);
				int ret = m_ui.GetAuctionBid(m_player[index], currentBid);
				DBG_PRINT(stderr, "Got a bid of %i\n", ret);
				if (ret == -1) // fold
				{
					auction_players[index] = 0;
					remainingPlayers--;
					if (remainingPlayers == 1)
						break;
				}
				else
				{
					currentBid = ret;
					leadingPlayer = index;
				}
			}
		}
	}

	DBG_PRINT(stderr, "%s won the auction with bid $%i.00\n", m_player[leadingPlayer].name, currentBid);
	BuyProperty(leadingPlayer, currentBid, square);
}

void CGame::BuyProperty(int player, int price, int square)
{
	m_player[player].money -= price; 
	m_board[square].owner = player;
	m_player[player].AddProperty(square);
	DBG_PRINT(stderr, "%s bought %s\n", m_player[player].name, m_board[square].name.c_str());
}

bool CGame::TokenInUse(int token)
{
	for (int index=0; index < MAX_PLAYERS; index++)
	{
		if (m_player[index].token == token)
			return true;
	}

	return false;
}

void CGame::Mortgage(int property)
{
	// mark the property as mortgaged, and give the player more cash
	m_board[property].mortgaged = true;

	m_player[m_board[property].owner].money += m_board[property].value / 2;
}

int CGame::ChanceCard()
{
	int i;
   int card = rand() % sizeof(chance_cards)/sizeof(chance_card);
	switch(chance_cards[card].action)
	{
	case ACTION_MOVE_RELATIVE:
      SetPlayerSquare(m_currentPlayer, m_player[m_currentPlayer].location + chance_cards[card].value);
		MovePlayer(GetCurrentPlayerSquare());
		break;

	case ACTION_MOVE_TO_SQUARE:
		MovePlayer(chance_cards[card].value);
		break;

	case ACTION_PAY_ALL_PLAYERS:
		for (i=0; i < MAX_PLAYERS; i++)
		{
			if (i != m_currentPlayer && m_player[i].active)
				PayMoney(m_currentPlayer, i, chance_cards[card].value);
		}
		break;

	case ACTION_GOOJF:
		break;

	case ACTION_PAY:
		PayMoney(m_currentPlayer, PLAYER_BANK, chance_cards[card].value);
		break;

	case ACTION_GET_PAID:
		PayMoney(PLAYER_BANK, m_currentPlayer, chance_cards[card].value);
		break;

	case ACTION_MOVE_TO_UTILITY:
		MovePlayer(MOVE_TO_NEXT_UTIL);
		break;

	//ACTION_STREET_REPAIRS_CC:
	}

	return card;
}

int CGame::CommunityChestCard()
{
   int card = rand() % sizeof(community_chest_cards)/sizeof(chance_card);

	return card;
}


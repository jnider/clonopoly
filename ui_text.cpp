#include <cstring>
#include "ui_text.h"
#include "game.h"

#ifdef DEBUG
#define DBG_PRINT fprintf
#else
#define DBG_PRINT(...)
#endif // DEBUG

extern property m_board[NUM_PROPERTIES];

static const char tokens[MAX_TOKENS + 1] = "@#^&*!~+";

CTextUI::CTextUI()
{
}

void CTextUI::SplashScreen()
{
	printf("Clonopoly\n");
	printf("Version: %i.%i\n", MAJOR_VERSION, MINOR_VERSION);
}

// Prompt the user to add players. Each player added must call game->AddPlayer()
void CTextUI::GetPlayers()
{
	int token=-1;
	char name[256];

	while (m_game->ActivePlayers() < MAX_PLAYERS)
	{
		memset(name, 0, 256);
		printf("Enter player %i name: ", m_game->ActivePlayers() + 1);
		int strings = scanf("%255[^\n]ms", name);
		getc(stdin); // consume the newline

		// Empty name means we're done
		if (strings == 0)
			break;

		while (1)
		{
			printf("Pick a token for %s\n", name);
			//printf("1: %c\t 2: %c\t3: %c\t4: %c\t5: %c\t6: %c\t7: %c\t8: %c\n",
			//	tokens[0], tokens[1], tokens[2], tokens[3],
			//	tokens[4], tokens[5], tokens[6], tokens[7]);
			for (int i = 0; i < MAX_TOKENS; i++)
			{
				if (!m_game->TokenInUse(i))
					printf("%i: %c\t", i+1, tokens[i]);
			}
			printf("\n");
			int answer = getc(stdin);
			getc(stdin); // consume the newline
			if (answer > '0' && answer < '9')
			{
				token = answer - '1';

				// make sure the token isn't already used
				if (!m_game->TokenInUse(token))
					break;
			}
		}

		m_game->AddPlayer(token, name);
	}
}

void CTextUI::SetTokenActive(int id)
{
}


void CTextUI::Doubles()
{
}


bool CTextUI::BuyProperty(const property& p)
{
	printf("%s is for sale. It costs $%i.00\n", p.name.c_str(), p.value);
	printf("Would you like to buy it?\n");
	while (1)
	{
		int answer = getc(stdin);
		if (answer == 'Y' || answer == 'y')
			return true;
		if (answer == 'N' || answer == 'n')
			return false;
	}
	return false;
}


bool CTextUI::LiquidateAssets()
{
	return false;
}


void CTextUI::SetPlayerSquare(int player, int square)
{
	DBG_PRINT(stderr, "Player: %i Square: %i\n", player, square);
	printf("You are on %s\n", m_game->GetSquareName(square).c_str());
}


void CTextUI::SetCurrentPlayerStatus(Player& p)
{
	printf("\n\nCurrent player: %s\n", p.name);

	// is the player in jail?
	if (p.location == SQUARE_IN_JAIL)
	{
		printf("You are in jail. You have %i turns left\n", p.turnsLeftInJail);
	}
	else
	{
		printf("You are on %s\n", m_board[p.location].name.c_str());
	}

	// properties
	char token = tokens[p.token];
	printf("$50:  ||-|%c|%c||%c|%c|%c||\n",
		m_board[SQUARE_MEDITERRANEAN].owner == p.index ? token : ' ',
		m_board[SQUARE_BALTIC].owner == p.index ? token : ' ',
		m_board[SQUARE_ORIENTAL].owner == p.index ? token : ' ',
		m_board[SQUARE_VERMONT].owner == p.index ? token : ' ',
		m_board[SQUARE_CONNECTICUT].owner == p.index ? token : ' ');
	printf("$100: ||%c|%c|%c||%c|%c|%c||\n",
		m_board[SQUARE_ST_CHARLES].owner == p.index ? token : ' ',
		m_board[SQUARE_STATES].owner == p.index ? token : ' ',
		m_board[SQUARE_VIRGINIA].owner == p.index ? token : ' ',
		m_board[SQUARE_ST_JAMES].owner == p.index ? token : ' ',
		m_board[SQUARE_TENNESSEE].owner == p.index ? token : ' ',
		m_board[SQUARE_NEW_YORK].owner == p.index ? token : ' ');
	printf("$150: ||%c|%c|%c||%c|%c|%c||\n",
		m_board[SQUARE_KENTUCKY].owner == p.index ? token : ' ',
		m_board[SQUARE_INDIANA].owner == p.index ? token : ' ',
		m_board[SQUARE_ILLINOIS].owner == p.index ? token : ' ',
		m_board[SQUARE_ATLANTIC].owner == p.index ? token : ' ',
		m_board[SQUARE_VENTNOR].owner == p.index ? token : ' ',
		m_board[SQUARE_MARVIN].owner == p.index ? token : ' ');
	printf("$200: ||%c|%c|%c||%c|%c|-||\n",
		m_board[SQUARE_PACIFIC].owner == p.index ? token : ' ',
		m_board[SQUARE_N_CAROLINA].owner == p.index ? token : ' ',
		m_board[SQUARE_PENNSYLVANIA].owner == p.index ? token : ' ',
		m_board[SQUARE_PARK_PLACE].owner == p.index ? token : ' ',
		m_board[SQUARE_BOARDWALK].owner == p.index ? token : ' ');
	printf("RR:   ||%c|%c|%c|%c||\n",
		m_board[SQUARE_READING_RR].owner == p.index ? token : ' ',
		m_board[SQUARE_PENNSYLVANIA_RR].owner == p.index ? token : ' ',
		m_board[SQUARE_BO_RR].owner == p.index ? token : ' ',
		m_board[SQUARE_SHORT_LINE_RR].owner == p.index ? token : ' ');
	printf("Utils: ||%c|%c||\n",
		m_board[SQUARE_ELECTRIC_CO].owner == p.index ? token : ' ',
		m_board[SQUARE_WATER_WORKS].owner == p.index ? token : ' ');

	// money
	printf("You have $%i.00\n", p.money);

	// GOOJF cards
}


void CTextUI::ShowOptionsMenu()
{
}


void CTextUI::DeleteStatusArea(void)
{
}


void CTextUI::SetCurrentPlayerStatusArea(Player* p)
{
}


void CTextUI::StatusAreaSetPlayerIconList(int count)
{
}


void CTextUI::DisableStatusArea(void)
{
}


void CTextUI::EnableStatusArea(void)
{
}


void CTextUI::DrawStatusArea()
{
}


/*
void CTextUI::DrawSplash()
{
}
*/


void CTextUI::RollDice()
{
	int d1, d2;
	printf("Rolling dice...\n");
	//sleep(2);
	m_game->ReadDice(&d1, &d2);
	printf("Rolled: %i %i = %i\n", d1, d2, d1 + d2);
}

void CTextUI::SetPlayerOptions(bool roll, bool mortgage, bool buy, bool sell, bool trade, bool done)
{
	printf("Actions\n");

	if (roll)
		printf("1: roll\t");

	if (mortgage)
		printf("2: mortgage\t");

	if (buy)
		printf("3: buy house\t");

	if (sell)
		printf("4: sell house\t");

	if (trade)
		printf("5: trade\t");

	if (done)
		printf("6: finish turn\n");
}

int CTextUI::GetAction()
{
	int action;

	while(1)
	{
		printf("What would you like to do?\n");
		if (scanf("%i", &action) == 0)
			continue;

		switch(action)
		{
		case 1:
			return ACTION_ROLL;

		case 2:
			return ACTION_MORTGAGE;

		case 3:
			return ACTION_BUY;

		case 4:
			return ACTION_SELL;

		case 5:
			return ACTION_TRADE;

		case 6:
			return ACTION_DONE;
		}
	}
}

void CTextUI::CommunityChestCard()
{
	printf("community chest\n");
}

void CTextUI::ChanceCard()
{
	printf("chance\n");
}

int CTextUI::IncomeTax()
{
	int action;

	while(1)
	{
		printf("You have to pay income tax.\n1: 10%%\t2: $200\n");
		if (scanf("%i", &action) == 0)
			continue;

		switch(action)
		{
		case 1:
			return INCOME_TAX_10_PERCENT;

		case 2:
			return INCOME_TAX_200;
		}
	}
}

int CTextUI::GetAuctionBid(Player& p, int currentBid)
{

	int newBid;
	while (1)
	{
		printf("%s's turn to bid.\n", p.name);
		printf("Current bid is %i. How much do you want to bid? (0 to fold)\n", currentBid);
		getc(stdin);
		if (scanf("%i", &newBid) == 0)
		{
			printf("%i is not a valid bid\n", newBid);
			continue;
		}

		if (newBid == 0)
		{
			printf("You fold.\n");
			return -1;
		}

		if (newBid <= currentBid)
		{
			printf("Umm, you have to bid _more_ than the current bid ($%i.00)\n", currentBid);
		}
		else
			break;
	}

	return newBid;
}


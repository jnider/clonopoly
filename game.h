#ifndef _GAME__H
#define _GAME__H

#include "ui_game.h"
#include "board.h"
#include "player.h"

#define MAJOR_VERSION 1
#define MINOR_VERSION 0

#define MAX_PLAYERS 4
#define MAX_TOKENS 8
#define STARTING_CASH 1500
#define ACCORDING_TO_DICE -1
#define MOVE_TO_NEXT_RR -2
#define MOVE_TO_NEXT_UTIL -3
#define PLAYER_BANK  0xFF
#define NUM_CHANCE_CARDS 10
#define NUM_CC_CARDS 10

enum
{
	SQUARE_GO,                     // 0
	SQUARE_MEDITERRANEAN,
	SQUARE_CC_1,
	SQUARE_BALTIC,
	SQUARE_INCOME_TAX,             // 4
	SQUARE_READING_RR,             // 5
	SQUARE_ORIENTAL,
	SQUARE_CHANCE_1,
	SQUARE_VERMONT,
	SQUARE_CONNECTICUT,
	SQUARE_JUST_VISITING,          // 10
	SQUARE_ST_CHARLES,
	SQUARE_ELECTRIC_CO,            // 12
	SQUARE_STATES,
	SQUARE_VIRGINIA,
	SQUARE_PENNSYLVANIA_RR,        // 15
	SQUARE_ST_JAMES,
	SQUARE_CC_2,
	SQUARE_TENNESSEE,
	SQUARE_NEW_YORK,
	SQUARE_FREE_PARKING,           // 20
	SQUARE_KENTUCKY,
	SQUARE_CHANCE_2,
	SQUARE_INDIANA,
	SQUARE_ILLINOIS,
	SQUARE_BO_RR,                  // 25
	SQUARE_ATLANTIC,
	SQUARE_VENTNOR,
	SQUARE_WATER_WORKS,            // 28
	SQUARE_MARVIN,
	SQUARE_GO_TO_JAIL,             // 30
	SQUARE_PACIFIC,
	SQUARE_N_CAROLINA,
	SQUARE_CC_3,
	SQUARE_PENNSYLVANIA,
	SQUARE_SHORT_LINE_RR,          // 35
	SQUARE_CHANCE_3,
	SQUARE_PARK_PLACE,
	SQUARE_LUXURY_TAX,             // 38
	SQUARE_BOARDWALK,
	SQUARE_IN_JAIL,                // 40
};

#define IS_RAILROAD(_s) (_s == SQUARE_READING_RR || _s == SQUARE_PENNSYLVANIA_RR || _s == SQUARE_BO_RR || _s == SQUARE_SHORT_LINE_RR)

enum GameReturn
{
	OK,
	ERR_NO_PLAYERS,	// no players defined!
	INCOME_TAX_200,
	INCOME_TAX_10_PERCENT,
};

enum GameAction
{
	ACTION_DONE,
	ACTION_ROLL,
	ACTION_MORTGAGE,
	ACTION_TRADE,
	ACTION_BUY,
	ACTION_SELL,
};

enum ChanceAction
{
	ACTION_MOVE_RELATIVE,
	ACTION_MOVE_TO_SQUARE,
	ACTION_PAY_ALL_PLAYERS,
	ACTION_GOOJF,
	ACTION_PAY,
	ACTION_GET_PAID,
	ACTION_MOVE_TO_UTILITY,
	ACTION_STREET_REPAIRS_CC,
};

struct chance_card
{
	std::string description;
	int action;
	int value;
};

class CGameUI;

class CGame
{
public:
   CGame(CGameUI* ui);
   ~CGame();

	bool Initialize(); // perform one-time initialization
	int Play(); // start the game engine
   void NewGame(); // reset the state
   bool IsCurrentPlayerInJail();
   unsigned int ActivePlayers(); // how many active players are left
   void PlayerOutOfBusiness(int id); // mark this player as 'out'
   void Over(); // game is done - who won?
   Player* GetCurrentPlayer(); // who's turn is it?
   void FinishTurn(); // finish this player's turn
   void DoublesCountInc();
   void DoublesCountReset();
   unsigned int DoublesCount();
   bool IsOver() { return gameOver; }
   int HasAssets(int playerID);
   void SetPlayerName(unsigned int id, const char* name);
   bool AddPlayer(int token, const char* name);
   void SetCurrentPlayerSquare(int square);
   int GetCurrentPlayerSquare();
   void MovePlayer(int square);
   void RollDice();
   void PayMoney(int from, int to, int amount);
   void SetPlayerSquare(int id, int square);
   int GetPlayerSquare(int id);
   int StartTurn();
	void SetPlayerOptions();
	void ReadDice(int* d1, int *d2);
	int GetAction();
	const std::string GetSquareName(int square);
	void Auction(int square);
	void BuyProperty(int player, int price, int square);
	bool TokenInUse(int i);
	void Mortgage(int property);
	int ChanceCard();
	int CommunityChestCard();

protected:
   CGameUI& m_ui;
   bool gameOver;       // is the game active?
   unsigned int m_players;         // how many active players are there
   unsigned int doublesCount;       // current player can only roll doubles 3 times before going to jail
   Player m_player[MAX_PLAYERS];   // the actual players
   int m_currentPlayer;            // who's turn is it?
   int dice[2];
   int owner[NUM_PROPERTIES];
	bool rolledDice;					// did the current player already roll the dice?
};

#endif // _GAME__H

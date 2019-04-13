#ifndef _GAME__H
#define _GAME__H

#include "ui_game.h"
#include "board.h"
#include "player.h"

#define MAX_PLAYERS 4
#define STARTING_CASH 1500
#define ACCORDING_TO_DICE -1
#define PLAYER_BANK  0xFF

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

#define IS_RAILROAD(_s) (_s == SQUARE_READING_RR || _s == SQUARE_PENNSYLVANIA_RR || _s == SQUARE_BO_RR || _s == SQUARE_SHORT_LINE_RR)

class CGame
{
public:
   CGame(CGameUI& ui);
   ~CGame();
   void NewGame();
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

protected:
   CGameUI& m_ui;
   CClonopolyBoard board;
   bool gameOver;       // is the game active?
   unsigned int players;         // how many active players are there
   unsigned int doublesCount;       // current player can only roll doubles 3 times before going to jail
   Player player[MAX_PLAYERS];   // the actual players
   int currentPlayer;            // who's turn is it?
   int dice[2];
   int owner[NUM_PROPERTIES];
};

#endif // _GAME__H

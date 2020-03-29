#ifndef _GAME_UI__H
#define _GAME_UI__H

#include "game.h"
#include "player.h"
#include "property.h"

class CGame;

class CGameUI
{
public:
   virtual ~CGameUI() {};
   void Init(CGame* g) { m_game = g; }
   virtual void SplashScreen()=0;

	// Prompt the user to add players. Each player added must call game->AddPlayer()
	virtual void GetPlayers()=0;

   virtual void SetTokenActive(int id)=0;
   virtual void Doubles()=0;
   virtual bool BuyProperty(const property& p)=0;
   virtual bool LiquidateAssets()=0;
   virtual void SetPlayerSquare(int player, int square)=0;
   virtual void SetCurrentPlayerStatus(Player& p)=0;
   virtual void ShowOptionsMenu()=0;
   virtual void DeleteStatusArea(void)=0;
   virtual void SetCurrentPlayerStatusArea(Player* p)=0;
   virtual void StatusAreaSetPlayerIconList(int count)=0;
   virtual void DisableStatusArea(void)=0;
   virtual void EnableStatusArea(void)=0;
   virtual void DrawStatusArea()=0;
   //virtual void DrawSplash()=0;
   virtual void RollDice()=0;
	virtual void SetPlayerOptions(bool roll, bool mortgage, bool buy, bool sell, bool trade, bool done)=0;
	virtual int GetAction()=0;
	virtual void CommunityChestCard()=0;
	virtual void ChanceCard()=0;
	virtual int IncomeTax()=0;
	virtual int GetAuctionBid(Player& p, int price)=0;

protected:
	CGame* m_game;
};

#endif // _GAME_UI__H

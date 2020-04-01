#include "ui_game.h"

class CTextUI : public CGameUI
{
public:
	CTextUI();
   virtual ~CTextUI() {};
   void SplashScreen();
	void GetPlayers();
   void SetTokenActive(int id);
   void Doubles();
   bool BuyProperty(const property& p);
   bool LiquidateAssets();
   void SetPlayerSquare(int player, int square);
   void SetCurrentPlayerStatus(Player& p);
   void ShowOptionsMenu();
   void DeleteStatusArea(void);
   void SetCurrentPlayerStatusArea(Player* p);
   void StatusAreaSetPlayerIconList(int count);
   void DisableStatusArea(void);
   void EnableStatusArea(void);
   void DrawStatusArea();
   //void DrawSplash();
   void RollDice();
	void SetPlayerOptions(bool roll, bool mortgage, bool buy, bool sell, bool trade, bool done);
	int GetAction();
	void CommunityChestCard(int card);
	void ChanceCard(int card);
	int IncomeTax();
	int GetAuctionBid(Player& p, int price);
	int Mortgage(Player& p);
};

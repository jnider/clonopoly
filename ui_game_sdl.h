#ifndef _GAME_UI_SDL__H
#define _GAME_UI_SDL__H

#include "ui_game.h"
#include "id.h"

struct StatusArea
{
   Ifel* ifel;
   SDL_Surface* surface;
   Player* player;
   Image** playerIconList;
   int playerIconCount;
   //CClonopolyBoard& board;
};

class CSDLGameUI : public CGameUI
{
public:
   ~CSDLGameUI();
   int Init(IfelOnKeyPressedFn kp, IfelOnMouseClickFn mc);
   void SetPlayerSquare(int id, const SDL_Rect& loc);
   void SetTokenActive(int id);
   void Doubles(IfelOnMouseClickFn mc);
   bool BuyProperty(const property& p);
   bool LiquidateAssets();
   void SetPlayerSquare(int id, SDL_Rect& loc);
   void SetCurrentPlayerStatus(Player& p);
   void ShowOptionsMenu();
   void DeleteStatusArea(void);
   void SetCurrentPlayerStatusArea(Player* p);
   void StatusAreaSetPlayerIconList(Image** l, int count);
   void DisableStatusArea(void);
   void EnableStatusArea(void);
   void DrawStatusArea();

protected:
   int CreateStatusArea(const SDL_Rect* loc);

protected:
   Image* image[ID_IMG_COUNT];
   MessageBox* messageBox;
   struct StatusArea* status;
};

#endif // _GAME_UI_SDL__H


#ifndef _GAME_UI__H
#define _GAME_UI__H

#include "ui.h" // IfelOnKeyPressedFn
#include "player.h"
#include "property.h"

class CGameUI
{
public:
   virtual ~CGameUI() {};
   virtual int Init(IfelOnKeyPressedFn kp, IfelOnMouseClickFn mc)=0;
   virtual void SetTokenActive(int id)=0;
   virtual void Doubles(IfelOnMouseClickFn mc)=0;
   virtual bool BuyProperty(const property& p)=0;
   virtual bool LiquidateAssets()=0;
   virtual void SetPlayerSquare(int id, SDL_Rect& loc)=0;
   virtual void SetCurrentPlayerStatus(Player& p)=0;
   virtual void ShowOptionsMenu()=0;
   virtual void DeleteStatusArea(void)=0;
   virtual void SetCurrentPlayerStatusArea(Player* p)=0;
   virtual void StatusAreaSetPlayerIconList(Image** l, int count)=0;
   virtual void DisableStatusArea(void)=0;
   virtual void EnableStatusArea(void)=0;
};

#endif // _GAME_UI__H

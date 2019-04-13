#ifndef _STATUS__H
#define _STATUS__H

#include "player.h"

int CreateStatusArea(const SDL_Rect* loc);
void DeleteStatusArea(void);
void SetCurrentPlayerStatusArea(Player* p);
void StatusAreaSetPlayerIconList(Image** l, int count);
void DisableStatusArea(void);
void EnableStatusArea(void);

#endif // _STATUS__H


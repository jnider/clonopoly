#include "player.h"

Player::Player()
{
   properties = ListCreate();
}

Player::~Player()
{
   ListDestroy(properties);
}

property *Player::GetFirstProperty()
{
   return (property*)ListGetFirst(properties, &properties_iter);
}

property *Player::GetNextProperty()
{
   return (property*)ListGetNext(&properties_iter);
}


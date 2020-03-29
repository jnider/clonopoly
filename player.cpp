#include "player.h"

Player::Player(): token(-1)
{
   //properties = ListCreate();
}

Player::~Player()
{
//   ListDestroy(properties);
}

property* Player::GetFirstProperty()
{
 //  return (property*)ListGetFirst(properties, &properties_iter);
	properties_iter = properties.begin();
	return *properties_iter;
}

property* Player::GetNextProperty()
{
  // return (property*)ListGetNext(&properties_iter);
	properties_iter++;
	if (properties_iter == properties.end())
		return NULL;
	return *properties_iter;
}

bool Player::HasProperties()
{
	return (!properties.empty());
}

void Player::AddProperty(property* p)
{
	properties.push_back(p);
}


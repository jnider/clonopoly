#include "player.h"

Player::Player(): token(-1),
						m_properties(0)
{
   //properties = ListCreate();
}

Player::~Player()
{
//   ListDestroy(properties);
}

bool Player::HasProperties()
{
	return (m_properties);
}

void Player::AddProperty(int index)
{
	m_properties |= (1<<index);
}

bool Player::OwnsProperty(int index)
{
	return m_properties & (1<<index);
}


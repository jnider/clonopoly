#ifndef _PLAYER__H
#define _PLAYER__H

#include <vector>
#include "property.h"
//#include "list.h"

class Player
{
public:
   Player();
   ~Player();
	bool HasProperties();
	void AddProperty(int index);
	bool OwnsProperty(int index);

public:
   int index;              // what player number am I? (index into player[])
   int token;              // which piece am I using?
   int active;             // still in the game?
   int location;           // which square am I currently on?
   int money;              // how much cash do I have
   int inJail;             // am I in jail?
   int turnsLeftInJail;    // how long until I have to pay
   char* name;             // what's my name? -- should be unicode
	unsigned long m_properties; // bitmap of owned properties
	//std::vector<property*> properties;
	//std::vector<property*>::iterator properties_iter;
   //list* properties;       // the properties I own
   //iterator properties_iter;
   //list* cards;            // "get out of jail free", etc.
   //iterator cards_iter;
};

#endif // _PLAYER__H

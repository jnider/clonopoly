#ifndef _PLAYER__H
#define _PLAYER__H

#include "property.h"
#include "list.h"

class Player
{
public:
   Player();
   ~Player();
   property *GetFirstProperty();
   property *GetNextProperty();

public:
   int index;              // what player number am I? (index into player[])
   int token;              // which piece am I using?
   int active;             // still in the game?
   int location;           // which square am I currently on?
   int money;              // how much cash do I have
   int inJail;             // am I in jail?
   int turnsLeftInJail;    // how long until I have to pay
   char* name;             // what's my name? -- should be unicode
   list* properties;       // the properties I own
   iterator properties_iter;
   list* cards;            // "get out of jail free", etc.
   iterator cards_iter;
};

#endif // _PLAYER__H

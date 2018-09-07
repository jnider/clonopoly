#ifndef _GAME__H
#define _GAME__H

#define MAX_PLAYERS 4
#define MAX_HOUSES 5 // how many houses can be on each property (house #5 = hotel)
#define NUM_PROPERTIES (10 * 4)
#define STARTING_CASH 1500
#define ACCORDING_TO_DICE -1
#define PLAYER_BANK  0xFF


typedef struct Player
{
   int index;              // what player number am I? (index into player[])
   int token;              // which piece am I using?
   int active;             // still in the game?
   int location;           // which square am I currently on?
   int money;              // how much cash do I have
   int inJail;             // am I in jail?
   int turnsLeftInJail;    // how long until I have to pay
   int doublesCount;       // can only roll doubles 3 times before going to jail
   char* name;             // what's my name? -- should be unicode
} Player;

#endif // _GAME__H

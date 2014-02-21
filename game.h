#ifndef _GAME__H
#define _GAME__H

#define MAX_PLAYERS 4
#define MAX_HOUSES 5 // how many houses can be on each property (house #5 = hotel)
#define NUM_PROPERTIES (10 * 4)
#define STARTING_CASH 1500
#define ACCORDING_TO_DICE -1
#define PLAYER_BANK  0xFF

#define PROPERTY_HEIGHT 92
#define PROPERTY_WIDTH 56
#define PROPERTY_TOP_ROW 6
#define PROPERTY_BOTTOM_ROW 604
#define PROPERTY_LEFT_COLUMN 7

#define SQUARE_GO                      0
#define SQUARE_INCOME_TAX              4
#define SQUARE_READING_RR              5
#define SQUARE_JUST_VISITING           10
#define SQUARE_ELECTRIC_CO             12
#define SQUARE_PENNSYLVANIA_RR         15
#define SQUARE_FREE_PARKING            20
#define SQUARE_BO_RR                   25
#define SQUARE_WATER_WORKS             28
#define SQUARE_GO_TO_JAIL              30
#define SQUARE_SHORT_LINE_RR           35
#define SQUARE_LUXURY_TAX              38
#define SQUARE_IN_JAIL                 40


typedef struct Player
{
   int token;              // which piece am I using?
   int active;             // still in the game?
   int location;           // which square am I currently on?
   int money;              // how much cash do I have
   int inJail;             // am I in jail?
   int turnsLeftInJail;    // how long until I have to pay
   int doublesCount;       // can only roll doubles 3 times before going to jail
   char* name;
} Player;

typedef struct Property
{
   char* name;             // printable property name
   int value;              // original cost
   int rent[MAX_HOUSES];   // how much visitors have to pay
   int numHouses;          // how many houses are on the property
   int owner;              // who owns it?
   int mortgaged;          // is it mortgaged?
   SDL_Rect loc;           // boundaries of the square on the board
} Property;


#endif // _GAME__H

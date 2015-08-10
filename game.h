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

enum PropertySet
{
   SET_NONE,      // anything that can't be owned (go, jail, CC/chance, income tax)
   SET_RR,        // Railroads - Reading, Pennsylvania, B&O,  
   SET_UTIL,      // Utilities - Electric co, Water works
   SET_PURPLE,    // Mediterranean, Baltic
   SET_LT_BLUE,   // Oriental, Vermont, Connecticut
   SET_VIOLET,    // St. Charles, States, Virginia
   SET_GOLD,      // St. James, Tennessee, New York
   SET_RED,       // Kentucky, Indiana, Illinois
   SET_YELLOW,    // Atlantic, Ventnor, Marvin
   SET_GREEN,     // Pacific, N.Carolina, Pennsylvania
   SET_BLUE,      // Park Place, Boardwalk
};

typedef struct Property
{
   char* name;             // printable property name
   int value;              // original cost
   int set;                // which set does this property belong to?
   int rent[MAX_HOUSES+1]; // how much visitors have to pay
   int mortgage;           // mortgage value
   int houseCost;          // how much does it cost to buy a house
   int numHouses;          // how many houses are on the property
   int owner;              // who owns it?
   int mortgaged;          // is it mortgaged?
   SDL_Rect loc;           // boundaries of the square on the board
} Property;

// get the array of all properties
Property* GetProperties(void);

#endif // _GAME__H

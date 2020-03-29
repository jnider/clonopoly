#ifndef _PROPERTY__H
#define _PROPERTY__H

#include <string>
//#include "SDL2/SDL.h" // SDL_Rect
//#include "game.h"

#define MAX_HOUSES 5 // how many houses can be on each property (house #5 = hotel)

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

/*
class CProperty
{
public:
   CProperty();
   //CProperty(const char* name, int val, PropertySet, int* rent, int mortgage, int cost);
   CProperty(const char* name, int value, PropertySet set, int rent[], int mortgage, int houseCost, int x, int y, int w, int h);
   ~CProperty();

   int Rent() { return m_rent[m_numHouses]; }
   void SetOwner(int owner) { m_owner = owner; }
   int Owner() { return m_owner; }
   bool IsMortgaged() { return m_mortgaged; }
   const std::string& Name() { return m_name; }
   const SDL_Rect& Location() { return m_loc; }
   int Value() { return m_value; }

private:
   const std::string m_name; // printable property name
   int m_value;              // original cost
   enum PropertySet m_set;   // which set does this property belong to?
   int m_rent[MAX_HOUSES+1]; // how much visitors have to pay
   int m_mortgage;           // mortgage value
   int m_houseCost;          // how much does it cost to buy a house
   int m_numHouses;          // how many houses are on the property
   int m_owner;              // who owns it?
   bool m_mortgaged;          // is it mortgaged?
   SDL_Rect m_loc;           // boundaries of the square on the board
};
*/

typedef struct property
{
   const std::string name; // printable property name
   int value;              // original cost
   enum PropertySet set;   // which set does this property belong to?
   int rent[MAX_HOUSES+1]; // how much visitors have to pay
   int mortgage;           // mortgage value
   int houseCost;          // how much does it cost to buy a house
   int numHouses;          // how many houses are on the property
   int owner;              // who owns it?
   bool mortgaged;         // is it mortgaged?
   //SDL_Rect m_loc;           // boundaries of the square on the board
} property;

#endif // _PROPERTY__H

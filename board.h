#ifndef _BOARD__H
#define _BOARD__H

#include "game.h"
#include "property.h"

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

class CClonopolyBoard
{
public:
   CClonopolyBoard();
   void Reset(); /* reset the board back to initial positions */
   CProperty& Property(int square); /* how much does this property cost to buy? */

private:
   int owner[NUM_PROPERTIES];
   static CProperty m_board[NUM_PROPERTIES];
};

#endif // _BOARD__H

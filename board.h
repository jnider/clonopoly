#ifndef _BOARD__H
#define _BOARD__H

#include "property.h"

#define NUM_PROPERTIES (10 * 4)

// Move these to the UI
//#define PROPERTY_HEIGHT 92
//#define PROPERTY_WIDTH 56
//#define PROPERTY_TOP_ROW 6
//#define PROPERTY_BOTTOM_ROW 604
//#define PROPERTY_LEFT_COLUMN 7

class CClonopolyBoard
{
public:
   CClonopolyBoard();
   property& Property(int square);

private:
   static property m_board[NUM_PROPERTIES];
};

#endif // _BOARD__H

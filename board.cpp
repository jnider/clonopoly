#include "board.h"
#include "game.h"

property CClonopolyBoard::m_board[NUM_PROPERTIES] =
{
   { "Go",                    0,   SET_NONE,    { 0,   0,   0,   0,   0,   0},   0,   0, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*9, PROPERTY_BOTTOM_ROW, PROPERTY_HEIGHT, PROPERTY_HEIGHT}},
   { "Mediterranean",         60,  SET_PURPLE,  { 2,  10,  30,  90, 160, 250},  30,  50, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*8, PROPERTY_BOTTOM_ROW, PROPERTY_WIDTH, PROPERTY_HEIGHT}},
   { "Community Chest",       0,   SET_NONE,    { 0,   0,   0,   0,   0,   0},   0,   0, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*7, PROPERTY_BOTTOM_ROW, PROPERTY_WIDTH, PROPERTY_HEIGHT}},
   { "Baltic",                60,  SET_PURPLE,  { 4,  20,  60, 180, 320, 450},  30,  50, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*6, PROPERTY_BOTTOM_ROW, PROPERTY_WIDTH, PROPERTY_HEIGHT}},
   { "Income Tax",            0,   SET_NONE,    { 0,   0,   0,   0,   0,   0},   0,   0, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*5, PROPERTY_BOTTOM_ROW, PROPERTY_WIDTH, PROPERTY_HEIGHT}},
   { "Reading Railroad",      200, SET_RR,      {25,   0,   0,   0,   0,   0}, 100,   0, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*4, PROPERTY_BOTTOM_ROW, PROPERTY_WIDTH, PROPERTY_HEIGHT}},
   { "Oriental Ave",          100, SET_LT_BLUE, { 6,  30,  90, 270, 400, 550},  50,  50, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*3, PROPERTY_BOTTOM_ROW, PROPERTY_WIDTH, PROPERTY_HEIGHT}},
   { "Chance",                0,   SET_NONE,    { 0,   0,   0,   0,   0,   0},   0,   0, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*2, PROPERTY_BOTTOM_ROW, PROPERTY_WIDTH, PROPERTY_HEIGHT}},
   { "Vermont Ave",           100, SET_LT_BLUE, { 6,  30,  90, 270, 400, 550},  50,  50, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*1, PROPERTY_BOTTOM_ROW, PROPERTY_WIDTH, PROPERTY_HEIGHT}},
   { "Connecticut Ave",       120, SET_LT_BLUE, { 8,  40, 100, 300, 450, 600},  60,  50, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT, PROPERTY_BOTTOM_ROW, PROPERTY_WIDTH, PROPERTY_HEIGHT}},
///////////////////
   { "Just Visiting",         0,   SET_NONE,    { 0,   0,   0,   0,   0,    0},   0,   0, 0, -1, 0, {PROPERTY_LEFT_COLUMN, PROPERTY_BOTTOM_ROW, PROPERTY_HEIGHT, PROPERTY_HEIGHT}},
   { "St. Charles",           140, SET_VIOLET,  {10,  50, 150, 450, 625,  750},  70, 100, 0, -1, 0, {PROPERTY_LEFT_COLUMN, PROPERTY_TOP_ROW+PROPERTY_HEIGHT+PROPERTY_WIDTH*8, PROPERTY_HEIGHT, PROPERTY_WIDTH}},
   { "Electric Company",      150, SET_UTIL,    { 0,   0,   0,   0,   0,    0},   0,   0, 0, -1, 0, {PROPERTY_LEFT_COLUMN, PROPERTY_TOP_ROW+PROPERTY_HEIGHT+PROPERTY_WIDTH*7, PROPERTY_HEIGHT, PROPERTY_WIDTH}},
   { "States Ave",            140, SET_VIOLET,  {10,  50, 150, 450, 625,  750},  70, 100, 0, -1, 0, {PROPERTY_LEFT_COLUMN, PROPERTY_TOP_ROW+PROPERTY_HEIGHT+PROPERTY_WIDTH*6, PROPERTY_HEIGHT, PROPERTY_WIDTH}},
   { "Virginia Ave",          160, SET_VIOLET,  {12,  60, 180, 500, 700,  900},  80, 100, 0, -1, 0, {PROPERTY_LEFT_COLUMN, PROPERTY_TOP_ROW+PROPERTY_HEIGHT+PROPERTY_WIDTH*5, PROPERTY_HEIGHT, PROPERTY_WIDTH}},
   { "Pennsylvania Railroad", 200, SET_RR,      {25,   0,   0,   0,   0,    0}, 100,   0, 0, -1, 0, {PROPERTY_LEFT_COLUMN, PROPERTY_TOP_ROW+PROPERTY_HEIGHT+PROPERTY_WIDTH*4, PROPERTY_HEIGHT, PROPERTY_WIDTH}},
   { "St. James Place",       180, SET_GOLD,    {14,  70, 200, 550, 750,  950},  90, 100, 0, -1, 0, {PROPERTY_LEFT_COLUMN, PROPERTY_TOP_ROW+PROPERTY_HEIGHT+PROPERTY_WIDTH*3, PROPERTY_HEIGHT, PROPERTY_WIDTH}},
   { "Community Chest",       0,   SET_NONE,    { 0,   0,   0,   0,   0,    0},   0,   0, 0, -1, 0, {PROPERTY_LEFT_COLUMN, PROPERTY_TOP_ROW+PROPERTY_HEIGHT+PROPERTY_WIDTH*2, PROPERTY_HEIGHT, PROPERTY_WIDTH}},
   { "Tennessee Ave",         180, SET_GOLD,    {14,  70, 200, 550, 750,  950},  90, 100, 0, -1, 0, {PROPERTY_LEFT_COLUMN, PROPERTY_TOP_ROW+PROPERTY_HEIGHT+PROPERTY_WIDTH*1, PROPERTY_HEIGHT, PROPERTY_WIDTH}},
   { "New York Ave",          200, SET_GOLD,    {16,  80, 220, 600, 800, 1000}, 100, 100, 0, -1, 0, {PROPERTY_LEFT_COLUMN, PROPERTY_TOP_ROW+PROPERTY_HEIGHT, PROPERTY_HEIGHT, PROPERTY_WIDTH}},
///////////////////
   { "Free Parking",          0,   SET_NONE,    { 0,   0,   0,   0,    0,    0},   0,   0, 0, -1, 0, {PROPERTY_LEFT_COLUMN, PROPERTY_TOP_ROW, PROPERTY_HEIGHT, PROPERTY_HEIGHT}},
   { "Kentucky Ave",          220, SET_RED,     {18,  90, 250, 700,  875, 1050}, 110, 150, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT, PROPERTY_TOP_ROW, PROPERTY_WIDTH, PROPERTY_HEIGHT}},
   { "Chance",                0,   SET_NONE,    { 0,   0,   0,   0,    0,    0},   0,   0, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*1, PROPERTY_TOP_ROW, PROPERTY_WIDTH, PROPERTY_HEIGHT}},
   { "Indiana",               220, SET_RED,     {18,  90, 250, 700,  875, 1050}, 110, 150, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*2, PROPERTY_TOP_ROW, PROPERTY_WIDTH, PROPERTY_HEIGHT}},
   { "Illinois",              240, SET_RED,     {20, 100, 300, 750,  925, 1100}, 120, 150, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*3, PROPERTY_TOP_ROW, PROPERTY_WIDTH, PROPERTY_HEIGHT}},
   { "B & O Railroad",        200, SET_RR,      {25,   0,   0,   0,    0,    0}, 100,   0, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*4, PROPERTY_TOP_ROW, PROPERTY_WIDTH, PROPERTY_HEIGHT}},
   { "Atlantic Ave",          260, SET_YELLOW,  {22, 110, 330, 800,  975, 1150}, 130, 150, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*5, PROPERTY_TOP_ROW, PROPERTY_WIDTH, PROPERTY_HEIGHT}},
   { "Ventnor Ave",           260, SET_YELLOW,  {22, 110, 330, 800,  975, 1150}, 130, 150, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*6, PROPERTY_TOP_ROW, PROPERTY_WIDTH, PROPERTY_HEIGHT}},
   { "Water Works",           150, SET_UTIL,    { 0,   0,   0,   0,    0,    0},  75,   0, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*7, PROPERTY_TOP_ROW, PROPERTY_WIDTH, PROPERTY_HEIGHT}},
   { "Marvin Gardens",        280, SET_YELLOW,  {24, 120, 360, 850, 1025, 1200}, 140, 150, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*8, PROPERTY_TOP_ROW, PROPERTY_WIDTH, PROPERTY_HEIGHT}},
///////////////////
   { "Go To Jail",            0,   SET_NONE,    { 0,   0,   0,    0,    0,    0},   0,   0, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*9, PROPERTY_TOP_ROW, PROPERTY_HEIGHT, PROPERTY_HEIGHT}},
   { "Pacific Ave",           300, SET_GREEN,   {26, 130, 390,  900, 1100, 1275}, 150, 200, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*9, PROPERTY_TOP_ROW+PROPERTY_HEIGHT, PROPERTY_HEIGHT, PROPERTY_WIDTH}},
   { "North Carolina Ave",    300, SET_GREEN,   {26, 130, 390,  900, 1100, 1275}, 150, 200, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*9, PROPERTY_TOP_ROW+PROPERTY_HEIGHT+PROPERTY_WIDTH*1, PROPERTY_HEIGHT, PROPERTY_WIDTH}},
   { "Community Chest",       0,   SET_NONE,    { 0,   0,   0,    0,    0,    0},   0,   0, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*9, PROPERTY_TOP_ROW+PROPERTY_HEIGHT+PROPERTY_WIDTH*2, PROPERTY_HEIGHT, PROPERTY_WIDTH}},
   { "Pennsylvania Ave",      320, SET_GREEN,   {28, 150, 450, 1000, 1200, 1400}, 160, 200, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*9, PROPERTY_TOP_ROW+PROPERTY_HEIGHT+PROPERTY_WIDTH*3, PROPERTY_HEIGHT, PROPERTY_WIDTH}},
   { "Short Line Railroad",   200, SET_RR,      {25,   0,   0,    0,    0,    0}, 100,   0, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*9, PROPERTY_TOP_ROW+PROPERTY_HEIGHT+PROPERTY_WIDTH*4, PROPERTY_HEIGHT, PROPERTY_WIDTH}},
   { "Chance",                0,   SET_NONE,    { 0,   0,   0,    0,    0,    0},   0,   0, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*9, PROPERTY_TOP_ROW+PROPERTY_HEIGHT+PROPERTY_WIDTH*5, PROPERTY_HEIGHT, PROPERTY_WIDTH}},
   { "Park Place",            350, SET_BLUE,    {35, 175, 500, 1100, 1300, 1500}, 175, 200, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*9, PROPERTY_TOP_ROW+PROPERTY_HEIGHT+PROPERTY_WIDTH*6, PROPERTY_HEIGHT, PROPERTY_WIDTH}},
   { "Luxury Tax",            0,   SET_NONE,    { 0,   0,   0,    0,    0,    0},   0,   0, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*9, PROPERTY_TOP_ROW+PROPERTY_HEIGHT+PROPERTY_WIDTH*7, PROPERTY_HEIGHT, PROPERTY_WIDTH}},
   { "Boardwalk",             400, SET_BLUE,    {50, 200, 600, 1400, 1700, 2000}, 200, 200, 0, -1, 0, {PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*9, PROPERTY_TOP_ROW+PROPERTY_HEIGHT+PROPERTY_WIDTH*8, PROPERTY_HEIGHT, PROPERTY_WIDTH}},
};

CClonopolyBoard::CClonopolyBoard()
/*
   m_board(
      {
         CProperty( "Go",                    0,   SET_NONE,    { 0,   0,   0,   0,   0,   0},   0,   0, 
            PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*9, PROPERTY_BOTTOM_ROW, PROPERTY_HEIGHT, PROPERTY_HEIGHT),
         CProperty( "Mediterranean",         60,  SET_PURPLE,  { 2,  10,  30,  90, 160, 250},  30,  50,
            PROPERTY_LEFT_COLUMN+PROPERTY_HEIGHT+PROPERTY_WIDTH*8, PROPERTY_BOTTOM_ROW, PROPERTY_WIDTH, PROPERTY_HEIGHT)
      })
*/
{
}

property& CClonopolyBoard::Property(int square)
{
   return m_board[square];
}

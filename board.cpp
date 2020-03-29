#include "board.h"
#include "game.h"

property m_board[NUM_PROPERTIES] =
{
   { "Go",                    0,   SET_NONE,    { 0,   0,   0,    0,    0,    0},   0,   0, 0, -1, 0 },
   { "Mediterranean",         60,  SET_PURPLE,  { 2,  10,  30,   90,  160,  250},  30,  50, 0, -1, 0 },
   { "Community Chest",       0,   SET_NONE,    { 0,   0,   0,    0,    0,    0},   0,   0, 0, -1, 0 },
   { "Baltic",                60,  SET_PURPLE,  { 4,  20,  60,  180,  320,  450},  30,  50, 0, -1, 0 },
   { "Income Tax",            0,   SET_NONE,    { 0,   0,   0,    0,    0,    0},   0,   0, 0, -1, 0 },
   { "Reading Railroad",      200, SET_RR,      {25,   0,   0,    0,    0,    0}, 100,   0, 0, -1, 0 },
   { "Oriental Ave",          100, SET_LT_BLUE, { 6,  30,  90,  270,  400,  550},  50,  50, 0, -1, 0 },
   { "Chance",                0,   SET_NONE,    { 0,   0,   0,    0,    0,    0},   0,   0, 0, -1, 0 },
   { "Vermont Ave",           100, SET_LT_BLUE, { 6,  30,  90,  270,  400,  550},  50,  50, 0, -1, 0 },
   { "Connecticut Ave",       120, SET_LT_BLUE, { 8,  40, 100,  300,  450,  600},  60,  50, 0, -1, 0 },
///////////////////
   { "Just Visiting",         0,   SET_NONE,    { 0,   0,   0,    0,    0,    0},   0,   0, 0, -1, 0 },
   { "St. Charles",           140, SET_VIOLET,  {10,  50, 150,  450,  625,  750},  70, 100, 0, -1, 0 },
   { "Electric Company",      150, SET_UTIL,    { 0,   0,   0,    0,    0,    0},   0,   0, 0, -1, 0 },
   { "States Ave",            140, SET_VIOLET,  {10,  50, 150,  450,  625,  750},  70, 100, 0, -1, 0 },
   { "Virginia Ave",          160, SET_VIOLET,  {12,  60, 180,  500,  700,  900},  80, 100, 0, -1, 0 },
   { "Pennsylvania Railroad", 200, SET_RR,      {25,   0,   0,    0,    0,    0}, 100,   0, 0, -1, 0 },
   { "St. James Place",       180, SET_GOLD,    {14,  70, 200,  550,  750,  950},  90, 100, 0, -1, 0 },
   { "Community Chest",       0,   SET_NONE,    { 0,   0,   0,    0,    0,    0},   0,   0, 0, -1, 0 },
   { "Tennessee Ave",         180, SET_GOLD,    {14,  70, 200,  550,  750,  950},  90, 100, 0, -1, 0 },
   { "New York Ave",          200, SET_GOLD,    {16,  80, 220,  600,  800, 1000}, 100, 100, 0, -1, 0 },
///////////////////
   { "Free Parking",          0,   SET_NONE,    { 0,   0,   0,    0,    0,    0},   0,   0, 0, -1, 0 },
   { "Kentucky Ave",          220, SET_RED,     {18,  90, 250,  700,  875, 1050}, 110, 150, 0, -1, 0 },
   { "Chance",                0,   SET_NONE,    { 0,   0,   0,    0,    0,    0},   0,   0, 0, -1, 0 },
   { "Indiana",               220, SET_RED,     {18,  90, 250,  700,  875, 1050}, 110, 150, 0, -1, 0 },
   { "Illinois",              240, SET_RED,     {20, 100, 300,  750,  925, 1100}, 120, 150, 0, -1, 0 },
   { "B & O Railroad",        200, SET_RR,      {25,   0,   0,    0,    0,    0}, 100,   0, 0, -1, 0 },
   { "Atlantic Ave",          260, SET_YELLOW,  {22, 110, 330,  800,  975, 1150}, 130, 150, 0, -1, 0 },
   { "Ventnor Ave",           260, SET_YELLOW,  {22, 110, 330,  800,  975, 1150}, 130, 150, 0, -1, 0 },
   { "Water Works",           150, SET_UTIL,    { 0,   0,   0,    0,    0,    0},  75,   0, 0, -1, 0 },
   { "Marvin Gardens",        280, SET_YELLOW,  {24, 120, 360,  850, 1025, 1200}, 140, 150, 0, -1, 0 },
///////////////////
   { "Go To Jail",            0,   SET_NONE,    { 0,   0,   0,    0,    0,    0},   0,   0, 0, -1, 0 },
   { "Pacific Ave",           300, SET_GREEN,   {26, 130, 390,  900, 1100, 1275}, 150, 200, 0, -1, 0 },
   { "North Carolina Ave",    300, SET_GREEN,   {26, 130, 390,  900, 1100, 1275}, 150, 200, 0, -1, 0 },
   { "Community Chest",       0,   SET_NONE,    { 0,   0,   0,    0,    0,    0},   0,   0, 0, -1, 0 },
   { "Pennsylvania Ave",      320, SET_GREEN,   {28, 150, 450, 1000, 1200, 1400}, 160, 200, 0, -1, 0 },
   { "Short Line Railroad",   200, SET_RR,      {25,   0,   0,    0,    0,    0}, 100,   0, 0, -1, 0 },
   { "Chance",                0,   SET_NONE,    { 0,   0,   0,    0,    0,    0},   0,   0, 0, -1, 0 },
   { "Park Place",            350, SET_BLUE,    {35, 175, 500, 1100, 1300, 1500}, 175, 200, 0, -1, 0 },
   { "Luxury Tax",            0,   SET_NONE,    { 0,   0,   0,    0,    0,    0},   0,   0, 0, -1, 0 },
   { "Boardwalk",             400, SET_BLUE,    {50, 200, 600, 1400, 1700, 2000}, 200, 200, 0, -1, 0 },
};


chance_card chance_cards[NUM_CHANCE_CARDS] =
{
	{ "Hospital Fees", "You had to go to the hospital. Pay $150" },
	{ "Beauty Contest", "You won second place in a beauty contest. Receive $10" },
};


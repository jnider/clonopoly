#ifndef _ID__H
#define _ID__H

// ifel id's
enum
{
   // images
   ID_IMG_BASE=0,
   ID_IMG_BOARD,
   ID_IMG_HORSE,
   ID_IMG_SHOE,
   ID_IMG_DOG,
   ID_IMG_BAG,
   ID_IMG_LAST,

   // buttons
   ID_BTN_BASE = 100,
   ID_BTN_OPTIONS,
   ID_BTN_CANCEL,
   ID_BTN_OK,
   ID_BTN_ROLL,
   ID_BTN_BUY_SELL,
   ID_BTN_TRADE,
   ID_BTN_LAST,

   // message boxes
   ID_MSGBOX_BASE = 200,
   ID_MSGBOX_NEW_GAME,
   ID_MSGBOX_ROLL,
   ID_MSGBOX_BUY_PROPERTY,

   // custom
   ID_MENU_BASE = 300,
   ID_MENU_OPTIONS
};

#define ID_IMG_COUNT (ID_IMG_LAST - ID_IMG_BASE - 1)
#define ID_BTN_COUNT (ID_BTN_LAST - ID_BTN_BASE - 1)
#define BUTTON_INDEX(_a) (_a - ID_BTN_BASE - 1)

#endif // _ID__H


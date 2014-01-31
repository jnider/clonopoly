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

   // buttons
   ID_BTN_BASE = 100,
   ID_BTN_OPTIONS,

   // message boxes
   ID_MSGBOX_BASE = 200,
   ID_MSGBOX1,

   // custom
   ID_MENU_BASE = 300,
   ID_MENU_OPTIONS
};

#define ID_IMG_COUNT (ID_IMG_BAG-ID_IMG_BASE)
#define ID_BTN_COUNT (ID_BTN_OPTIONS - ID_BTN_BASE)

#endif // _ID__H


#include "ui_game_sdl.h"
#include "options.h"
#include "property.h"
#include "SDL/SDL_gfxPrimitives.h"

static Button* button[ID_BTN_COUNT];

static const char* piece_name[] =
{
   "piece_horse",
   "piece_shoe",
   "piece_dog",
   "piece_bag"
};

static void DrawStatusCB(Ifel *i)
{
   CSDLGameUI* ui = (CSDLGameUI*)i->data;
   ui->DrawStatusArea();
}

int CSDLGameUI::Init(IfelOnKeyPressedFn kp, IfelOnMouseClickFn mc)
{
   image[ID_IMG_BOARD] = CreateImage(ID_IMG_BOARD, NULL, "graphics/board.png");
   image[ID_IMG_BOARD]->el.OnKeyPressed = kp;

   // load player piece images
   for (int i=ID_IMG_HORSE; i < ID_IMG_COUNT; i++)
   {
      char filename[64];
      sprintf(filename, "graphics/%s.png", piece_name[i-ID_IMG_HORSE]);
      image[i] = CreateImage(i, NULL, filename);
      if (!image[i])
      {
         fprintf(stderr, "Can't load image %s\n", filename);

         // unload all successfully loaded pieces
         for (int j=i-1; j >=ID_IMG_HORSE; j--)
            DeleteImage(image[j]);

         return 1;
      }
      image[i]->el.active = 0;
   }

   // load buttons
   SDL_Rect posRect;
   int index = BUTTON_INDEX(ID_BTN_OPTIONS);
   //button[index] = CreatePngButton(ID_BTN_OPTIONS, NULL, 0, image[ID_IMG_BOARD]->el.loc.y + image[ID_IMG_BOARD]->el.loc.h,
   //   "graphics/btn_options_1.png",
   //   "graphics/btn_options_2.png",
   //   "graphics/btn_options_3.png");
   posRect.x = 0;
   posRect.y = image[ID_IMG_BOARD]->el.loc.y + image[ID_IMG_BOARD]->el.loc.h;
   posRect.w = 200;
   posRect.h = 50;
   button[index] = CreateTextButton(ID_BTN_OPTIONS, NULL, &posRect, "OPTIONS");
   if (!button[index])
   {
      fprintf(stderr, "Can't create options button\n");
      return 2;
   }
   button[index]->el.OnMouseClick = mc;

   index = BUTTON_INDEX(ID_BTN_ROLL);
   posRect.x = button[BUTTON_INDEX(ID_BTN_OPTIONS)]->el.loc.x + button[BUTTON_INDEX(ID_BTN_OPTIONS)]->el.loc.w + 5;
   button[index] = CreateTextButton(ID_BTN_ROLL, NULL, &posRect, "ROLL");
   if (!button[index])
   {
      fprintf(stderr, "Can't create roll button\n");
      return 2;
   }
   button[index]->el.OnMouseClick = mc;
   DisableButton(button[index]);

   index = BUTTON_INDEX(ID_BTN_BUY_SELL);
   posRect.x = button[BUTTON_INDEX(ID_BTN_ROLL)]->el.loc.x + button[BUTTON_INDEX(ID_BTN_ROLL)]->el.loc.w + 5;
   button[index] = CreateTextButton(ID_BTN_ROLL, NULL, &posRect, "BUY/SELL");
   if (!button[index])
   {
      fprintf(stderr, "Can't create buy/sell button\n");
      return 2;
   }
   button[index]->el.OnMouseClick = mc;
   DisableButton(button[index]);

   // create status area
   posRect.x = image[ID_IMG_BOARD]->el.loc.x + image[ID_IMG_BOARD]->el.loc.w;
   posRect.y = 0;
   posRect.w = 275;
   posRect.h = 600;
   if (CreateStatusArea(&posRect) != 0)
   {
      fprintf(stderr, "Can't create status area\n");
      return 3;
   }
   StatusAreaSetPlayerIconList(image, ID_IMG_COUNT);

   return 0;
}

void CSDLGameUI::DisableStatusArea()
{
   status->ifel->active = 0;
}

/* update the position of the player piece */
void CSDLGameUI::SetPlayerSquare(int id, const SDL_Rect& loc)
{
   image[id]->el.loc.w = loc.w;
   image[id]->el.loc.h = loc.h;
   image[id]->el.loc.x = loc.x + ((image[id]->el.loc.w - image[id]->el.loc.w) >> 1); // without the brackets on the >> this doesn't calculate right
   image[id]->el.loc.y = loc.y + image[id]->el.loc.h - image[id]->el.loc.h;
}

void CSDLGameUI::SetTokenActive(int id)
{
   image[id]->el.active = 1;
}

CSDLGameUI::~CSDLGameUI()
{
   // free UI elements
   for (int i=ID_IMG_BASE+1; i < ID_IMG_BASE+ID_IMG_COUNT; i++)
      DeleteImage(image[i]);

   for (int i=ID_BTN_BASE; i < ID_BTN_BASE+ID_BTN_COUNT; i++)
   {
      DeleteButton(button[i-ID_BTN_BASE]);
   }
   DeleteOptionsMenu();
   DeleteStatusArea();
   DestroyUI();
}

bool CSDLGameUI::BuyProperty(const property& p)
{
   char msg[100];
   // endSquare
   sprintf(msg, "Would you like to buy %s? It costs $%i", p.m_name.c_str(),
      p.m_value);
   if (ModalMessageBox(ID_MSGBOX_BUY_PROPERTY, msg) == MB_YES)
      return true;
   return false;
}

/*
void CSDLGameUI::Update(const CGame& game)
{
   SetCurrentPlayerStatusArea(player[currentPlayer]);
   EnableStatusArea();
   EnableButton(button[BUTTON_INDEX(ID_BTN_ROLL)]);
}
*/

void CSDLGameUI::Doubles(IfelOnMouseClickFn mc)
{
   if (!messageBox)
   {
      messageBox = CreateMessageBox(ID_MSGBOX_ROLL, "Doubles - roll again");
      messageBox->el.OnMouseClick = mc;
   }
}

bool CSDLGameUI::LiquidateAssets()
{
   return false;
}

void CSDLGameUI::SetPlayerSquare(int id, SDL_Rect& loc)
{
}

void CSDLGameUI::SetCurrentPlayerStatus(Player& p)
{
}

void CSDLGameUI::ShowOptionsMenu()
{
}

void CSDLGameUI::DrawStatusArea()
{
   SDL_Surface* name;
   SDL_Surface* money;
   SDL_Surface* prop_name;
   SDL_Rect loc;

   // background
   roundedBoxRGBA(status->surface, 0, 0, status->surface->w, status->surface->h, 10, 100, 100, 0, 100);
   roundedBoxRGBA(status->surface, 6, 6, status->surface->w-10, status->surface->h-10, 10, 0, 80, 0, 255);

   // current player's name
   SDL_Color textColor;
   textColor.r = 0x8F;
   textColor.g = 0x80;
   textColor.b = 0xFF;
   loc.x = 10;
   loc.y = 6;
   name = TTF_RenderText_Solid(font, status->player->name, textColor);
   if (!name)
   {
      fprintf(stderr, "Error rendering name\n");
   }
   SDL_BlitSurface(name, NULL, status->surface, &loc);
   SDL_FreeSurface(name);

   // remaining money
   char tempText[64];
   sprintf(tempText, "$%i.00", status->player->money);
   money = TTF_RenderText_Solid(font, tempText, textColor);
   loc.y += 26;
   if (money)
   {
      SDL_BlitSurface(money, NULL, status->surface, &loc);
      SDL_FreeSurface(money);
   }

   // player's token - right aligned
   loc.x = status->surface->w - image[status->player->token]->surface->w; 
   loc.y = 0;
   SDL_BlitSurface(image[status->player->token]->surface, NULL, status->surface, &loc);

   // draw the list of properties owned by the current player
   loc.x = 10;
   loc.y = 55;
   //for (int prop=0; prop < NUM_PROPERTIES; prop++)
   property *p = status->player->GetFirstProperty();
   while (p)
   {
      prop_name = TTF_RenderText_Solid(font, p->m_name.c_str(), textColor);
      SDL_BlitSurface(prop_name, NULL, status->surface, &loc);
      SDL_FreeSurface(money);
      loc.y += 20;
      p = status->player->GetNextProperty();
   }

   // don't remember what this was for
   //SDL_BlitSurface(status->surface, NULL, i->surface, NULL);
}

int CSDLGameUI::CreateStatusArea(const SDL_Rect* loc)
{
   // if it doesn't exist yet, create it
   if (!status)
   {
      status = (struct StatusArea*)malloc(sizeof(struct StatusArea));
      if (!status)
         return 1;

      // create the surface
      status->surface = SDL_CreateRGBSurface(SDL_SWSURFACE, loc->w, loc->h, 32, 0, 0, 0, 0);
      if (!status->surface)
      {
         fprintf(stderr, "Error creating status area surface\n");
         free(status);
         status = NULL;
         return 2;
      }

      // create the ifel
      status->ifel = (struct Ifel*)malloc(sizeof(struct Ifel));
      if (!status->ifel)
      {
         free(status);
         SDL_FreeSurface(status->surface);
         return 3;
      }
      status->ifel->loc.x = loc->x;
      status->ifel->loc.y = loc->y;
      status->ifel->loc.w = loc->w;
      status->ifel->loc.h = loc->h;
      CreateIfel(ID_MENU_OPTIONS, NULL, status->ifel, IFEL_CUSTOM, DrawStatusCB);
      status->ifel->data = status;
   }

   // set it active
   status->ifel->active = 1;

   return 0;
}

void CSDLGameUI::DeleteStatusArea(void)
{
   if (status)
   {
      DeleteIfel(NULL, status->ifel);
      free(status->ifel);
      free(status);
      status = NULL;
   }
}

void CSDLGameUI::SetCurrentPlayerStatusArea(Player* p)
{
}

void CSDLGameUI::StatusAreaSetPlayerIconList(Image** l, int count)
{
}

void CSDLGameUI::EnableStatusArea(void)
{
}


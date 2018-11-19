#include "I2C_LCD.h"

#ifndef GUI_STORAGE_PTR
  #define GUI_STORAGE_PTR PROGMEM
#endif


const unsigned char GUI_STORAGE_PTR acCarte[] = {
  _XXXXXXX, XXXXXXXX, XXXXXXX_,
  XX______, X______X, ______XX,
  X_XX____, _X____X_, ____XX_X,
  X___XX__, _X____X_, __XX___X,
  X_____XX, XXXXXXXX, XX_____X,
  X______X, XX____XX, X______X,
  X______X, ___XX___, X______X,
  X______X, ___XX___, X______X,
  X______X, XX____XX, X______X,
  X_____XX, XXXXXXXX, XX_____X,
  X___XX__, _X____X_, __XX___X,
  X_XX____, _X____X_, ____XX_X,
  XX______, X______X, ______XX,
  _XXXXXXX, XXXXXXXX, XXXXXXX_
};

GUI_Bitmap_t bmCarte = {
  24, /* XSize */
  14, /* YSize */
  3, /* BytesPerLine */
  1, /* BitsPerPixel */
  acCarte,
};

/* *** End of file *** */

// *****************************************************************************
// INFO-H-304 2017-2018
// CORNIL Bruno
// *****************************************************************************
#ifndef CGRAPHIC_H
#define CGRAPHIC_H
#include "cGame.h"

#define WINDOWWIDTH  600
#define WINDOWHEIGHT 600
#define TOKENRADIUS  4.0
#define TOKENEDGES   10

typedef struct {
   int r;
   int g;
   int b;
} tColor;

typedef struct {
   float x;
   float y;
} tPoint;

void graphicInit(int argc, char *argv[], cGame *game);
void graphicMainLoop();

#endif


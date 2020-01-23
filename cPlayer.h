// *****************************************************************************
// INFO-H-304 2017-2018
// CORNIL Bruno
// *****************************************************************************
#ifndef CPLAYER_H
#define CPLAYER_H
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include "cOthelloBoard.h"

#ifndef MAX_IA_LEVEL
#define MAX_IA_LEVEL 6
#endif

using namespace std;

class cPlayer {
   private:
      char type;
      char color;
      bool isMouseEvent;
      cPosition mousePosition;
      map<char,string> name;
      bool   _isMoving;
      cPosition  currentPosition;
      cPosition  lastPosition;
      ofstream outputFile;
      ifstream inputFile;
#ifdef PROFILE
      int nNodes;
#endif

      cPosition string2Coord(string &choice);
      int AlphaBeta(cPosition &bestMove, int level, cOthelloBoard &board, int playerColor, bool isNodeMax, int alpha, int beta);
      void getIAInput(cOthelloBoard &board, int &row, int &column);

   public:
      cPlayer(char color);
      void   setMoving(bool state);
      bool   isMoving();
      char   getColor();
      string getName();
      string getLastMoveString();
      void   notifyOpponentRowColumn(cPosition &position);
      void   getRowColumn(cOthelloBoard &board, cPosition &position);
      void   mouseEventCallback(cPosition &position);
};

#endif

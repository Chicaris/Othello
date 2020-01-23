// *****************************************************************************
// INFO-H-304 2017-2018
// CORNIL Bruno
// *****************************************************************************
#ifndef CGAME_H
#define CGAME_H
#include <string>
#include "cOthelloBoard.h"
#include "cPlayer.h"
#include "cPosition.h"

class cGame {
   private:
      cOthelloBoard board;
      cPlayer *players[2];
      int     previousPlayer;
      int     currentPlayer;

   public:
      cGame();
      void mouseEventCallback(cPosition &position);
      char getBoardContent(cPosition &position);
      void getStatusMessage(string &message);
      bool isGameOver();
      void play();
      void displayScore();
      void mainLoop();
};
#endif

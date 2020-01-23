// *****************************************************************************
// INFO-H-304 2017-2018
// CORNIL Bruno
// *****************************************************************************
#ifndef CBOARD_H
#define CBOARD_H
#include <vector>
#include <deque>
#include <string>
#include "cPosition.h"

#define BOARDSIZE 8
#define B_EMPTY '.'
#define B_WHITE 'O'
#define B_BLACK 'X'

using namespace std;

class cOthelloBoard {
   private:
      vector<cPosition>  directions;
      deque<string> statusMessage;

      void defaultInit();
   public:
      vector<vector<char> > board;

      cOthelloBoard();
      cOthelloBoard(const cOthelloBoard &board);
      char get(cPosition &position);
      void set(cPosition &position, char content);
      bool isWithinBoard(cPosition move);
      bool isMovePossible(int color );
      bool isMoveValid(int color, cPosition move);
      bool isMoveValidDirection(int color, cPosition move, cPosition dir);
      bool move(int color, cPosition move);
      void display();
      void setStatusMessage(string &message);
      void getStatusMessage(string &message);
      void getScore(int &nBlack, int &nWhite);
      int  getMetric(int color);
};

#endif

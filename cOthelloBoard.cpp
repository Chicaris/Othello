// *****************************************************************************
// INFO-H-304 2017-2018
// CORNIL Bruno
// *****************************************************************************
#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include "cOthelloBoard.h"

using namespace std;

// -----------------------------------------------------------------------------
// cOthelloBoard
// - Class constructor
// - Create board and users
// -----------------------------------------------------------------------------
cOthelloBoard::cOthelloBoard(){
   defaultInit();
}

// -----------------------------------------------------------------------------
// cOthelloBoard
// - Class constructor 
// - Copy board and users from supplied parameter
// -----------------------------------------------------------------------------
cOthelloBoard::cOthelloBoard(const cOthelloBoard &initData){

   defaultInit();

   for (int i=0;i<BOARDSIZE;i++) {
      for (int j=0;j<BOARDSIZE;j++) {
         board[i][j]=initData.board[i][j];
      }
   }
}

// -----------------------------------------------------------------------------
// cOthelloBoard
// - Class constructor
// - Create board and users
// -----------------------------------------------------------------------------
void cOthelloBoard::defaultInit(){

   // Init. default board
   board.clear();

   for (int i=0;i<BOARDSIZE;i++) {
      board.push_back(vector<char>(BOARDSIZE,B_EMPTY));
   }
   board[BOARDSIZE/2 -1][BOARDSIZE/2 -1]=B_WHITE;
   board[BOARDSIZE/2 -1][BOARDSIZE/2   ]=B_BLACK;
   board[BOARDSIZE/2   ][BOARDSIZE/2 -1]=B_BLACK;
   board[BOARDSIZE/2   ][BOARDSIZE/2   ]=B_WHITE;

   // Init. valid move directions
   for (int i=-1;i<2;i++) {
      for (int j=-1;j<2;j++) {
         if (i!=0 || j!=0) {
            directions.push_back(cPosition(i,j));
         }
      }
   }
}

// -----------------------------------------------------------------------------
// get:
// - Get content of a board position
// -----------------------------------------------------------------------------
char cOthelloBoard::get(cPosition &position) {
   return board[position.row][position.column];
}

// -----------------------------------------------------------------------------
// set:
// - Set content of a board position
// -----------------------------------------------------------------------------
void cOthelloBoard::set(cPosition &position, char content) {
   board[position.row][position.column]=content;
}

// -----------------------------------------------------------------------------
// isWithinBoard:
// - Check if row,colum coordinates are in the board
// -----------------------------------------------------------------------------
bool cOthelloBoard::isWithinBoard(cPosition move) {
   return move.row>=0 && move.row<BOARDSIZE && move.column>=0 && move.column<BOARDSIZE;
}

// -----------------------------------------------------------------------------
// isMoveValidDirection
// - Check if a move if valid for the current user in a given direction
// -----------------------------------------------------------------------------
bool cOthelloBoard::isMoveValidDirection(int color, cPosition position, cPosition moveDirection) {

   // Current neighboor position
   cPosition currentPosition = position + moveDirection;

   bool isDirectionValid = false;
   if (isWithinBoard(currentPosition)) {
      char cell=get(currentPosition);
      // Check if the neighboor has a different color
      if (cell != B_EMPTY && cell != color) {
         // Check if all cells in the given direction are within the board, not empty and a different color
         do {
            currentPosition = currentPosition + moveDirection;
            cell=B_EMPTY;
            if (isWithinBoard(currentPosition)) {
               cell=get(currentPosition);
            }
         } while (cell != B_EMPTY && cell != color);

         // Check if end cell is from the same color
         if (cell == color) {
            isDirectionValid = true;
         }
      }
   }
   return isDirectionValid;
}

// -----------------------------------------------------------------------------
// isMoveValid
// - Check if a move if valid for the current user
// -----------------------------------------------------------------------------
bool cOthelloBoard::isMoveValid(int color, cPosition move) {

   // Make sure position is withing the board and empty
   bool isMoveValid = false;
   if (isWithinBoard(move) && get(move)==B_EMPTY) {
      // Try in all possible directions
      for (vector<cPosition>::iterator direction=directions.begin(); direction!=directions.end();++direction) {
         // until one valid direction is found
         if (isMoveValidDirection(color, move, *direction)) {
            isMoveValid = true;
            break;
         }
      }
   }
   return isMoveValid;
}

// -----------------------------------------------------------------------------
// isMovePossible:
// - Check if a move is possible for ther current user for at least one board
//   position
// -----------------------------------------------------------------------------
bool cOthelloBoard::isMovePossible(int color) {
   bool isValidMove = false;

   // Check the whole board
   for (int i=0;i<BOARDSIZE && !isValidMove;i++) {
      for (int j=0;j<BOARDSIZE && !isValidMove;j++) {
         isValidMove = isMoveValid(color, cPosition(i,j));
      }
   }
   return isValidMove;
}

// -----------------------------------------------------------------------------
// move:
// - If valid, implement the board move for the current user by toggling token
//   in all possible directions
// -----------------------------------------------------------------------------
bool cOthelloBoard::move(int color, cPosition move) {
   string msg;
   
   bool isValid = isMoveValid(color, move);
   if (isValid) {
      // Put new token
      set(move,color);
      // Check for all possible directions
      for (vector<cPosition>::iterator direction=directions.begin(); direction!=directions.end();++direction) {
         if (isMoveValidDirection(color, move, *direction)) {
            // Toggle all tokens in one direction
            cPosition currentMove = move + *direction;
            while (get(currentMove) != color) {
               set(currentMove, color);
               currentMove = currentMove + *direction;
            }
         }
      }
   } else {
      msg = "Position invalide !\r\n";
      setStatusMessage(msg);
   }

   return isValid;
}

// -----------------------------------------------------------------------------
// display:
// - Display board
// -----------------------------------------------------------------------------
void cOthelloBoard::display(){

   // Display board
   string header;
   header = " ";
   for (int i=0;i<BOARDSIZE;i++) {
      header += (char)('a'+i);
   }
   cout << header << endl;
   for (int i=0;i<BOARDSIZE;i++) {
      cout << i+1;
      for (int j=0;j<BOARDSIZE;j++) {
         cout << board[i][j]; 
      }
      cout << i+1 << endl;
   }
   cout << header << endl;
}

// -----------------------------------------------------------------------------
// setStatusMessage:
// - Display status
// -----------------------------------------------------------------------------
void cOthelloBoard::setStatusMessage(string &message){
   statusMessage.push_back(message);
#ifndef GRAPH
   cout << message << flush;
#endif
}

// -----------------------------------------------------------------------------
// getStatusMessage:
// - return status message if any
// -----------------------------------------------------------------------------
void cOthelloBoard::getStatusMessage(string &message){
   if (! statusMessage.empty()){
      message = statusMessage.front();
      statusMessage.pop_front();
   }
}

// -----------------------------------------------------------------------------
// getScore:
// - Compute #of black and white token
// -----------------------------------------------------------------------------
void cOthelloBoard::getScore(int &nBlack, int &nWhite) {
   nBlack = 0;
   nWhite = 0;
   for (int i=0;i<BOARDSIZE;i++) {
      for (int j=0;j<BOARDSIZE;j++) {
         switch(board[i][j]) {
            case B_BLACK:
               nBlack++;
               break;
            case B_WHITE:
               nWhite++;
               break;
         }
      }
   }
}

// -----------------------------------------------------------------------------
// getMetric:
// - Compute of token for color
// -----------------------------------------------------------------------------
int  cOthelloBoard::getMetric(int color){
   int metric = 0;
   for (int i=0;i<BOARDSIZE;i++) {
      for (int j=0;j<BOARDSIZE;j++) {
         if (isMoveValid(color, cPosition(i,j))) {
            metric++;
         }
      }
   }
   return metric;
}

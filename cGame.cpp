// *****************************************************************************
// INFO-H-304 2017-2018
// CORNIL Bruno
// *****************************************************************************
#include <limits>
#include "cPosition.h"
#include "cOthelloBoard.h"
#include "cPlayer.h"
#include "cGame.h"

using namespace std;

// -----------------------------------------------------------------------------
// cGame
// - Class constructor
// - Create board and users
// -----------------------------------------------------------------------------
cGame::cGame(){

   board = cOthelloBoard();

   // Create both players
   //players.push_back(cPlayer(B_BLACK));
   //players.push_back(cPlayer(B_WHITE));
   // FIXME declaring <vector> for players doesn't work/compile issue ...
   players[0]=new cPlayer(B_BLACK);
   players[1]=new cPlayer(B_WHITE);

   // Use -1 to detect game start in display fct
   previousPlayer= -1;
   currentPlayer = 0;
}

// -----------------------------------------------------------------------------
// mouseEventCallback:
// - Handle mouse event from graphic interface
// -----------------------------------------------------------------------------

void cGame::mouseEventCallback(cPosition &position) {
  players[currentPlayer]->mouseEventCallback(position);
}

// -----------------------------------------------------------------------------
// getBoardContent:
// - Return board content for given position
// -----------------------------------------------------------------------------

char cGame::getBoardContent(cPosition &position) {
  return board.get(position);
}

// -----------------------------------------------------------------------------
// getStatusMessage:
// - Return board status Message
// -----------------------------------------------------------------------------

void cGame::getStatusMessage(string &statusMessage) {
  board.getStatusMessage(statusMessage);
}

// -----------------------------------------------------------------------------
// isGameOver:
// - Return true is both players can't move anymore
// -----------------------------------------------------------------------------

bool cGame::isGameOver() {
//   return !(players[0]->isMoving() || players[1]->isMoving());
   return !(board.isMovePossible(B_BLACK) || board.isMovePossible(B_WHITE));
}

// -----------------------------------------------------------------------------
// play:
// - Display last move and current board
// - Ask current player to move if possible
// -----------------------------------------------------------------------------
void cGame::play() {
   string statusMessage;

   // Summary of previous action
   if (previousPlayer < 0) {
      statusMessage="Debut de la partie";
   }  else {
      statusMessage="Le joueur " + players[previousPlayer]->getName();
      if (players[previousPlayer]->isMoving()) {
         statusMessage+=" a place son pion en " + players[previousPlayer]->getLastMoveString();
      } else {
         statusMessage+=" a passe son tour";
      }
   }
   board.setStatusMessage(statusMessage);

#ifndef GRAPH
   // Wait for user to hit <enter> to continue
   cout << " ... <enter> pour continuer" << endl;
   string choice;
   getline(cin, choice);

   board.display();
#endif

   cPosition position;
   int color=players[currentPlayer]->getColor();
   players[currentPlayer]->setMoving(false);
   if (board.isMovePossible(color)) { 
      players[currentPlayer]->setMoving(true);
      // Move only upon reception of a valid user input
      do {
         players[currentPlayer]->getRowColumn(board,position);
      } while (! board.move(color, position));
      // Tell next play (required for FILE type)
      players[(currentPlayer+1)%2]->notifyOpponentRowColumn(position);
   }

   // Next player
   previousPlayer = currentPlayer;
   currentPlayer = (currentPlayer+1)%2;
}

// -----------------------------------------------------------------------------
// displayScore:
// - Count and report number of token for each player
// - Report Winner
// -----------------------------------------------------------------------------
void cGame::displayScore(){
   int nBlack, nWhite;

   board.getScore(nBlack, nWhite);

   cout << "Scores: " << players[0]->getName() << "=" << nBlack << ", " << players[1]->getName() << "=" << nWhite << endl;
   if (nBlack == nWhite) {
      cout << "Match Null !";
   } else {
      cout << "Le gagnant est le joueur [";
      if (nBlack > nWhite) {
         cout << players[0]->getName(); 
      } else {
         cout << players[1]->getName(); 
      }
   }
   cout << "]" << endl;
}

// -----------------------------------------------------------------------------
// mainLoop
// - Loop until gameover & display score
// -----------------------------------------------------------------------------
void cGame::mainLoop(){
   while (! isGameOver()) {
      play();
   };
   displayScore();
}

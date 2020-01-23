// *****************************************************************************
// INFO-H-304 2017-2018
// CORNIL Bruno
// *****************************************************************************
#include <iostream>
#include <string>
#include <map>
#include <chrono>
#include <thread>
#include <climits>
#include <ctime>
#include <ctype.h>
#include "cOthelloBoard.h"
#include "cPlayer.h"

using namespace std;

// -----------------------------------------------------------------------------
// cPlayer
// - Constructor
// -----------------------------------------------------------------------------
cPlayer::cPlayer(char color) {
   this->color = color;
   isMouseEvent = false;

   name[B_WHITE] = "blanc";
   name[B_BLACK] = "noir";
   _isMoving = true;
   lastPosition = cPosition();

   bool isChoiceOK = false;
   string choice;
   while (! isChoiceOK) {
      cout << "Joueur " << name[color] << endl;
      cout << " - [H] Humain" << endl;
      cout << " - [A] Intelligence Artificielle" << endl;
      cout << " - [F] Fichier" << endl;
      cout << "Choix ? ";
      getline(cin, choice);
      type = toupper(choice[0]);

      string inputFileName;
      string outputFileName;
      isChoiceOK = true;
      switch (type) {
         case 'H':
            break;
         case 'A':
            break;
         case 'F':
            do {
               cout << "Chemin du dossier de travail ? ";
               getline(cin, choice);
               switch (color) {
                  case B_WHITE:
                     inputFileName =choice+"/"+name[B_WHITE]+".txt";
                     outputFileName=choice+"/"+name[B_BLACK]+".txt";
                     break;
                  case B_BLACK:
                     inputFileName =choice+"/"+name[B_BLACK]+".txt";
                     outputFileName=choice+"/"+name[B_WHITE]+".txt";
                     break;
               }
               outputFile.open(outputFileName.c_str());
               inputFile.open(inputFileName.c_str());
            } while (!(outputFile.is_open() && inputFile.is_open()));
            break;
         default:
            cout << "[ERREUR] Choix incorrect !" << endl;
            isChoiceOK = false;
      }
   }
}

// -----------------------------------------------------------------------------
// mouseEventCallback:
// - Handle mouse event from graphic interface
// -----------------------------------------------------------------------------

void cPlayer::mouseEventCallback(cPosition &position) {
   if (!isMouseEvent) {
      mousePosition = position;
      isMouseEvent = true;
   }
}

// -----------------------------------------------------------------------------
// setMoving:
// - set move state (false = was not able to move)
// -----------------------------------------------------------------------------
void cPlayer::setMoving(bool state) {
   _isMoving=state;
}

// -----------------------------------------------------------------------------
// isMoving:
// - Return move state (false = was not able to move)
// -----------------------------------------------------------------------------
bool cPlayer::isMoving() {
   return _isMoving;
}

// -----------------------------------------------------------------------------
// getColor:
// - Return user color
// -----------------------------------------------------------------------------
char cPlayer::getColor() {
   return color;
}

// -----------------------------------------------------------------------------
// getName:
// - Return user name
// -----------------------------------------------------------------------------

string cPlayer::getName() {
   return name[color];
}

// -----------------------------------------------------------------------------
// getLastMoveString:
// - Return last move in a string format
// -----------------------------------------------------------------------------
string cPlayer::getLastMoveString() {
   string position;
   position += (char)('a'+lastPosition.column);
   position += (char)('1'+lastPosition.row);
   return position;
}

// -----------------------------------------------------------------------------
// opponentRowColumn:
// - receive information about opponent move (only useful for FILE type)
// -----------------------------------------------------------------------------
void cPlayer::notifyOpponentRowColumn(cPosition &position) {
   string move;
   move += ('a'+position.column);
   move += ('1'+position.row);
   switch (type) {
      case 'F':
         outputFile << move << endl;
         break;
   }
}

// -----------------------------------------------------------------------------
// string2Coord:
// - Convert a string input (2 characters) into a cPosition
//   Row and column expected to start with 1 and A 
// -----------------------------------------------------------------------------
cPosition cPlayer::string2Coord(string &choice) {
   cPosition move;
   move.column = (int) (toupper(choice[0])-'A');
   move.row    = (int) (choice[1]-'1');
   return move;
}

// -----------------------------------------------------------------------------
// AlphaBeta:
// - Compute AI input based on 'AlphaBeta' algorithm
// - References
//   * http://pageperso.lif.univ-mrs.fr/~liva.ralaivola/lib/exe/fetch.php?id=teaching%3A20122013%3Aprojetalgo&cache=cache&media=teaching:20122013:minmax.pdf 
//   * https://fr.wikipedia.org/wiki/%C3%89lagage_alpha-b%C3%AAta
// -----------------------------------------------------------------------------
int cPlayer::AlphaBeta(cPosition &bestMove, int level, cOthelloBoard &board, int playerColor, bool isNodeMax, int alpha, int beta) {
   cOthelloBoard childBoard;
   cPosition currentMove;
   cPosition returnMove; // Unused, required for AlphaBeta call
   int currentScore, bestScore;

#ifdef PROFILE
         nNodes++;
#endif
   int nextPlayerColor = B_BLACK;
   if (playerColor == B_BLACK) {
      nextPlayerColor = B_WHITE;
   }
#ifdef DEBUG
   cout << ">>>>>" << endl;
   cout << "L" << level << "[" << (char) playerColor << "," << isNodeMax << "] (alpha,beta) = (" << alpha << "," << beta << "), bestMove = (" << bestMove.row << "," << bestMove.column << ")" << endl;
   string dummy;
   getline(cin, dummy);
#endif
   if (level == 0) {
      // max. depth reach, return board metric for user color
      bestScore=board.getMetric(color);
#ifdef DEBUG
      cout << "Leaf score = " << bestScore << endl;
      cout << ">>>>>" << endl;
#endif
   } else {
      if (board.isMovePossible(playerColor)) { 
      // Childs exist => search valid ones
         bestScore = (isNodeMax ? INT_MIN: INT_MAX);
         for (int i=0;i<BOARDSIZE;i++) {
            for (int j=0;j<BOARDSIZE;j++) {
               currentMove = cPosition(i,j);
               if (board.isMoveValid(playerColor, currentMove)) { 
#ifdef DEBUG
                  cout << "Move -> (" << currentMove.row << "," << currentMove.column << ")" << endl;
#endif
                  // Create a copy of the board and move
                  childBoard = cOthelloBoard(board);
                  childBoard.move(playerColor, currentMove);
                  // Compute next level
                  if (isNodeMax) {
                     // IA goal is to try to maximize his score
                     currentScore = AlphaBeta(returnMove, level-1, childBoard, nextPlayerColor, !isNodeMax, alpha, beta);
                     if (currentScore > bestScore) {
                        bestScore = currentScore;
                        bestMove  = currentMove;
#ifndef MINMAX
                        if (bestScore > beta) { // beta break (higher that a previous lower score -> opponent won't select anyway, no point to continue)
                           return bestScore;
                        }
#endif
                     }
                     alpha = max(currentScore, alpha);
                  } else {
                     // Opponent goal is to try to minimize IA score
                     currentScore = AlphaBeta(returnMove, level-1, childBoard, nextPlayerColor, !isNodeMax, alpha, beta);
                     if (currentScore < bestScore) {
                        bestScore = currentScore;
                        bestMove  = currentMove;
#ifndef MINMAX
                        if (bestScore < alpha) { // alpha break (lower than a previous score -> IA won't select anyway, no point to continue)
                           return bestScore;
                        }
#endif
                     }
                     beta = min(currentScore, beta);
                  }
#ifdef DEBUG
                  cout << "currentScore=" << currentScore << " (bestScore = " << bestScore << ")" << endl;
#endif
               }
            }
         }
      } else {
      // Not childs => pass and move to the next player
         childBoard = cOthelloBoard(board);
         bestScore = AlphaBeta(bestMove, level-1, childBoard, nextPlayerColor, !isNodeMax, alpha, beta);
      }
   }
   return bestScore;
}

// -----------------------------------------------------------------------------
// getRowColumn:
// - Fetch user input
// -----------------------------------------------------------------------------
void cPlayer::getRowColumn(cOthelloBoard &board, cPosition &position) {
   string statusMessage;
   string choice;
   clock_t tic, toc;
   switch (type) {
      case 'A':
         statusMessage="IA "+name[color]+" calcule ... ";
         board.setStatusMessage(statusMessage);
         tic = clock();
#ifdef PROFILE
         nNodes=0;
#endif
         AlphaBeta(position, MAX_IA_LEVEL, board, color, true, INT_MIN, INT_MAX);

         toc = clock();
         statusMessage=to_string(double(toc - tic) / CLOCKS_PER_SEC)+" secondes\n";
         board.setStatusMessage(statusMessage);
#ifdef PROFILE
         cout << "nNode:" << nNodes << endl;
#endif
         break;

      case 'F':
         statusMessage="En attente du joueur "+name[color]+" ...\n";
         board.setStatusMessage(statusMessage);
         while (!getline(inputFile, choice)) {
            inputFile.clear();
            this_thread::sleep_for(std::chrono::milliseconds(1000));
         }
         position=string2Coord(choice);
         break;

      default: // 'H'
         statusMessage="Mouvement "+name[color]+" ? :";
         board.setStatusMessage(statusMessage);
#ifdef GRAPH
         while (! isMouseEvent);
         position = mousePosition;
         isMouseEvent = false;
#else
         getline(cin, choice);
         position=string2Coord(choice);
#endif
   }
   lastPosition = position;
}


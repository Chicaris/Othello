// *****************************************************************************
// INFO-H-304 2017-2018
// CORNIL Bruno
// *****************************************************************************
#include "cGame.h"
#ifdef GRAPH
#include <thread>
#include <GL/glut.h>
#include "graphic.h"
#endif

int main(int argc, char *argv[]) {

   cGame othello;

#ifdef GRAPH
   // GLUT has to be the main thread ... -> start game in a new one
   thread game(&cGame::mainLoop, &othello);
   
   graphicInit(argc, argv, &othello);
   graphicMainLoop();

   game.join();
#else
   othello.mainLoop();
#endif
}

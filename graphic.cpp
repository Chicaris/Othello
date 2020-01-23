// *****************************************************************************
// INFO-H-304 2017-2018
// CORNIL Bruno
// *****************************************************************************
#include <GL/glut.h>
#include <vector>
#include "math.h"
#include "graphic.h"
#include "cGame.h"
#include "cOthelloBoard.h"

// Private variables & functions

cGame *pGame;
vector<tPoint> token;
vector<string> statusMessage(2);

void createToken() {
   tPoint point;
   token.clear();
   for(int i = 0; i <=TOKENEDGES; i ++) {
      float theta = 2.0f * M_PI * float(i)/TOKENEDGES;
      point.x = TOKENRADIUS * cosf(theta);
      point.y = TOKENRADIUS * sinf(theta);

      token.push_back(point);
   }
}

void drawToken(int row, int column, tColor color){

   float xCenter=15.0+column*10.0;
   float yCenter=15.0+row*10.0;

   glColor3ub(color.r,color.g,color.b);
   glBegin(GL_TRIANGLE_FAN);
   for(vector<tPoint>::iterator it=token.begin(); it!=token.end(); ++it) {
      glVertex2f(xCenter+(*it).x, yCenter+(*it).y);
   } 
   glEnd();
}

void displayString(string message, int x, int y) {
   int i=0;
   for(string::iterator character=message.begin(); character!= message.end(); ++character) {
      glRasterPos2i(x+2*i++,y);
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *character);
   }
}

// Callback functions

void displayCallback(void) {

   tColor white_token = {200,200,200};
   tColor black_token = {0,0,0};

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glViewport(0, 0, WINDOWWIDTH, WINDOWHEIGHT);

   // Display Row and Column titles
   char rowHeader='1';
   char columHeader='a';

   glColor3ub(255, 255, 255);
   for(int i = 0; i < BOARDSIZE; i ++) {
      glRasterPos2i(15+i*10,5);
      glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, (char(columHeader+i)));
      glRasterPos2i(5,15+i*10);
      glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, (char(rowHeader+i)));
   }

   // Display status (previous & current message)
   string message;
   pGame->getStatusMessage(message);
   if (! message.empty()) {
      statusMessage[0]=statusMessage[1];
      statusMessage[1]=message;
   }
   glColor3ub(0, 128, 0);
   displayString(statusMessage[0], 5, 95);
   glColor3ub(0, 255, 0);
   displayString(statusMessage[1], 5, 98);

   // Display board & grid
   glColor3ub(171, 149, 132);
   glBegin(GL_POLYGON);
   glVertex2i(10, 10);
   glVertex2i(90, 10);
   glVertex2i(90, 90);
   glVertex2i(10, 90);
   glEnd();

   glColor3ub(87, 65, 47);
   for(int i = 0; i <=8; i ++) {
      if (i==0 || i==8) {
         glLineWidth(4.0f);

      } else {
         glLineWidth(2.0f);
      }
      glBegin(GL_LINES);
      glVertex3i(i*10+10, 10,      0);
      glVertex3i(i*10+10, 90,      0);
      glVertex3i(10,      i*10+10, 0);
      glVertex3i(90,      i*10+10, 0);
      glEnd();
   }

   // Display Token
   for (int i=0;i<BOARDSIZE;i++) {
      for (int j=0;j<BOARDSIZE;j++) {
         cPosition position(i,j);
         char item=pGame->getBoardContent(position);
         switch (item) {
            case B_WHITE:
               drawToken(i,j,white_token);
               break;
            case B_BLACK:
               drawToken(i,j,black_token);
               break;
            default:
               break;
         }
      }
   }
   glutSwapBuffers();
}

void mouseButtonCallback(int button, int state, int x, int y) {
   if (button == 0 && state == 1) { // Left button released
      int column = (x - 60) / 60;
      int row    = (y - 60) / 60;
      //cout << "(" << column << "," << row << ")" << endl;
      cPosition mousePosition(row, column);
      pGame->mouseEventCallback(mousePosition);
   }
}

void timerCallback(int value) {
   displayCallback();
   glutTimerFunc(0.1, timerCallback, 0);
}

// Public functions

void graphicInit(int argc, char *argv[], cGame *game) {
   pGame = game;
   glutInit(&argc, argv);

   createToken();

   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
   glutInitWindowSize(WINDOWWIDTH, WINDOWHEIGHT);
   glutCreateWindow("Othello");

   glutDisplayFunc(displayCallback);
   glutMouseFunc(mouseButtonCallback);
   glutTimerFunc(0.1, timerCallback, 0);

   glClearColor(0.0,0.0,0.0,0.0);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity();
   gluOrtho2D (0.0 , 100, 100.0, 0);
}


void graphicMainLoop() {
   glutMainLoop();
}


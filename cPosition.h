// *****************************************************************************
// INFO-H-304 2017-2018
// CORNIL Bruno
// *****************************************************************************
#ifndef CMOVE_H
#define CMOVE_H

class cPosition {
public:
         int row;
         int column;

         cPosition();
         cPosition(int row, int column);
         cPosition operator+(const cPosition &operand);
      };

#endif

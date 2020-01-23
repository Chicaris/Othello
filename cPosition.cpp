// *****************************************************************************
// INFO-H-304 2017-2018
// CORNIL Bruno
// *****************************************************************************
#include "cPosition.h"

// -----------------------------------------------------------------------------
// cPosition
// - Class constructor
// -----------------------------------------------------------------------------
cPosition::cPosition(){
   row=0;
   column=0;
}

cPosition::cPosition(int row, int column){
   this->row   =row;
   this->column=column;
}

cPosition cPosition::operator+(const cPosition &operand){
   cPosition move;
   move.row    = this->row    + operand.row;
   move.column = this->column + operand.column;
   return move;
}

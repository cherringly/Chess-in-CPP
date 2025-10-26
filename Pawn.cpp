/*
Team Members:

Name: Mary Zhang
JHED: mzhan188

Name: Megan Weng
JHED: mweng5

Name: Jiaxing Brisbois
JHED: jbrisbo1
*/

#include "Pawn.h"
#include <cmath>

namespace Chess
{
  bool Pawn::legal_move_shape(const Position& start, const Position& end) const {
    //check if start and end are within A-H and 1-8
    if(start.first < 'A' || start.first > 'H' || start.second < '1' || start.second > '8' 
      || end.first < 'A' || end.first > 'H' || end.second < '1' || end.second > '8') {
      return false;
    }

    int col_diff = end.first - start.first;
    int row_diff = end.second - start.second;

    // Pawns must stay in the same column for non-capture moves
    if (col_diff != 0) {
        return false;
    }

    // White pawns move upward (increasing row), black pawns move downward
    if (is_white()) {
        // Can move 1 or 2 squares forward from starting position (row '2')
        if (start.second == '2') {
            return (row_diff == 1 || row_diff == 2);
        }
        // Otherwise can only move 1 square
        return (row_diff == 1);
    } else {
        // Black pawns start at row '7' and move downward
        if (start.second == '7') {
            return (row_diff == -1 || row_diff == -2);
        }
        // Otherwise can only move 1 square
        return (row_diff == -1);
    }
  }
    
  bool Pawn::legal_capture_shape(const Position& start, const Position& end) const {
    int col_diff = abs(end.first - start.first);
    int row_diff = end.second - start.second;

    // Pawns capture diagonally forward by 1 square
    if (col_diff != 1) {
        return false;
    }

    // White pawns capture upward (+1 row), black pawns capture downward (-1 row)
    if(is_white())  {return (row_diff == 1);}
    else  {return (row_diff == -1);}
  }

}

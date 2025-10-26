/*
Team Members:

Name: Mary Zhang
JHED: mzhan188

Name: Megan Weng
JHED: mweng5

Name: Jiaxing Brisbois
JHED: jbrisbo1
*/

#include "Rook.h"

namespace Chess
{
  bool Rook::legal_move_shape(const Position& start, const Position& end) const {
    //check if start and end are within A-H and 1-8
    if(start.first < 'A' || start.first > 'H' || start.second < '1' || start.second > '8' 
      || end.first < 'A' || end.first > 'H' || end.second < '1' || end.second > '8') {
      return false;
    }

    // calculate the difference in x and y coordinates
    int dx = std::abs(end.first - start.first);
    int dy = std::abs(end.second - start.second);

    // return if moved in a straight line (either horizontally or vertically)
    return (dx == 0 && dy > 0) || (dy == 0 && dx > 0);
  }
}

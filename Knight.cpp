/*
Team Members:

Name: Mary Zhang
JHED: mzhan188

Name: Megan Weng
JHED: mweng5

Name: Jiaxing Brisbois
JHED: jbrisbo1
*/

#include "Knight.h"
#include <cmath>

namespace Chess
{
  bool Knight::legal_move_shape(const Position& start, const Position& end) const {
    //check if start and end are within A-H and 1-8
    if(start.first < 'A' || start.first > 'H' || start.second < '1' || start.second > '8' 
      || end.first < 'A' || end.first > 'H' || end.second < '1' || end.second > '8') {
      return false;
    }

    // calculate the difference in x and y coordinates
    int dx = std::abs(end.first - start.first);
    int dy = std::abs(end.second - start.second);

    // return if moved in an L shape (2 squares in one direction and 1 square in the other)
    return (dx == 2 && dy == 1) || (dx == 1 && dy == 2);
  }
}

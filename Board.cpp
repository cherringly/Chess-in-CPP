/*
Team Members:

Name: Mary Zhang
JHED: mzhan188

Name: Megan Weng
JHED: mweng5

Name: Jiaxing Brisbois
JHED: jbrisbo1
*/

#include <iostream>
#include <utility>
#include <map>
#ifndef _WIN32
#endif // !_WIN32
#include "Board.h"
#include "CreatePiece.h"
#include "Exceptions.h"

namespace Chess
{
  /////////////////////////////////////
  // DO NOT MODIFY THIS FUNCTION!!!! //
  /////////////////////////////////////
  Board::Board(){}

  // Destructor
  Board::~Board() {
    for (std::map<Position, Piece*>::const_iterator it = occ.cbegin(); it != occ.cend(); ++it) {
      delete it->second;
    }
  }

  // Assignment operator
  Board& Board::operator=(const Board& other) {
    // delete everything on current map
    for (std::map<Position, Piece*>::const_iterator it = occ.cbegin(); it != occ.cend(); ++it) {
    delete it->second;
    }

    // copy everything from the other map to this map
    occ = other.occ;
    for (std::map<Position, Piece*>::const_iterator it = other.occ.cbegin(); it != other.occ.cend(); ++it) {
      Piece* p = create_piece(it->second->to_ascii());
      occ[it->first] = p;
    }
    // return itself
    return *this;
  }

  // Returns a const pointer to the piece at a prescribed location if it exists,
	// or nullptr if there is nothing there.
  const Piece* Board::operator()(const Position& position) const {
    return get_piece(position);
  }

  // Just operator() but it has a name so it can be called
  const Piece* Board::get_piece(const Position& position) const {
    // iterate through the board and when it gets to the position, return the piece
    for (std::map<Position, Piece*>::const_iterator it = occ.cbegin(); it != occ.cend(); ++it) {
      if (it->first == position) {return it->second;}
    }
    // if the position was not found, there's no piece there so return nullptr
    return nullptr;
  }
  
  // Attempts to add a new piece with the specified designator, at the given position.
  // Throw exception for the following cases:
  // -- the designator is invalid, throw exception with error message "invalid designator"
  // -- the specified position is not on the board, throw exception with error message "invalid position"
  // -- if the specified position is occupied, throw exception with error message "position is occupied"
  void Board::add_piece(const Position& position, const char& piece_designator) {
    // make sure position is within A-H and 1-8
    if (position.first >= 'A' && position.first <= 'H' 
      && position.second >= '1' && position.second <= '8') {
      if (occ[position] != nullptr) {
        // there is already a piece in this position, so can't add it
        throw Exception("position is occupied");
      } else {
        // attempt to create and place the piece in that position
        occ[position] = create_piece(piece_designator);
        if (occ[position] == nullptr){
          // piece was not created successfully
          throw Exception("invalid designator");
        }
      }
    } else {
      // position was not between A-H and 1-8
      throw Exception("invalid position");
    }
  }

  // Opposite of add_piece, deallocates memory of piece and removes it from board
  void Board::remove_piece(const Position& position) {
    delete get_piece(position);
    occ.erase(position);
  }

  // Displays the board by printing it to stdout
  void Board::display() const {
    std::cout << "  ABCDEFGH  " << std::endl;
    std::cout << "  --------  " << std::endl;
    for (char i = '8'; i >= '1'; i--) {
      std::cout << i << "|";
      for (char c = 'A'; c <= 'H'; c++) {
        // print out whatever piece is at that position
        const Piece* p = get_piece(Position(c,i));
        if (p != nullptr) {std::cout << p->to_unicode();}
        else {std::cout << " ";}
      }
      std::cout << "|" << i << std::endl;
    }
    std::cout << "  --------  " << std::endl;
    std::cout << "  ABCDEFGH  " << std::endl;
  }

  // Returns true if the board has the right number of kings on it
  bool Board::has_valid_kings() const {
    int white_king_count = 0;
    int black_king_count = 0;
    for (std::map<std::pair<char, char>, Piece*>::const_iterator it = occ.begin();
    it != occ.end();
    it++) {
      if (it->second) {
        switch (it->second->to_ascii()) {
          case 'K':
            white_king_count++;
            break;
          case 'k':
            black_king_count++;
            break;
        }
      }
    }
    return (white_king_count == 1) && (black_king_count == 1);
  }

  // Checks if the path is clear for moving
  bool Board::check_path(const Position& start, const Position& end, char piece_designator) const {
    switch (piece_designator) {
      // the only time the pawn needs to check path is when it moves forward 2 spaces
      case 'P': case 'p': return check_rook_path(start,end);
      // call check bishop path for diagonal
      case 'B': case 'b': return check_bishop_path(start,end);
      // call check rook path for horizontal/vertical
      case 'R': case 'r': return check_rook_path(start,end);
      // call bishop if it's diagonal and call rook if it's horizontal/vertical
      case 'Q': case 'q':
        if (start.first != end.first && start.second != end.second) {return check_bishop_path(start,end);}
        else {return check_rook_path(start,end);}
      // since we don't know how mystery will move, check rook and bishop or just continue
      case 'M': case 'm':
        // if (abs(start.first-end.first) == abs(start.second-end.second)) {return check_bishop_path(start,end);}
        // if (start.first == end.first || start.second == end.second) {return check_rook_path(start,end);}
        // calculate the difference in x and y coordinates
        int dx = std::abs(end.first - start.first);
        int dy = std::abs(end.second - start.second);

        // return if moved like rook (either horizontally, vertically)
        // return if moved like bishop (diagonally)
        // return if moved in an L shape (2 squares in one direction and 1 square in the other)
        bool rook_move = ((dx == 0 && dy > 0) || (dy == 0 && dx > 0));
        bool bishop_move = ((dx == dy && dx > 0));
        bool knight_move = (dx == 2 && dy == 1) || (dx == 1 && dy == 2);

        if(knight_move){
          return true; // Knight moves don't require path checking
        } else if(rook_move) {
          return check_rook_path(start, end);
        } else if(bishop_move) {
          return check_bishop_path(start, end);
        }
    }
    return true;
  }

  // Check diagonal path
  bool Board::check_bishop_path(const Position& start, const Position& end) const {
    if (start.first < end.first && start.second < end.second) { // right up
      for (char c = start.first + 1, r = start.second + 1; c < end.first && r < end.second; c++, r++) {
        if (get_piece(Position(c,r)) != nullptr) {return false;}
      }
    } else if (start.first < end.first && end.second < start.second) { // right down
      for (char c = start.first + 1, r = start.second - 1; c < end.first && r > end.second; c++, r--) {
        if (get_piece(Position(c,r)) != nullptr) {return false;}
      }
    } else if (end.first < start.first && start.second < end.second) { // left up
      for (char c = start.first - 1, r = start.second + 1; c > end.first && r < end.second; c--, r++) {
        if (get_piece(Position(c,r)) != nullptr) {return false;}
      }
    } else if (end.first < start.first && end.second < start.second) { // left down
      for (char c = start.first - 1, r = start.second - 1; c > end.first && r > end.second; c--, r--) {
        if (get_piece(Position(c,r)) != nullptr) {return false;}
      }
    }
    return true;
  }

  // Check horizontal/vertical path
  bool Board::check_rook_path(const Position& start, const Position& end) const {
    if (start.first < end.first) { // right
      for (char c = start.first + 1; c < end.first; c++) {
        if (get_piece(Position(c,start.second)) != nullptr) {return false;}
      }
    } else if (end.first < start.first) { // left
      for (char c = end.first + 1; c < start.first; c++) {
        if (get_piece(Position(c,start.second)) != nullptr) {return false;}
      }
    } else if (start.second < end.second) { // up
      for (char r = start.second + 1; r < end.second; r++) {
        if (get_piece(Position(start.first,r)) != nullptr) {return false;}
      }
    } else if (end.second < start.second) { // down
      for (char r = end.second + 1; r < start.second; r++) {
        if (get_piece(Position(start.first,r)) != nullptr) {return false;}
      }
    }
    return true;
  }

  /////////////////////////////////////
  // DO NOT MODIFY THIS FUNCTION!!!! //
  /////////////////////////////////////
  std::ostream& operator<<(std::ostream& os, const Board& board) {
    for(char r = '8'; r >= '1'; r--) {
      for(char c = 'A'; c <= 'H'; c++) {
	const Piece* piece = board(Position(c, r));
	if (piece) {
	  os << piece->to_ascii();
	} else {
	  os << '-';
	}
      }
      os << std::endl;
    }
    return os;
  }
}

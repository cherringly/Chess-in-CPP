/*
Team Members:

Name: Mary Zhang
JHED: mzhan188

Name: Megan Weng
JHED: mweng5

Name: Jiaxing Brisbois
JHED: jbrisbo1
*/

#ifndef GAME_H
#define GAME_H


#include <iostream>
#include "Piece.h"
#include "Board.h"
#include "Exceptions.h"


namespace Chess
{


   class Game {


   public:
       // This default constructor initializes a board with the standard
       // piece positions, and sets the state to white's turn
       Game();

       // destructor
       ~Game();

       // copy constructor
       Game(const Game &game);

       // Returns true if it is white's turn
       /////////////////////////////////////
       // DO NOT MODIFY THIS FUNCTION!!!! //
       /////////////////////////////////////
       bool turn_white() const { return is_white_turn; }
  
   /////////////////////////////////////
       // DO NOT MODIFY THIS FUNCTION!!!! //
       /////////////////////////////////////
   // Displays the game by printing it to stdout
       void display() const { board.display(); }
  
   /////////////////////////////////////
       // DO NOT MODIFY THIS FUNCTION!!!! //
       /////////////////////////////////////
   // Checks if the game is valid
       bool is_valid_game() const { return board.has_valid_kings(); }


       // Attempts to make a move. If successful, the move is made and
       // the turn is switched white <-> black. Otherwise, an exception is thrown
       void make_move(const Position& start, const Position& end);


       // Returns true if the designated player is in check
       bool in_check(const bool& white) const;


       // Returns true if the designated player is in mate
       bool in_mate(const bool& white) const;


       // Returns true if the designated player is in mate
       bool in_stalemate(const bool& white) const;


       // Return the total material point value of the designated player
       int point_value(const bool& white) const;


       // Return true if moving piece to Position end would expose check
       bool check_move_check(const Position start, const Position end) const;


       // Try all possible moves for a piece at the given start position
       bool try_all_moves(Game& copy, const Position& start) const;


       // If pawn is on promotion rank, promote it to a queen
       void handle_promotion(const Position& position);


   private:
       // The board
       Board board;


       // Is it white's turn?
       bool is_white_turn;


       // Writes the board out to a stream
       friend std::ostream& operator<< (std::ostream& os, const Game& game);


       // Reads the board in from a stream
       friend std::istream& operator>> (std::istream& is, Game& game);
   };
}
#endif // GAME_H



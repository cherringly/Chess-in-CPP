/*
Team Members:

Name: Mary Zhang
JHED: mzhan188

Name: Megan Weng
JHED: mweng5

Name: Jiaxing Brisbois
JHED: jbrisbo1
*/

#include <cassert>
#include "Game.h"
#include "Piece.h"

namespace Chess
{
	/////////////////////////////////////
	// DO NOT MODIFY THIS FUNCTION!!!! //
	/////////////////////////////////////
	Game::Game() : is_white_turn(true) {
		// Add the pawns
		for (int i = 0; i < 8; i++) {
			board.add_piece(Position('A' + i, '1' + 1), 'P');
			board.add_piece(Position('A' + i, '1' + 6), 'p');
		}

		// Add the rooks
		board.add_piece(Position( 'A'+0 , '1'+0 ) , 'R' );
		board.add_piece(Position( 'A'+7 , '1'+0 ) , 'R' );
		board.add_piece(Position( 'A'+0 , '1'+7 ) , 'r' );
		board.add_piece(Position( 'A'+7 , '1'+7 ) , 'r' );

		// Add the knights
		board.add_piece(Position( 'A'+1 , '1'+0 ) , 'N' );
		board.add_piece(Position( 'A'+6 , '1'+0 ) , 'N' );
		board.add_piece(Position( 'A'+1 , '1'+7 ) , 'n' );
		board.add_piece(Position( 'A'+6 , '1'+7 ) , 'n' );

		// Add the bishops
		board.add_piece(Position( 'A'+2 , '1'+0 ) , 'B' );
		board.add_piece(Position( 'A'+5 , '1'+0 ) , 'B' );
		board.add_piece(Position( 'A'+2 , '1'+7 ) , 'b' );
		board.add_piece(Position( 'A'+5 , '1'+7 ) , 'b' );

		// Add the kings and queens
		board.add_piece(Position( 'A'+3 , '1'+0 ) , 'Q' );
		board.add_piece(Position( 'A'+4 , '1'+0 ) , 'K' );
		board.add_piece(Position( 'A'+3 , '1'+7 ) , 'q' );
		board.add_piece(Position( 'A'+4 , '1'+7 ) , 'k' );
	}

	// copy constructor & sets is_white_turn
	Game::Game(const Game &game) {
        board = game.board;
		is_white_turn = game.is_white_turn;
    }

	// destructor
	Game::~Game() {}

	// handle promotion of pawns to queens 
	void Game::handle_promotion(const Position& position) {
       const Piece* piece = board(position);
       if (!piece) return; // No piece at the position

       char designator = piece->to_ascii();
       // Check if it's a pawn on the promotion rank
       if (designator == 'P' && position.second == '8') {
           board.remove_piece(position);
           board.add_piece(position, 'Q'); // Promote to queen
       }
       else if (designator == 'p' && position.second == '1') {
           board.remove_piece(position);
           board.add_piece(position, 'q'); // Promote to queen
       }
   }


	// Attempts to make a move. If successful, the move is made and
	// the turn is switched white <-> black. Otherwise, an exception is thrown
	void Game::make_move(const Position& start, const Position& end) {
		// check start pos
		if(start.first < 'A' || start.first > 'H' || start.second < '1' || start.second > '8') {
			throw Chess::Exception("start position is not on board");
		}

		// check end pos
		if(end.first < 'A' || end.first > 'H' || end.second < '1' || start.second > '8') {
			throw Chess::Exception("end position is not on board");
		}

		// get pieces at specified start and end positions
		const Piece* sloc = board(start);
		const Piece* eloc = board(end);

		// check if piece is present at start pos
		if (sloc == nullptr) {
			throw Chess::Exception("no piece at start position");
		}

		// check if piece is white and it is white's turn		
		if (sloc->is_white() != is_white_turn) {
			throw Chess::Exception("piece color and turn do not match");
		}

		// check if path is clear
		if (!board.check_path(start, end, sloc->to_ascii())) {
			throw Chess::Exception("path is not clear");
		}

		// first try to capture, then try to move
		if (eloc != nullptr) {
			if (sloc->is_white() == eloc->is_white()) {
				throw Chess::Exception("cannot capture own piece");
			}

			if (!(sloc->legal_capture_shape(start,end))) {
				throw Chess::Exception("illegal capture shape");
			}

			// check if moving piece would expose check
			if (check_move_check(start, end)) {
				throw Chess::Exception("move exposes check");
			}

			// update piece position/board if no errs present
			board.remove_piece(end);
			board.add_piece(end, sloc->to_ascii());
			board.remove_piece(start);
			handle_promotion(end);
			is_white_turn = !is_white_turn;

		} else { // no capture, try to move
			if (!(sloc->legal_move_shape(start,end))) {
				throw Chess::Exception("illegal move shape");
			}

			// check if moving piece would expose check
			if (check_move_check(start, end)) {
				throw Chess::Exception("move exposes check");
			}

			// update piece position/board if no errs present
			board.add_piece(end, sloc->to_ascii());
			board.remove_piece(start);
			handle_promotion(end);
			is_white_turn = !is_white_turn;
		}
	}

	// check if moving piece would expose check
	bool Game::check_move_check(const Position start, const Position end) const {
		// make a copy of the game state
		Game copy(*this);

		const Piece* sloc = copy.board(start);
		copy.board.remove_piece(end);
		copy.board.add_piece(end, sloc->to_ascii());
		copy.board.remove_piece(start);
		
		// check if the move exposes check on copy
		if (copy.in_check(is_white_turn)) {
			return true;
		}

		return false;
	}

	// Returns true if the designated player is in check
    bool Game::in_check(const bool& white) const {
		Position position;
		std::map<std::pair<char, char>, Piece*> occ = board.get_map();

		// find the king's position
		for (std::map<std::pair<char, char>, Piece*>::const_iterator it = occ.begin();
		it != occ.end();
		it++) {
			if (it->second) {
				switch (it->second->to_ascii()) {
				case 'K':
					if(white) {
						position = it->first;
					}
					break;
				case 'k':
					if(!white) {
					    position = it->first;
					}
					break;
				}
			}
		}

		// check if any piece on the board has a legal move to the king's position
		for (std::map<std::pair<char, char>, Piece*>::const_iterator it = occ.begin();
		it != occ.end();
		it++) {
			if (it->second) {
				// check if capturing the king is legal and if path is clear
				if (it->second->legal_capture_shape(it->first, position) &&
                board.check_path(it->first, position, it->second->to_ascii())) {
					if (it->second->is_white() != white) {
						return true;
					}
				}
			}
		}
		return false;
	}

	// Try all possible moves for a piece at the given start position
	// Returns true if any move is valid
    bool Game::try_all_moves(Game& copy, const Position& start) const {
		for (char c = 'A'; c <= 'H'; c++) {
			for (char i = '1'; i <= '8'; i++) {
				// for each position
				try {
					copy.make_move(start,Position(c,i));
					return true;
				} catch (Chess::Exception &e) {}
			}
		}
		return false; // no valid moves found
	}

	// Returns true if the designated player is in mate
	bool Game::in_mate(const bool& white) const {
		if (in_check(white)) { //check if in check
			Game copy(*this);
			std::map<std::pair<char, char>, Piece*> occ = copy.board.get_map();
			// check if any piece can make a valid move, returns false (not in mate) if so
			for (std::map<std::pair<char, char>, Piece*>::const_iterator it = occ.begin(); it != occ.end(); it++) {
				if (try_all_moves(copy,it->first)) {return false;}
			}
			return true; // no valid moves found, in mate
		}
		return false; // not in mate if not in check
	}

	// Returns true if the designated player is in stalemate
	bool Game::in_stalemate(const bool& white) const {
		if (!in_check(white)) {
			Game copy(*this);
			std::map<std::pair<char, char>, Piece*> occ = board.get_map();
			// for each piece on the board, check if any piece can make a valid move
			for (std::map<std::pair<char, char>, Piece*>::const_iterator it = occ.begin(); it != occ.end(); it++) {
				if (it->second && ( white == it->second->is_white())) {
					if (try_all_moves(copy,it->first)) {
						return false; // found a valid move, not in stalemate
					}
				}
			}
			return true; // no valid moves found, in stalemate
		}
		return false; // not in stalemate if in check
	}

    // Return the total material point value of the designated player
    int Game::point_value(const bool& white) const {
		int whitePoints = 0;
		int blackPoints = 0;
		
		// get map of board and iterate through it, add points based on what piece it is
		std::map<std::pair<char, char>, Piece*> occ = board.get_map();
		for (std::map<std::pair<char, char>, Piece*>::const_iterator it = occ.begin();
		it != occ.end();
		it++) {
			if (it->second) {
				switch (it->second->to_ascii()) {
					case 'P':
						whitePoints += 1;
						break;
					case 'p':
						blackPoints += 1;
						break;
					case 'N':
						whitePoints += 3;
						break;
					case 'n':
						blackPoints += 3;
						break;
					case 'R':
						whitePoints += 5;
						break;
					case 'r':
						blackPoints += 5;
						break;
					case 'B':
						whitePoints += 3;
						break;
					case 'b':
						blackPoints += 3;
						break;
					case 'Q':
						whitePoints += 9;
						break;
					case 'q':
						blackPoints += 9;
						break;
				}
			}		
		}
		// return the total points based on whose turn it is
        if (white) {
			return whitePoints;
		} else if (!white) {
			return blackPoints;
		}
		return -1; // this should never be reached
    }

	std::istream& operator>> (std::istream& is, Game& game) {
		Board boardIn;

		// Copy the provided layout onto the new board
		int cover = 0;
		for (char r = '8'; r >= 1; r--) {
			for (char c = 'A'; c <= 'H'; c++) {
				char cur;
				is >> cur;
				if (cover < 64) {
					if (cur != '-') {
						boardIn.add_piece(Position(c,r), cur);
					}
				}
				// after 64 squares, there is a final 
				// character indicating which player's
				// turn it is
				if (cover == 64) {
					if (cur == 'b') {
						game.is_white_turn = false;
					}
					if (cur == 'w') {
						game.is_white_turn = true;
					}
				}
				cover++;
			}
		}

		game.board = boardIn;
		return is;
	}

    /////////////////////////////////////
    // DO NOT MODIFY THIS FUNCTION!!!! //
    /////////////////////////////////////
	std::ostream& operator<< (std::ostream& os, const Game& game) {
		// Write the board out and then either the character 'w' or the character 'b',
		// depending on whose turn it is
		return os << game.board << (game.turn_white() ? 'w' : 'b');
	}
}

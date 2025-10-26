///////////////////////////////////
// IT IS OK TO MODIFY THIS FILE, //
// YOU WON'T HAND IT IN!!!!!     //
///////////////////////////////////
#ifndef MYSTERY_H
#define MYSTERY_H

#include "Piece.h"

namespace Chess
{
	class Mystery : public Piece {

	public:
		bool legal_move_shape(const Position& start, const Position& end) const override{
			if(start.first < 'A' || start.first > 'H' || start.second < '1' || start.second > '8' 
			|| end.first < 'A' || end.first > 'H' || end.second < '1' || end.second > '8') {
				return false;
			}

			// calculate the difference in x and y coordinates
			int dx = std::abs(end.first - start.first);
			int dy = std::abs(end.second - start.second);

			// return if moved in a straight line (either horizontally, vertically, or diagonally)
			// return if moved in an L shape (2 squares in one direction and 1 square in the other)
			return (((dx == 0 && dy > 0) || (dy == 0 && dx > 0) || (dx == dy && dx > 0)) || 
					(dx == 2 && dy == 1) || (dx == 1 && dy == 2));
		};

		char to_ascii() const override { return is_white() ? 'M' : 'm';	}
    
    std::string to_unicode() const override { return is_white() ? "\u2687" : "\u2689"; }

	private:
		Mystery(bool is_white) : Piece(is_white) {}

		friend Piece* create_piece(const char& piece_designator);
	};
}
#endif // MYSTERY_H
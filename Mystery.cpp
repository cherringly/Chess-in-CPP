// #include "Mystery.h"
// #include <cmath>

// namespace Chess {

// bool Mystery::legal_move_shape(const Position& start, const Position& end) const {
//     if(start.first < 'A' || start.first > 'H' || start.second < '1' || start.second > '8' 
//       || end.first < 'A' || end.first > 'H' || end.second < '1' || end.second > '8') {
//       return false;
//     }
    
//     // Calculate coordinate differences
//     int dx = abs(end.first - start.first);
//     int dy = abs(end.second - start.second);
    
//     // Knight moves (L-shape)
//     bool knight_move = (dx == 2 && dy == 1) || (dx == 1 && dy == 2);
    
//     // Bishop moves (any distance diagonal)
//     bool bishop_move = (dx == dy);
    
//     // Combine movement patterns
//     return knight_move || bishop_move;
// }

// } // namespace Chess
#include "Pawn.h"
#include "CreatePiece.h"  // Include the correct header
#include <iostream>
#include <vector>
#include <string>
#include <memory>  // For std::unique_ptr

struct TestCase {
    std::string name;
    Chess::Position start;
    Chess::Position end;
    char piece_designator; // 'P' for white pawn, 'p' for black pawn
    bool expected_move;
    bool expected_capture;
};

void run_pawn_tests() {
    std::vector<TestCase> tests = {
        // White pawn tests
        {"White pawn single forward from start", {'A', '2'}, {'A', '3'}, 'P', true, false},
        {"White pawn double forward from start", {'B', '2'}, {'B', '4'}, 'P', true, false},
        {"White pawn single forward not from start", {'C', '3'}, {'C', '4'}, 'P', true, false},
        {"White pawn invalid double forward not from start", {'D', '3'}, {'D', '5'}, 'P', false, false},
        {"White pawn backward move", {'E', '4'}, {'E', '3'}, 'P', false, false},
        {"White pawn diagonal right capture", {'F', '4'}, {'G', '5'}, 'P', false, true},
        {"White pawn diagonal left capture", {'G', '4'}, {'F', '5'}, 'P', false, true},
        {"White pawn invalid sideways", {'H', '4'}, {'G', '4'}, 'P', false, false},
        
        // Black pawn tests
        {"Black pawn single forward from start", {'A', '7'}, {'A', '6'}, 'p', true, false},
        {"Black pawn double forward from start", {'B', '7'}, {'B', '5'}, 'p', true, false},
        {"Black pawn single forward not from start", {'C', '6'}, {'C', '5'}, 'p', true, false},
        {"Black pawn invalid double forward not from start", {'D', '5'}, {'D', '3'}, 'p', false, false},
        {"Black pawn backward move", {'E', '4'}, {'E', '5'}, 'p', false, false},
        {"Black pawn diagonal right capture", {'F', '4'}, {'G', '3'}, 'p', false, true},
        {"Black pawn diagonal left capture", {'G', '4'}, {'F', '3'}, 'p', false, true},
        {"Black pawn invalid sideways", {'H', '4'}, {'G', '4'}, 'p', false, false}
    };

    int passed_move = 0;
    int passed_capture = 0;
    const int total = static_cast<int>(tests.size());

    for (const auto& test : tests) {
        // Use the official factory function
        std::unique_ptr<Chess::Piece> piece(Chess::create_piece(test.piece_designator));
        Chess::Pawn* pawn = dynamic_cast<Chess::Pawn*>(piece.get());
        
        if (!pawn) {
            std::cerr << "Error: Failed to create pawn for test: " << test.name << "\n";
            continue;
        }

        bool move_result = pawn->legal_move_shape(test.start, test.end);
        bool capture_result = pawn->legal_capture_shape(test.start, test.end);
        
        bool move_ok = (move_result == test.expected_move);
        bool capture_ok = (capture_result == test.expected_capture);

        if (move_ok) passed_move++;
        if (capture_ok) passed_capture++;

        std::cout << "Test: " << test.name << "\n";
        std::cout << "  Move shape: " << (move_ok ? "PASS" : "FAIL")
                  << " (got " << (move_result ? "true" : "false") 
                  << ", expected " << (test.expected_move ? "true" : "false") << ")\n";
        std::cout << "  Capture shape: " << (capture_ok ? "PASS" : "FAIL")
                  << " (got " << (capture_result ? "true" : "false")
                  << ", expected " << (test.expected_capture ? "true" : "false") << ")\n";
        std::cout << "----------------------------------------\n";
    }

    std::cout << "\nSUMMARY:\n";
    std::cout << "Move shape tests: " << passed_move << "/" << total << " passed\n";
    std::cout << "Capture shape tests: " << passed_capture << "/" << total << " passed\n";
}

int main() {
    std::cout << "Testing Pawn movement rules...\n\n";
    run_pawn_tests();
    return 0;
}
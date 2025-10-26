// g++ -std=c++11 tester.cpp Pawn.cpp Knight.cpp Bishop.cpp Rook.cpp Queen.cpp King.cpp CreatePiece.cpp -o tester
// ./tester

#include "Piece.h"
#include "CreatePiece.h"
#include "Pawn.h"
#include "Knight.h"
#include "Bishop.h"
#include "Rook.h"
#include "Queen.h"
#include "King.h"
#include <iostream>
#include <vector>
#include <string>
#include <memory>

struct TestCase {
    std::string name;
    Chess::Position start;
    Chess::Position end;
    char piece_designator;
    bool expected_move;
    bool expected_capture;
};

// Helper function to create pieces
std::unique_ptr<Chess::Piece> create_test_piece(char designator) {
    return std::unique_ptr<Chess::Piece>(Chess::create_piece(designator));
}

// Helper function to run tests
void run_tests(const std::vector<TestCase>& tests, const std::string& piece_name) {
    int passed_move = 0;
    int passed_capture = 0;
    const int total = static_cast<int>(tests.size());

    for (const auto& test : tests) {
        auto piece = create_test_piece(test.piece_designator);
        if (!piece) {
            std::cerr << "Error creating piece for test: " << test.name << "\n";
            continue;
        }

        bool move_result = piece->legal_move_shape(test.start, test.end);
        bool capture_result = piece->legal_capture_shape(test.start, test.end);
        
        bool move_ok = (move_result == test.expected_move);
        bool capture_ok = (capture_result == test.expected_capture);

        if (move_ok) passed_move++;
        if (capture_ok) passed_capture++;

        std::cout << piece_name << " Test: " << test.name << "\n";
        std::cout << "  Move shape: " << (move_ok ? "PASS" : "FAIL")
                  << " (got " << (move_result ? "true" : "false") 
                  << ", expected " << (test.expected_move ? "true" : "false") << ")\n";
        if (test.expected_capture != test.expected_move) {
            std::cout << "  Capture shape: " << (capture_ok ? "PASS" : "FAIL")
                      << " (got " << (capture_result ? "true" : "false")
                      << ", expected " << (test.expected_capture ? "true" : "false") << ")\n";
        }
        std::cout << "----------------------------------------\n";
    }

    std::cout << "\n" << piece_name << " SUMMARY:\n";
    std::cout << "Move shape tests: " << passed_move << "/" << total << " passed\n";
    if (passed_capture != passed_move) {
        std::cout << "Capture shape tests: " << passed_capture << "/" << total << " passed\n";
    }
    std::cout << "========================================\n\n";
}

void run_pawn_tests() {
    std::vector<TestCase> tests = {
        // White pawn tests
        {"Single forward from start", {'A', '2'}, {'A', '3'}, 'P', true, false},
        {"Double forward from start", {'B', '2'}, {'B', '4'}, 'P', true, false},
        {"Single forward not from start", {'C', '3'}, {'C', '4'}, 'P', true, false},
        {"Invalid double forward not from start", {'D', '3'}, {'D', '5'}, 'P', false, false},
        {"Backward move", {'E', '4'}, {'E', '3'}, 'P', false, false},
        {"Diagonal right capture", {'F', '4'}, {'G', '5'}, 'P', false, true},
        {"Diagonal left capture", {'G', '4'}, {'F', '5'}, 'P', false, true},
        {"Invalid sideways", {'H', '4'}, {'G', '4'}, 'P', false, false},
        
        // Black pawn tests
        {"Single forward from start", {'A', '7'}, {'A', '6'}, 'p', true, false},
        {"Double forward from start", {'B', '7'}, {'B', '5'}, 'p', true, false},
        {"Single forward not from start", {'C', '6'}, {'C', '5'}, 'p', true, false},
        {"Invalid double forward not from start", {'D', '5'}, {'D', '3'}, 'p', false, false},
        {"Backward move", {'E', '4'}, {'E', '5'}, 'p', false, false},
        {"Diagonal right capture", {'F', '4'}, {'G', '3'}, 'p', false, true},
        {"Diagonal left capture", {'G', '4'}, {'F', '3'}, 'p', false, true},
        {"Invalid sideways", {'H', '4'}, {'G', '4'}, 'p', false, false}
    };
    run_tests(tests, "PAWN");
}

void run_knight_tests() {
    std::vector<TestCase> tests = {
        // Knight moves (L-shape)
        {"Valid L-shape 1", {'B', '1'}, {'C', '3'}, 'N', true, true},
        {"Valid L-shape 2", {'B', '1'}, {'A', '3'}, 'N', true, true},
        {"Valid L-shape 3", {'E', '4'}, {'F', '6'}, 'N', true, true},
        {"Valid L-shape 4", {'E', '4'}, {'G', '5'}, 'N', true, true},
        {"Invalid move (1 square)", {'E', '4'}, {'E', '5'}, 'N', false, false},
        {"Invalid move (diagonal)", {'E', '4'}, {'F', '5'}, 'N', false, false},
        {"Invalid move (straight)", {'E', '4'}, {'E', '6'}, 'N', false, false},
        
        // Black knight tests
        {"Valid L-shape 1 (black)", {'B', '8'}, {'C', '6'}, 'n', true, true},
        {"Valid L-shape 2 (black)", {'B', '8'}, {'A', '6'}, 'n', true, true}
    };
    run_tests(tests, "KNIGHT");
}

void run_bishop_tests() {
    std::vector<TestCase> tests = {
        // Bishop moves (diagonal)
        {"Valid diagonal 1", {'C', '1'}, {'A', '3'}, 'B', true, true},
        {"Valid diagonal 2", {'C', '1'}, {'E', '3'}, 'B', true, true},
        {"Valid long diagonal", {'A', '1'}, {'H', '8'}, 'B', true, true},
        {"Invalid move (straight)", {'C', '1'}, {'C', '3'}, 'B', false, false},
        {"Invalid move (L-shape)", {'C', '1'}, {'D', '3'}, 'B', false, false},
        {"Invalid move (1 square non-diagonal)", {'C', '1'}, {'C', '2'}, 'B', false, false},
        
        // Black bishop tests
        {"Valid diagonal (black)", {'F', '8'}, {'C', '5'}, 'b', true, true},
        {"Invalid move (black)", {'F', '8'}, {'F', '7'}, 'b', false, false}
    };
    run_tests(tests, "BISHOP");
}

void run_rook_tests() {
    std::vector<TestCase> tests = {
        // Rook moves (straight)
        {"Valid horizontal", {'A', '1'}, {'H', '1'}, 'R', true, true},
        {"Valid vertical", {'A', '1'}, {'A', '8'}, 'R', true, true},
        {"Valid short move", {'D', '4'}, {'D', '5'}, 'R', true, true},
        {"Invalid move (diagonal)", {'A', '1'}, {'B', '2'}, 'R', false, false},
        {"Invalid move (L-shape)", {'A', '1'}, {'B', '3'}, 'R', false, false},
        
        // Black rook tests
        {"Valid horizontal (black)", {'A', '8'}, {'H', '8'}, 'r', true, true},
        {"Invalid move (black)", {'A', '8'}, {'B', '7'}, 'r', false, false}
    };
    run_tests(tests, "ROOK");
}

void run_queen_tests() {
    std::vector<TestCase> tests = {
        // Queen moves (straight or diagonal)
        {"Valid diagonal", {'D', '1'}, {'H', '5'}, 'Q', true, true},
        {"Valid straight", {'D', '1'}, {'D', '8'}, 'Q', true, true},
        {"Valid short move", {'E', '4'}, {'E', '5'}, 'Q', true, true},
        {"Invalid move (L-shape)", {'D', '1'}, {'E', '3'}, 'Q', false, false},
        {"Invalid move (knight-like)", {'D', '1'}, {'F', '2'}, 'Q', false, false},
        
        // Black queen tests
        {"Valid move (black)", {'D', '8'}, {'D', '4'}, 'q', true, true},
        {"Valid move (black)", {'D', '8'}, {'F', '6'}, 'q', true, true}
    };
    run_tests(tests, "QUEEN");
}

void run_king_tests() {
    std::vector<TestCase> tests = {
        // King moves (1 square any direction)
        {"Valid horizontal", {'E', '1'}, {'F', '1'}, 'K', true, true},
        {"Valid vertical", {'E', '1'}, {'E', '2'}, 'K', true, true},
        {"Valid diagonal", {'E', '1'}, {'F', '2'}, 'K', true, true},
        {"Invalid move (2 squares)", {'E', '1'}, {'E', '3'}, 'K', false, false},
        {"Invalid move (L-shape)", {'E', '1'}, {'F', '3'}, 'K', false, false},
        
        // Black king tests
        {"Valid move (black)", {'E', '8'}, {'D', '7'}, 'k', true, true},
        {"Invalid move (black)", {'E', '8'}, {'E', '6'}, 'k', false, false}
    };
    run_tests(tests, "KING");
}

int main() {
    std::cout << "Testing Chess Piece Movement Rules...\n\n";
    run_pawn_tests();
    run_knight_tests();
    run_bishop_tests();
    run_rook_tests();
    run_queen_tests();
    run_king_tests();
    return 0;
}
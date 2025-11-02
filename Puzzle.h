#pragma once
#include <array>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

struct Puzzle {
    std::array<int,9> board{};      // 0 represents blank
    int zeroIndex = 0;               // index of blank
    int g = 0;                       // cost so far
    int h = 0;                       // heuristic
    int f = 0;                       // f = g + h
    int depth = 0;                   // same as g for unit-cost moves
    std::string move;                // move that led here (e.g., "Up")
    std::shared_ptr<Puzzle> parent;  // parent for path reconstruction

    Puzzle() = default;
    explicit Puzzle(const std::array<int,9>& b);

    bool isGoal() const;
    void printState() const;
    std::vector<std::shared_ptr<Puzzle>> expand() const;

    bool operator==(const Puzzle& other) const { return board == other.board; }
};

// Helpers for hashing / keys
std::string toKey(const Puzzle& p);

#include "Puzzle.h"
#include <utility>

static const std::array<int,9> GOAL = {1,2,3,4,5,6,7,8,0};

Puzzle::Puzzle(const std::array<int,9>& b) : board(b) {
    for (int i = 0; i < 9; ++i) if (board[i] == 0) { zeroIndex = i; break; }
    depth = g;
    f = g + h;
}

bool Puzzle::isGoal() const {
    return board == GOAL;
}

void Puzzle::printState() const {
    auto show = [&](int v) {
        if (v == 0) std::cout << 'b';
        else std::cout << v;
    };
    for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < 3; ++c) {
            show(board[r*3+c]);
            if (c < 2) std::cout << ' ';
        }
        std::cout << '\n';
    }
}

std::vector<std::shared_ptr<Puzzle>> Puzzle::expand() const {
    std::vector<std::shared_ptr<Puzzle>> children;
    int zr = zeroIndex / 3, zc = zeroIndex % 3;

    auto makeChild = [&](int nr, int nc, const std::string& mv) {
        std::array<int,9> nb = board;
        int ni = nr*3 + nc;
        nb[zeroIndex] = nb[ni];
        nb[ni] = 0;
        auto ch = std::make_shared<Puzzle>(nb);
        ch->parent = std::make_shared<Puzzle>(*this);
        ch->g = this->g + 1;
        ch->depth = ch->g;
        ch->move = mv;
        // h and f will be set by the solver based on chosen heuristic
        return ch;
    };

    // Up
    if (zr > 0) children.push_back(makeChild(zr-1, zc, "Up"));
    // Down
    if (zr < 2) children.push_back(makeChild(zr+1, zc, "Down"));
    // Left
    if (zc > 0) children.push_back(makeChild(zr, zc-1, "Left"));
    // Right
    if (zc < 2) children.push_back(makeChild(zr, zc+1, "Right"));

    return children;
}

std::string toKey(const Puzzle& p) {
    std::string s; s.reserve(18);
    for (int i = 0; i < 9; ++i) {
        s.push_back(char(p.board[i] + '0')); // digits 0..9 are fine for 0..8
        if (i != 8) s.push_back(',');
    }
    return s;
}

#include <bits/stdc++.h>
#include "Puzzle.h"
#include "Heuristics.h"
#include "Solver.h"
#include "Utilities.h"

using namespace std;

static Puzzle defaultPuzzle() {
    // Default example from the prompt’s sample I/O
    // 1 2 3
    // 4 8 0
    // 7 6 5
    array<int,9> b = {1,2,3,4,8,0,7,6,5};
    return Puzzle(b);
}

static Puzzle readCustomPuzzle() {
    cout << "Enter your puzzle, use a zero to represent the blank\n";
    cout << "Enter the first row, use space or tabs between numbers ";
    int a,b,c; cin >> a >> b >> c;
    cout << "Enter the second row, use space or tabs between numbers ";
    int d,e,f; cin >> d >> e >> f;
    cout << "Enter the third row, use space or tabs between numbers ";
    int g,h,i; cin >> g >> h >> i;

    array<int,9> board = {a,b,c,d,e,f,g,h,i};
    return Puzzle(board);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cout << "Welcome to XXX 8 puzzle solver.\n";
    cout << "Type \"1\" to use a default puzzle, or \"2\" to enter your own puzzle.\n";

    int choice;
    while (!(cin >> choice) || (choice != 1 && choice != 2)) {
        cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Please enter 1 or 2: ";
    }

    Puzzle start = (choice == 1) ? defaultPuzzle() : readCustomPuzzle();

    if (!Utils::isSolvable(start)) {
        cout << "This puzzle is not solvable. Exiting.\n";
        return 0;
    }

    cout << "Enter your choice of algorithm\n";
    cout << "1. Uniform Cost Search\n";
    cout << "2. A* with the Misplaced Tile heuristic.\n";
    cout << "3. A* with the Euclidean distance heuristic.\n";

    int algo;
    while (!(cin >> algo) || (algo < 1 || algo > 3)) {
        cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Please enter 1, 2, or 3: ";
    }

    Solver solver(start);

    switch (algo) {
        case 1:
            solver.uniformCostSearch();
            break;
        case 2:
            solver.aStarMisplaced();
            break;
        case 3:
            solver.aStarEuclidean();
            break;
        default:
            break;
    }

    return 0;
}

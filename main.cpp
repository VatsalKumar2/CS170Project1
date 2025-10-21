#include <iostream>
#include <string.
#include <vector>

using namespace std;

int main() {

   vector<vector<int>> vecOfVec;
   vector<int> vec;
    int x, y, z; 
    //intro text
     cout << "Welcome to 862336120 8 puzzle solver." << endl;
     cout << "Type '1' to use a default puzzle, or '2' to enter your own puzzle." << endl;
     cout << endl;

     //instructions for puzzle

     cout << "Enter your puzzle, use a zero to represent the blank" << endl; 

     cout << "Enter your first row, use space or tabs between numbers" << endl;
      cin >> x; 
     vec.push_back(x);
     cin >> y;
     vec.push_back(y);
     cin >> z;
     vec.push_back(z);
     vecOfVec.push_back(vec);

     cout <<  "Enter your second row, use space or tabs between numbers" << endl;
    cin >> x; 
     vec.at(0) = x;
     cin >> y;
     vec.at(1) = y;
     cin >> z;
     vec.at(2) = z;
     vecOfVec.push_back(vec);

     cout << "Enter your third row, use space or tabs between numbers" << endl;
     cin >> x; 
     vec.at(0) = x;
     cin >> y;
     vec.at(1) = y;
     cin >> z;
     vec.at(2) = z;
     vecOfVec.push_back(vec);

     cout << "Enter your choice of algorithm" << endl;

     cout << "Uniform Cost Search (type A)" << endl;
     cout << "A* with the Misplaced Tile heuristic (type B)" << endl;
     cout <<"A* with the Euclidean distance heuristic (type C)" << endl;

    return 0;
}

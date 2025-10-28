// this is tentative code for the algorithms and how the 8-puzzle itself works. I don't presume these to be changed too much, this is just for the sake of a general idea!!

// SearchAlgorithms.cpp
#include <bits/stdc++.h>
using namespace std;

/* ========================== 8-Puzzle Core Types ========================== */

using State = array<int,9>;    // tiles 1..8, 0 = blank
const State GOAL = {1,2,3,4,5,6,7,8,0}; 

namespace std {
template<> struct hash<State> {
    size_t operator()(State const& s) const noexcept {
        size_t h = 1469598103934665603ULL;
        for (int v : s) { h ^= (uint64_t)(v+1); h *= 1099511628211ULL; }
        return h;
    }
};
}

/* ========================== Utilities ========================== */

static inline bool is_solvable(const State& s) {
    vector<int> a; a.reserve(9);
    for (int v : s) if (v != 0) a.push_back(v);
    int inv = 0;
    for (int i=0;i<(int)a.size();++i)
        for (int j=i+1;j<(int)a.size();++j)
            if (a[i] > a[j]) ++inv;
    return (inv % 2) == 0;
}

static inline vector<pair<char,State>> neighbors(const State& s) {
    int idx = int(find(s.begin(), s.end(), 0) - s.begin());
    int r = idx/3, c = idx%3;
    auto swap_at = [&](int i, int j){ State t = s; swap(t[i], t[j]); return t; };
    vector<pair<char,State>> out; out.reserve(4);
    if (r > 0) out.push_back({'U', swap_at(idx, idx-3)});
    if (r < 2) out.push_back({'D', swap_at(idx, idx+3)});
    if (c > 0) out.push_back({'L', swap_at(idx, idx-1)});
    if (c < 2) out.push_back({'R', swap_at(idx, idx+1)});
    return out;
}

static inline void print_board(const State& st) {
    for (int i=0;i<9;i+=3) {
        for (int j=0;j<3;++j) {
            int v = st[i+j];
            if (v==0) cout << "  ";
            else      cout << v << ' ';
        }
        cout << "\n";
    }
}

struct Solution {
    vector<char> actions;   // sequence like "URDL..."
    vector<State> states;   // path of boards (start..goal)
    int cost = -1;          // steps (unit costs)
};

/* ========================== Heuristics (8-Puzzle) ========================== */

enum class HeuristicType { UCS, MISPLACED, EUCLIDEAN };

struct Heuristic {
    // goal position lookup for tiles 1..8
    int gr[9], gc[9]; // index by tile value (0..8); 0 unused
    Heuristic() {
        for (int i=0;i<9;++i) {
            int val = GOAL[i];
            int r = i/3, c = i%3;
            gr[val] = r; gc[val] = c;
        }
    }
    double operator()(const State& s, HeuristicType type) const {
        if (type == HeuristicType::UCS) return 0.0;
        if (type == HeuristicType::MISPLACED) {
            int m = 0;
            for (int i=0;i<9;++i) if (s[i] != 0 && s[i] != GOAL[i]) ++m;
            return double(m);
        }
        // Euclidean: sum over tiles sqrt((dr)^2 + (dc)^2)
        double sum = 0.0;
        for (int i=0;i<9;++i) {
            int v = s[i]; if (v==0) continue;
            int r = i/3, c = i%3;
            int dr = r - gr[v], dc = c - gc[v];
            sum += sqrt(double(dr*dr + dc*dc));
        }
        return sum;
    }
};

/* ========================== A* / UCS for 8-Puzzle ========================== */

Solution solve_8puzzle(const State& start, HeuristicType htype) {
    Solution sol;
    if (!is_solvable(start)) {
        // unsolvable -> return empty
        return sol;
    }

    Heuristic H;

    struct PQItem {
        double f, neg_g;
        State s;
    };
    struct Cmp {
        bool operator()(PQItem const& a, PQItem const& b) const {
            if (a.f != b.f) return a.f > b.f;      // min-heap by f
            return a.neg_g > b.neg_g;              // tie-break: prefer deeper g
        }
    };

    unordered_map<State, double> g;
    unordered_map<State, State> parent;
    unordered_map<State, char> action_from;
    unordered_set<State> closed;

    priority_queue<PQItem, vector<PQItem>, Cmp> open;

    g[start] = 0.0;
    open.push({H(start, htype), -0.0, start});

    while (!open.empty()) {
        auto [fcur, ng, cur] = open.top(); open.pop();
        if (closed.count(cur)) continue;

        if (cur == GOAL) {
            // reconstruct
            vector<State> states{cur};
            vector<char> actions;
            while (parent.count(cur)) {
                actions.push_back(action_from[cur]);
                cur = parent[cur];
                states.push_back(cur);
            }
            reverse(states.begin(), states.end());
            reverse(actions.begin(), actions.end());
            sol.states = move(states);
            sol.actions = move(actions);
            sol.cost = int(sol.states.size()) - 1;
            return sol;
        }

        closed.insert(cur);

        for (auto& [act, nxt] : neighbors(cur)) {
            if (closed.count(nxt)) continue;
            double tentative = g[cur] + 1.0; // unit move cost
            auto it = g.find(nxt);
            if (it == g.end() || tentative < it->second) {
                g[nxt] = tentative;
                parent[nxt] = cur;
                action_from[nxt] = act;
                double f = tentative + H(nxt, htype);
                open.push({f, -tentative, nxt});
            }
        }
    }

    return sol; // goal not reached (shouldn't happen if solvable)
}

/* ========================== Demo / Main ========================== */

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Example start state (0 is the blank)
    State start = {1,2,3,4,0,6,7,5,8};

    cout << "Start state:\n"; print_board(start);
    if (!is_solvable(start)) {
        cout << "This configuration is unsolvable.\n";
        return 0;
    }

    // A* with Misplaced Tiles
    {
        auto res = solve_8puzzle(start, HeuristicType::MISPLACED);
        cout << "\nA* (Misplaced Tiles) — steps: " << res.cost
             << " | moves: ";
        for (char c : res.actions) cout << c;
        cout << "\n";
        for (size_t i=0;i<res.states.size();++i) {
            cout << "Step " << i << ":\n"; print_board(res.states[i]); cout << "\n";
        }
    }

    // A* with Euclidean Distance (per-tile)
    {
        auto res = solve_8puzzle(start, HeuristicType::EUCLIDEAN);
        cout << "A* (Euclidean) — steps: " << res.cost
             << " | moves: ";
        for (char c : res.actions) cout << c;
        cout << "\n";
    }

    // Uniform Cost Search (fewest moves)
    {
        auto res = solve_8puzzle(start, HeuristicType::UCS);
        cout << "UCS — steps: " << res.cost
             << " | moves: ";
        for (char c : res.actions) cout << c;
        cout << "\n";
    }

    return 0;
}


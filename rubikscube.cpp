#include <iostream>
#include <vector>
#include <queue>
#include <array>
#include <set>
using namespace std;

int rgw = 0, flu = 0;
int gwr = 1, luf = 1;
int wrg = 2, ufl = 2;

int rwb = 3, fur = 3;
int wbr = 4, urf = 4;
int brw = 5, rfu = 5;

int ryg = 6, fdl = 6;
int ygr = 7, dlf = 7;
int gry = 8, lfd = 8;

int rby = 9, frd = 9;
int byr = 10, rdf = 10;
int yrb = 11, dfr = 11;

int owg = 12, bul = 12;
int wgo = 13, ulb = 13;
int gow = 14, lbu = 14;

int obw = 15, bru = 15;
int bwo = 16, rub = 16;
int wob = 17, ubr = 17;

int ogy = 18, bld = 18;
int gyo = 19, ldb = 19;
int yog = 20, dbl = 20;

int oyb = 21, bdr = 21;
int ybo = 22, drb = 22;
int boy = 23, rbd = 23;

array<int, 24> perm_inverse_array(const array<int, 24>& p) {
    array<int, 24> inv;
    for (int i = 0; i < 24; i++) {
        inv[p[i]] = i;
    }
    return inv;
}

array<int, 24> I = {flu, luf, ufl, fur, urf, rfu, fdl, dlf, lfd, frd, rdf, dfr,
                   bul, ulb, lbu, bru, rub, ubr, bld, ldb, dbl, bdr, drb, rbd};

array<int, 24> F = {fdl, dlf, lfd, flu, luf, ufl, frd, rdf, dfr, fur, urf, rfu,
                   bul, ulb, lbu, bru, rub, ubr, bld, ldb, dbl, bdr, drb, rbd};

array<int, 24> Fi = perm_inverse_array(F);

array<int, 24> L = {ulb, lbu, bul, fur, urf, rfu, ufl, flu, luf, frd, rdf, dfr,
                   dbl, bld, ldb, bru, rub, ubr, dlf, lfd, fdl, bdr, drb, rbd};

array<int, 24> Li = perm_inverse_array(L);

array<int, 24> U = {rfu, fur, urf, rub, ubr, bru, fdl, dlf, lfd, frd, rdf, dfr,
                   luf, ufl, flu, lbu, bul, ulb, bld, ldb, dbl, bdr, drb, rbd};

array<int, 24> Ui = perm_inverse_array(U);

set<array<int, 24>> quarter_twists = {F, Fi, L, Li, U, Ui};



array<int, 24> vector_to_array(const vector<int>& vec) {
    array<int, 24> arr;
    for (int i = 0; i < 24; i++) {
        arr[i] = vec[i];
    }
    return arr;
}

vector<int> array_to_vector(const array<int, 24>& arr) {
    vector<int> vec(24);
    for (int i = 0; i < 24; i++) {
        vec[i] = arr[i];
    }
    return vec;
}

vector<int> perm_inverse(const array<int, 24>& p) {
    vector<int> inv(24);
    for (int i = 0; i < 24; i++) {
        inv[p[i]] = i;
    }
    return inv;
}

vector<vector<int>> forward_parents(24, vector<int>(24, -1));
vector<vector<int>> backward_parents(24, vector<int>(24, -1));
set<array<int, 24>> visited;

array<int, 24> perm_apply(const array<int, 24>& position, const array<int, 24>& perm) {
    array<int, 24> new_position;
    for (int i = 0; i < 24; i++) {
        new_position[i] = position[perm[i]];
    }
    return new_position;
}

inline char permToChar(array<int, 24> p) {
    if (p == F)
        return 'F';
    else if (p == Fi)
        return 'f';
    else if (p == L)
        return 'L';
    else if (p == Li)
        return 'l';
    else if (p == U)
        return 'U';
    else if (p == Ui)
        return 'u';
    else return 'n';
}

inline array<int, 24> charToPerm(char p) {
    if (p == 'F')
        return F;
    else if (p == 'f')
        return Fi;
    else if (p == 'L')
        return L;
    else if (p == 'l')
        return Li;
    else if (p == 'U')
        return U;
    else if (p == 'u')
        return Ui;
}

inline string charToMove(char c) {
    if (c == 'F')
        return "F ";
    else if (c == 'f')
        return "Fi ";
    else if (c == 'L')
        return "L ";
    else if (c == 'l')
        return "Li ";
    else if (c == 'U')
        return "U ";
    else if (c == 'u')
        return "Ui ";
}

inline string invCharToMove(char c) {
    if (c == 'F')
        return "Fi ";
    else if (c == 'f')
        return "F ";
    else if (c == 'L')
        return "Li ";
    else if (c == 'l')
        return "L ";
    else if (c == 'U')
        return "Ui ";
    else if (c == 'u')
        return "U ";
}
string shortest_path(const vector<int>& start, const vector<int>& end) {
    if (start == end)
        return "Already Solved!";

    visited.insert(start);
    visited.insert(end);

    queue<pair<vector<int>, bool>> Q;
    Q.push(make_pair(start, true));
    Q.push(make_pair(end, false));

    while (true) {
        if (Q.empty())
            return "No Solution!";
        pair<vector<int>, bool> vertex = Q.front();
        vector<int> position = vertex.first;
        Q.pop();

        if (vertex.second == true) {
            set<array<int, 24>>::iterator move;
            for (move = quarter_twists.begin(); move != quarter_twists.end(); ++move) {
                array<int, 24> next_position = perm_apply(position, *move);
                if (visited.find(next_position) != visited.end()) {
                    continue;
                }
                visited.insert(next_position);
                forward_parents[vector_to_array(next_position)] = vector_to_array(position);
                Q.push(make_pair(next_position, true));
                if (backward_parents[vector_to_array(next_position)] != vector<int>(24, -1)) {
                    string solution;
                    vector<int> temp = next_position;
                    vector<int> x = forward_parents[vector_to_array(temp)];
                    while (x != vector<int>(24, -1)) {
                        solution = charToMove(arrayToChar(vector_to_array(x))) + solution;
                        temp = x;
                        x = forward_parents[vector_to_array(temp)];
                    }
                    temp = next_position;
                    vector<int> y = backward_parents[vector_to_array(temp)];
                    while (y != vector<int>(24, -1)) {
                        solution = solution + invCharToMove(arrayToChar(vector_to_array(y)));
                        temp = y;
                        y = backward_parents[vector_to_array(temp)];
                    }
                    return solution;
                }
            }
        } else {
            set<array<int, 24>>::iterator move;
            for (move = quarter_twists.begin(); move != quarter_twists.end(); ++move) {
                array<int, 24> next_position = perm_apply(position, *move);
                if (visited.find(next_position) != visited.end()) {
                    continue;
                }
                visited.insert(next_position);
                backward_parents[vector_to_array(next_position)] = vector_to_array(position);
                Q.push(make_pair(next_position, false));
                if (forward_parents[vector_to_array(next_position)] != vector<int>(24, -1)) {
                    string solution;
                    vector<int> temp = next_position;
                    vector<int> x = forward_parents[vector_to_array(temp)];
                    while (x != vector<int>(24, -1)) {
                        solution = charToMove(arrayToChar(vector_to_array(x))) + solution;
                        temp = x;
                        x = forward_parents[vector_to_array(temp)];
                    }
                    temp = next_position;
                    vector<int> y = backward_parents[vector_to_array(temp)];
                    while (y != vector<int>(24, -1)) {
                        solution = solution + invCharToMove(arrayToChar(vector_to_array(y)));
                        temp = y;
                        y = backward_parents[vector_to_array(temp)];
                    }
                    return solution;
                }
            }
        }
    }
}

array<int, 24> vector_to_array(const vector<int>& vec) {
    array<int, 24> arr;
    copy(vec.begin(), vec.end(), arr.begin());
    return arr;
}

int main() {
    
    vector<int> start = {6, 7, 8, 20, 18, 19, 3, 4, 5, 16, 17, 15, 0, 1, 2, 14, 12, 13, 10, 11, 9, 21, 22, 23};
    vector<int> ISet(I.begin(), I.end());
    string solution = shortest_path(start, ISet);
    cout << solution << endl;

    return 0;
}

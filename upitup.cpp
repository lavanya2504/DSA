#define _GLIBCXX_USE_CXX11_ABI 0
#include <iostream>
#include <vector>
#include <array>
#include <cmath>
#include <queue>
#include <cassert>
#include <algorithm>

using namespace std;

struct PuzzleBoard {
    int cells[3][3];
};

void printBoard(const PuzzleBoard &board) {
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            cout << board.cells[row][col] << " ";
        }
        cout << endl;
    }
}

pair<int, int> findEmptyCell(const PuzzleBoard &board) {
    int row = 0, col = 0;
    for (row = 0; row < 3; row++) {
        for (col = 0; col < 3; col++) {
            if (board.cells[row][col] == 0) {
                return make_pair(row, col);
            }
        }
    }
    return make_pair(-1, -1); // Return a special value to indicate unsolvable
}

bool isValid(const PuzzleBoard &board) {
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            if (board.cells[row][col] > 1) {
                return false;
            }
            if ((row * col == 1) && board.cells[1][1] != 0) {
                return false;
            }
        }
    }
    return true;
}

int computeOrder(const PuzzleBoard &board) {
    int order = 0;
    int exponent = 0;
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            order += board.cells[row][col] * pow(7, exponent);
            exponent++;
        }
    }
    return order;
}

PuzzleBoard moveUp(PuzzleBoard puzzle, int row, int col) {
    PuzzleBoard newPuzzle = puzzle;
    int value = puzzle.cells[row - 1][col];
    if (value == 1) {
        value = 6;
    } else if (value == 2) {
        value = 5;
    } else if (value == 5) {
        value = 1;
    } else if (value == 6) {
        value = 2;
    }
    newPuzzle.cells[row][col] = value;
    newPuzzle.cells[row - 1][col] = 0;
    return newPuzzle;
}

PuzzleBoard moveDown(const PuzzleBoard &puzzle, int row, int col) {
    PuzzleBoard newPuzzle = puzzle;
    int value = puzzle.cells[row + 1][col];
    if (value == 1) {
        value = 5;
    } else if (value == 2) {
        value = 6;
    } else if (value == 5) {
        value = 2;
    } else if (value == 6) {
        value = 1;
    }
    newPuzzle.cells[row][col] = value;
    newPuzzle.cells[row + 1][col] = 0;
    return newPuzzle;
}

PuzzleBoard moveLeft(const PuzzleBoard &puzzle, int row, int col) {
    PuzzleBoard newPuzzle = puzzle;
    int value = puzzle.cells[row][col - 1];
    if (value == 1) {
        value = 4;
    } else if (value == 2) {
        value = 3;
    } else if (value == 3) {
        value = 1;
    } else if (value == 4) {
        value = 2;
    }
    newPuzzle.cells[row][col] = value;
    newPuzzle.cells[row][col - 1] = 0;
    return newPuzzle;
}

PuzzleBoard moveRight(const PuzzleBoard &puzzle, int row, int col) {
    PuzzleBoard newPuzzle = puzzle;
    int value = puzzle.cells[row][col + 1];
    if (value == 1) {
        value = 3;
    } else if (value == 2) {
        value = 4;
    } else if (value == 3) {
        value = 2;
    } else if (value == 4) {
        value = 1;
    }
    newPuzzle.cells[row][col] = value;
    newPuzzle.cells[row][col + 1] = 0;
    return newPuzzle;
}

uint8_t visitedOrders[40353600] = {0};
uint64_t parentOrders[40353600];

vector<char> solvePuzzle(const PuzzleBoard &puzzle) {
    int count = 0;
    queue<PuzzleBoard> queue;
    queue.push(puzzle);
    visitedOrders[computeOrder(puzzle)] = 'S'; // S means start

    while (!queue.empty()) {
        PuzzleBoard currentPuzzle = queue.front();
        queue.pop();

        if (isValid(currentPuzzle)) {
            vector<char> moves;
            int order = computeOrder(currentPuzzle);

            while (visitedOrders[order] != 'S') {
                moves.push_back(visitedOrders[order]);
                order = parentOrders[order];
            }

            reverse(moves.begin(), moves.end());

            for (char move : moves) {
                switch (move) {
                    case 'U':
                        currentPuzzle = moveUp(currentPuzzle, findEmptyCell(currentPuzzle).first, findEmptyCell(currentPuzzle).second);
                        break;
                    case 'D':
                        currentPuzzle = moveDown(currentPuzzle, findEmptyCell(currentPuzzle).first, findEmptyCell(currentPuzzle).second);
                        break;
                    case 'L':
                        currentPuzzle = moveLeft(currentPuzzle, findEmptyCell(currentPuzzle).first, findEmptyCell(currentPuzzle).second);
                        break;
                    case 'R':
                        currentPuzzle = moveRight(currentPuzzle, findEmptyCell(currentPuzzle).first, findEmptyCell(currentPuzzle).second);
                        break;
                }

                // Check for unsolvable board after each move
                if (findEmptyCell(currentPuzzle).first == -1) {
                    cout << "The puzzle is unsolvable" << endl;
                    return vector<char>();
                }

                cout << "Move: " << move << endl;
                printBoard(currentPuzzle);
            }

            return moves;
        }

        auto emptyCell = findEmptyCell(currentPuzzle);
        int row = emptyCell.first;
        int col = emptyCell.second;

        PuzzleBoard moveUpBoard, moveDownBoard, moveLeftBoard, moveRightBoard;

        if (row != 0) {
            moveUpBoard = moveUp(currentPuzzle, row, col);
            int moveUpOrder = computeOrder(moveUpBoard);
            if (!visitedOrders[moveUpOrder]) {
                visitedOrders[moveUpOrder] = 'U';
                parentOrders[moveUpOrder] = computeOrder(currentPuzzle);
                queue.push(moveUpBoard);
                count++;
            }
        }

        if (row != 2) {
            moveDownBoard = moveDown(currentPuzzle, row, col);
            int moveDownOrder = computeOrder(moveDownBoard);
            if (!visitedOrders[moveDownOrder]) {
                visitedOrders[moveDownOrder] = 'D';
                parentOrders[moveDownOrder] = computeOrder(currentPuzzle);
                queue.push(moveDownBoard);
                count++;
            }
        }

        if (col != 0) {
            moveLeftBoard = moveLeft(currentPuzzle, row, col);
            int moveLeftOrder = computeOrder(moveLeftBoard);
            if (!visitedOrders[moveLeftOrder]) {
                visitedOrders[moveLeftOrder] = 'L';
                parentOrders[moveLeftOrder] = computeOrder(currentPuzzle);
                queue.push(moveLeftBoard);
                count++;
            }
        }

        if (col != 2) {
            moveRightBoard = moveRight(currentPuzzle, row, col);
            int moveRightOrder = computeOrder(moveRightBoard);
            if (!visitedOrders[moveRightOrder]) {
                visitedOrders[moveRightOrder] = 'R';
                parentOrders[moveRightOrder] = computeOrder(currentPuzzle);
                queue.push(moveRightBoard);
                count++;
            }
        }
    }

    cout << "The puzzle is unsolvable" << endl;
    return vector<char>();
}

void readPuzzle(PuzzleBoard &puzzle) {
    cout << "Enter the initial state of the 3x3 puzzle:" << endl;
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            cin >> puzzle.cells[row][col];
        }
    }
}

int main() {
    PuzzleBoard puzzle;
    readPuzzle(puzzle);
    printBoard(puzzle);

    auto moves = solvePuzzle(puzzle);

    for (char move : moves) {
        cout << move << endl;
    }

    return 0;
}


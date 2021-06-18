#include <cmath>
#include <iostream>
#include <fstream>
#include <queue>

#include "Labyrinth.h"


Labyrinth::Labyrinth(const std::string &name) {
    if(not name.empty())
        read(name);
}

Labyrinth::~Labyrinth() {
    for(uint i = 0; i < size; ++i) {
        delete map[i];
    }
    delete map;
}

bool Labyrinth::isPassable(position next) const {
    return 0 <= next.first && next.first < size &&
           0 <= next.second && next.second < size &&
           map[next.first][next.second] != 'x' &&
           map[next.first][next.second] != '*' &&
           map[next.first][next.second] != '.';
}

void Labyrinth::findStartEnd(bool lab) {
    for(uint i = 0; i < size; ++i) {
        for (uint j = 0; j < size; ++j) {
            char curr = map[i][j];
            if((lab && curr == '*') || curr == 'm') {
                start.first = i;
                start.second = j;
            }
            if(curr == 'e') {
                end.first = i;
                end.second = j;
            }
        }
    }
}

std::string Labyrinth::getExt(const std::string &name) const{
    std::string ext;
    auto it = name.cend();

    for(; *it != '.'; --it) {}
    for(; it != name.cend(); ++it) {
        ext += *it;
    }

    return ext;
}

bool Labyrinth::findPathRec(position curr) {
    // bottom of recursion
    print();

    if(curr == end) {
        return true;
    }

    // Else check every neighbour field
    // Enter if it's passable
    for(int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            if(abs(i + j) != 1)
                continue;

            position next{curr.first + i, curr.second + j};
            if (isPassable(next)) {
                map[curr.first][curr.second] = '.';
                map[next.first][next.second] = 'm';
                if(findPathRec(next)) return true;
                map[curr.first][curr.second] = 'm';
                map[next.first][next.second] = '0';
            }
        }
    }

    // So there is no path after all
    return false;
}

bool Labyrinth::findPath() {
    StackPositions stack;
    StackFrame stackFrame;

    // Start position
    stack.push(start);
    stackFrame.push(stack);

    // For every position in stackFrame.top()
    // we get a set of the new positions and
    // "recurse"
    while(!stackFrame.empty()) {
        StackPositions& currPositions = stackFrame.top();

        position& curr = currPositions.top();
        char& currPos = map[curr.first][curr.second];
        StackPositions newPositions;

        // currPos == '.' when prev step didn't have any new positions
        if(currPos != '.') {
            currPos = 'm';

            if(curr == end) {
                return true;
            }

            // Get stack of new positions
            for(int i = -1; i <= 1; ++i) {
                for (int j = -1; j <= 1; ++j) {
                    if(abs(i + j) != 1)
                        continue;
                    position next{curr.first + i, curr.second + j};
                    if (isPassable(next)) {
                        newPositions.push(next);
                    }
                }
            }
        }

        // If there are no new positions
        // we move back(remove the current one)
        if(newPositions.empty()) {
            currPos = '0';
            currPositions.pop();

            // if no positions are left from the stack
            // we remove this set from the stackFrame
            if(currPositions.empty()) {
                stackFrame.pop();
            }

        } else {
            // Else there are new positions to try
            stackFrame.push(newPositions);
            // We will "recurse" on them later
            // but let's get out of curr position!
            currPos = '.';
        }
    }

    return false;
}

void Labyrinth::findDistances() {
    // Queue of positions and their respective weights
    std::queue<std::pair<position, int>> queue;

    // Distance to starting position - 0
    queue.push({start, 0});

    // For every position we look for its neighbours
    // and assign for each valid one a weight
    while(!queue.empty()) {
        // We always take the smallest number which is not checked.
        // That guarantees we will find the shortest path to every
        // empty cell.
        weightedPosition& curr = queue.front();
        position& currPos = queue.front().first;

        // Update map
        char& posSymbol = map[currPos.first][currPos.second];
        if(posSymbol == '0' ||
           curr.second < posSymbol - '0')
        {
            posSymbol = curr.second + '0';
        }

        for(int i = -1; i <= 1; ++i) {
            for (int j = -1; j <= 1; ++j) {
                if(abs(i + j) != 1)
                    continue;
                position next{currPos.first + i, currPos.second + j};
                if (isPassable(next)) {
                    char& nextSym = map[next.first][next.second];
                    // If its not 0 its therefore the shortest path
                    if(nextSym == '0')
                        queue.push({next, curr.second + 1});
                }
            }
        }
        queue.pop();
    }
}

void Labyrinth::read(const std::string &name) {
    std::string ext = getExt(name);


    std::string path = "../data/" + name;
    std::ifstream file(path);

    if(!file.is_open()) {
        std::cout << "Corrupted file!\n";
        return;
    }

    file >> size;
    file.get();

    map = new char*[size];
    for(uint i = 0; i < size; ++i) {
        map[i] = new char[size];
    }


    std::string line;
    int row = 0;
    while(std::getline(file, line)) {
        int column = 0;
        for(int i = 0; i < line.length(); i += 2) {
            map[row][column] = line[i];
            ++column;
        }
        ++row;
    }

    findStartEnd(ext != "txt");
}

void Labyrinth::printPathStack() {
    if(findPath()) {
        print();
        return;
    }
    std::cout << "No path!\n";
}

void Labyrinth::printDistancesQueue() {
    findDistances();
    print();
}

void Labyrinth::print() const {
    for(uint i = 0; i < size; ++i) {
        for(uint j = 0; j < size; ++j) {
            std::cout << map[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}
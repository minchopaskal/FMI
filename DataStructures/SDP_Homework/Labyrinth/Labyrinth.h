#ifndef SDP_HOMEWORK_LABYRINTH_H
#define SDP_HOMEWORK_LABYRINTH_H

#include <stack>
#include <string>
#include <zconf.h>

using position = std::pair<int, int>;
using weightedPosition = std::pair<position, int>;

class Labyrinth {
private:
    using StackPositions = std::stack<position>;
    using StackFrame = std::stack<StackPositions>;

    //using Queue = std::queue<std::pair<position, int>>;

    char** map;
    uint size;
    position start;
    position end;

    bool isPassable(position) const;

    void findStartEnd(bool lab = false);

    std::string getExt(const std::string&) const;

    bool findPathRec(position);
    bool findPath();

    void findDistances();
public:
    Labyrinth(const std::string& name = "");
    Labyrinth(const Labyrinth&) = delete;
    Labyrinth&operator=(const Labyrinth&) = delete;
    ~Labyrinth();

    void read(const std::string&);
    void printPathStack();
    // We assume no distance is bigger than 9
    void printDistancesQueue();
    void print() const;
};


#endif //SDP_HOMEWORK_LABYRINTH_H

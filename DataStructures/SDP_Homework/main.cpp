#include <iostream>
#include "Tests/Tests.h"

using namespace StackTests;
using namespace QueueTests;


int main() {
    std::cout << "Stack tests:\n";
    testMergeStacks();
    testEditor();
    testDecompressString();
    testMouseLabyrinth();

    std::cout << "Queue tests:\n";
    testAllUnique();
    testReverse();
    testOrdered();
    testInterval();
    testDistanceLabyrinth();
    testBFS();
}

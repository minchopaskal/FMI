#include <iostream>
#include <stack>
#include <sstream>

#include "../Editor/Editor.h"
#include "../Labyrinth/Labyrinth.h"
#include "../StackAndQueueFunctions/StackAndQueueFunctions.h"
#include "Tests.h"

void StackTests::testMergeStacks() {
    std::stack<int> left;
    left.push(1);
    left.push(3);
    left.push(5);

    std::stack<int> right;
    right.push(1);
    right.push(2);
    right.push(6);

    std::stack<int> merged = StackFunctions::mergeStacks(left, right);
    while(!merged.empty()) {
        std::cout << merged.top() << " ";
        merged.pop();
    }
}

void StackTests::testEditor() {
    Editor editor("");
    editor.insert(0, "This is new text!");
    editor.insert(17, " Whole new sentence!");
    std::cout << editor.getText() << std::endl;
    editor.undo();
    std::cout << editor.getText() << std::endl;
    editor.redo();
    std::cout << editor.getText() << std::endl;
    editor.redo();
    editor.undo();
    std::cout << editor.getText() << std::endl;
}

void StackTests::testDecompressString() {
    std::cout << StackFunctions::decompressString("2(AB2(C))") << std::endl;
    std::cout << StackFunctions::decompressString("12(B2(Q))") << std::endl;
}

void StackTests::testMouseLabyrinth() {
    Labyrinth labyrinth("lab.txt");
    labyrinth.printPathStack();
}

void QueueTests::testAllUnique() {
    std::queue<int> unique;
    for(int i = 0; i < 5; ++i) {
        unique.push(i);
    }

    std::queue<int> nonUnique;
    for(int i = 0; i < 10; ++i) {
        nonUnique.push(
            i % 2 ? i * 2 : i
        );
    }

    std::cout << "Unique:" << QueueFunctions::allUnique(unique) << std::endl;
    std::cout << "Non-Unique:" << QueueFunctions::allUnique(nonUnique) << std::endl;
}

void QueueTests::testReverse() {
    std::cout << "Reversed Queue: ";

    std::queue<int> queue;
    for(int i = 0; i < 5; ++i) {
        queue.push(i);
    }

    QueueFunctions::reverse(queue);

    while(!queue.empty()) {
        std::cout << queue.front() << " ";
        queue.pop();
    }
    std::cout << std::endl;
}

void QueueTests::testOrdered() {
    std::cout << "Ordered Queue: ";
    std::queue<int> unordered;
    for(int i = 10; i >= 0; --i) {
        unordered.push(i);
    }

    QueueFunctions::printAscending(unordered);
}

void QueueTests::testInterval() {
    std::cout << "Array separated in intervals: ";
    int a = 3;
    int b = 5;
    int arr[]{6, 7, 8, 10, 3, -1, 5, 2, 4};
    QueueFunctions::printInterval(arr, 9, a, b);
    std::cout << std::endl;
}

void QueueTests::testDistanceLabyrinth() {
    Labyrinth labyrinth("labyrinth.lab");
    labyrinth.printDistancesQueue();
}

void QueueTests::testBFS() {

    int graphTest[9][9]{
            {0, 1, 0, 0, 0, 1, 0, 0, 0},
            {1, 0, 1, 0, 0, 0, 1, 1, 0},
            {0, 1, 0, 1, 0, 0, 0, 0, 1},
            {0, 0, 1, 0, 1, 0, 0, 0, 1},
            {0, 0, 0, 1, 0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 0, 0, 1, 0},
            {0, 1, 0, 0, 0, 1, 0, 0, 0},
            {0, 1, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 1, 1, 0, 0, 0, 0, 0}
    };

    int size = 9;
    auto graph = new int*[size];
    for(int i = 0; i < size; ++i) {
        graph[i] = new int[size];
    }

    for(int i = 0; i < size; ++i) {
        for(int j = 0; j < size; ++j) {
            graph[i][j] = graphTest[i][j];
        }
    }

    std::cout << QueueFunctions::getPathBFS(graph, size, 9, 7);
}










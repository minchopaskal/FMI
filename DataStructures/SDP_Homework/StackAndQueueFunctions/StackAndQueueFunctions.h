#include <iostream>
#include <queue>
#include <stack>

namespace StackFunctions {
    void lastMerge(std::stack<int>* merged, std::stack<int>& last);

    std::stack<int>& mergeStacks(std::stack<int>& leftStack,
                                 std::stack<int>& rightStack);

    int getCount(std::string::iterator& it);

    std::string decompressString(std::string str);
}

namespace QueueFunctions {
    template <class T>
    bool allUnique(std::queue<T> queue) {
        T sentinel;
        while(!queue.empty()) {
            queue.push(sentinel);
            T& curr = queue.front();
            queue.pop();
            while(queue.front() != sentinel) {
                if(queue.front() == curr) {
                    return false;
                }
                T& front = queue.front();
                queue.pop();
                queue.push(front);
            }
            queue.pop();

        }
        return true;
    }

    template <class T>
    void printQueue(std::queue<T>& queue) {
        while(!queue.empty()) {
            std::cout << queue.front() << " ";
            queue.pop();
        }
    }

    void reverse(std::queue<int>&);

    void printAscending(std::queue<int>);

    void printInterval(int* , int, int, int);

    std::string getPathBFS(int**, int, int, int);
}
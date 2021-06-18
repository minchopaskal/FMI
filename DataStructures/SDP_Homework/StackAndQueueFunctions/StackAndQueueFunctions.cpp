#include <queue>
#include <sstream>

#include "StackAndQueueFunctions.h"

/*
 * Stack Functions
 */
void StackFunctions::lastMerge(std::stack<int> *merged, std::stack<int> &last) {
    while (!last.empty()) {
        int val = last.top();
        merged->push(val);
        last.pop();
    }
}

std::stack<int>& StackFunctions::mergeStacks(std::stack<int>& leftStack, std::stack<int>& rightStack) {
    std::stack<int>* merged = new std::stack<int>();
    while(!leftStack.empty() && !rightStack.empty()) {
        int lTop = leftStack.top();
        int rTop = rightStack.top();
        if(lTop > rTop) {
            leftStack.pop();
            merged->push(lTop);
        } else {
            rightStack.pop();
            merged->push(rTop);
        }
    }

    if(leftStack.empty()) {
        lastMerge(merged, rightStack);
    }
    if(rightStack.empty()) {
        lastMerge(merged, leftStack);
    }

    return *merged;
}

int StackFunctions::getCount(std::string::iterator& it) {
    std::string currCountStr;
    while(*it != '(') {
        currCountStr += *it;
        ++it;
    }
    std::stringstream numStr(currCountStr);
    int currCount = 0;
    numStr >> currCount;
    return currCount;
}

std::string StackFunctions::decompressString(std::string str) {
    std::stack<std::string> strings;
    strings.push("");
    std::stack<int> multipliers;

    for(std::string::iterator it = str.begin(); it != str.end();) {
        if(*it == ')') {
            ++it;
        }
        else if(isdigit(*it)) {
            int currCount = getCount(it);
            multipliers.push(currCount);
            strings.push("");
            ++it;
        } else {
            strings.top() += *it;
            ++it;
        }
    }

    while(!multipliers.empty()) {
        const std::string& curr = strings.top();
        strings.pop();
        int currCount = multipliers.top();
        multipliers.pop();

        while(currCount > 0) {
            strings.top() += curr;
            --currCount;
        }
    }
    return strings.top();
}

/*
 * Queue Functions
 */
void QueueFunctions::reverse(std::queue<int>& queue) {
    std::stack<int> container;
    while(!queue.empty()) {
        int curr = queue.front();
        container.push(curr);
        queue.pop();
    }
    while(!container.empty()) {
        int curr = container.top();
        queue.push(curr);
        container.pop();
    }
}

void QueueFunctions::printAscending(std::queue<int> unordered) {
    int sentinel;
    std::queue<int> ordered;
    while(!unordered.empty()) {
        unordered.push(sentinel);
        int currMin = unordered.front();
        unordered.pop();
        while(unordered.front() != sentinel) {
            int curr = unordered.front();
            if(curr < currMin) {
                unordered.push(currMin);
                unordered.pop();
                currMin = curr;
            } else {
                unordered.push(curr);
                unordered.pop();
            }
        }
        unordered.pop();
        ordered.push(currMin);
    }
    printQueue(ordered);
    std::cout << std::endl;
}

void QueueFunctions::printInterval(int* arr, int size, int a, int b) {
    if(b < a) {
        std::swap(a, b);
    }

    std::queue<int> less;
    std::queue<int> between;
    std::queue<int> greater;

    for(int i = 0; i < size; ++i) {
        int curr = arr[i];
        if(curr < a) {
            less.push(curr);
        }
        else if(curr <= b) {
            between.push(curr);
        } else {
            greater.push(curr);
        }
    }

    QueueFunctions::printQueue(less);
    QueueFunctions::printQueue(between);
    QueueFunctions::printQueue(greater);
}

std::string QueueFunctions::getPathBFS(int** graph, int size, int i, int j) {
    i -= 1;
    j -= 1;
    auto traversed = new bool[size];
    // We start from i-th node
    traversed[i] = true;

    // Queue for BFS
    std::queue<int> toTraverse;
    toTraverse.push(i);

    /*
     * BFS
     */
    int depth = 1;
    while(!toTraverse.empty()) {
        ++depth;
        int curr = toTraverse.front();

        for(int k = 0; k < size; ++k) {
            if(graph[curr][k] && !traversed[k]) {
                toTraverse.push(k);
                // We put depth as value for each node
                graph[curr][k] = graph[k][curr] = depth;
                traversed[k] = true;
            }
        }
        toTraverse.pop();
        // Stop BFS on end node for optimization
        if(curr == j) {
            --depth;
            break;
        }
    }
    /*
     * End of BFS
     */
    for(int k = 0; k < size; ++k) {
        for (int l = 0; l < size; ++l)
            std::cout << graph[k][l] << " ";
        std::cout << std::endl;
    }

    // Get path from the depths of nodes
    std::stack<int> path;
    int curr = j;
    path.push(curr + 1);
    while(depth > 0){
        for(int k = 0; k < size; ++k) {
            if(graph[curr][k] == depth) {
                curr = k;
                path.push(curr + 1);
                break;
            }
        }

        --depth;
    }

    // Print path
    std::stringstream pathString;
    while(!path.empty()) {
        int currNode = path.top();
        path.pop();
        pathString << currNode << (path.empty() ? "" : " -> ");
    }

    return pathString.str();
}







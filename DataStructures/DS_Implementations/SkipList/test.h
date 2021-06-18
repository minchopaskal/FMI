#ifndef TESTS_H
#define TESTS_H

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <vector>

#include "bin_tree.cpp"
#include "skip_list.cpp"

#define ITERATE_VECTOR(obj, op, v) \
        for(auto it = (v).begin(); it != (v).end(); ++it) \
            ((obj).*(op))(*it);

#define ITERATE_INTERVAL(obj, op, beg, end) \
        for(int i = (beg); i <= (end); ++i) \
            ((obj).*(op))(i);

#define RUN_TEST(obj, op, v, name, size_name) \
        file << "========================================\n"; \
        file << (name) << '_' << (size_name) << std::endl; \
        auto s = std::chrono::high_resolution_clock::now(); \
        ITERATE_VECTOR((obj), (op), (v)) \
        auto e = std::chrono::high_resolution_clock::now(); \
        std::chrono::duration<double> d = e - s; \
        file << "Time: " << (name) << d.count() << std::endl; \
        file << "========================================\n\n";

#define RUN_TEST_INTERVAL(obj, op, beg, end, name, size_name) \
        file << "========================================\n"; \
        file << (name) << '_' << (size_name) << std::endl; \
        auto s = std::chrono::high_resolution_clock::now(); \
        ITERATE_INTERVAL((obj), (op), (beg), (end)) \
        auto e = std::chrono::high_resolution_clock::now(); \
        std::chrono::duration<double> d = e - s; \
        file << "Time: " << (name) << d.count() << std::endl; \
        file << "========================================\n\n";


class Test {
private:
    typedef std::vector<int> vec;
    typedef vec& vecRef;
    typedef const vecRef cvecRef;


    typedef bool (BinTree<int>::*treeFunc)(const int&);
    typedef bool (SkipList<int>::*slFunc)(const int&);

private:
    vec vectors[3];
    const char* sizes[3]{"small", "medium", "large"};
    std::ofstream file;

private:
    void runTree(treeFunc, const char*, bool, bool);
    void runSkipList(slFunc, const char*, double, bool, bool);

public:
    Test();

    void runTests();
};

#endif //SKIPLISTCLION_TESTS_H

#include "test.h"

Test::Test() {
    int size = 10;
    int factor = 100;
    for(vecRef v : vectors) {
        for(int j = 1; j <= size; ++j) {
            v.push_back(j);
        }
        std::random_shuffle(v.begin(), v.end());
        size *= factor;
        factor *= 10;
    }

}

void
Test::runTree(treeFunc tf,
              const char* name,
              bool random = false,
              bool populated = true) {
    int size = 10;
    int factor = 100;

    for(int i = 0; i < 3; ++i) {
        BinTree<int> t;
        if(populated) {
            for(int val : vectors[i]) {
                t.insert(val);
            }
        }

        if(random) {
            RUN_TEST(t, tf, vectors[i], name, sizes[i])
        } else {
            RUN_TEST_INTERVAL(t, tf, 1, size, name, sizes[i])
            size *= factor;
            factor /= 10; // <- if we don't do that all hopes are lost
        }
    }
}


void
Test::runSkipList(slFunc sf,
                  const char* name,
                  double probability,
                  bool random = false,
                  bool populated = true) {
    int size = 10;
    int factor = 100;

    for(int i = 0; i < 3; ++i) {
        SkipList<int> sl(vectors[i].size());
        if(populated) {
            for(int val : vectors[i]) {
                sl.insert(val);
            }
        }

        if(random) {
            RUN_TEST(sl, sf, vectors[i], name, sizes[i])
        } else {
            RUN_TEST_INTERVAL(sl, sf, 1, size, name, sizes[i])
            size *= factor;
            factor *= 10;
        }
    }
}


void
Test::runTests() {
/*  file.open("/home/ellie/Documents/C++/SkipList/tree_tests.txt");

    file << std::fixed;

    file << "Binary Tree:\n";
    runTree(&BinTree<int>::insert, "TreeInsert_Ordered", false, false);
    runTree(&BinTree<int>::insert, "TreeInsert_Random", true, false);
    runTree(&BinTree<int>::find, "TreeFind_Ordered");
    runTree(&BinTree<int>::find, "TreeFind_Random", true);
    runTree(&BinTree<int>::del, "TreeDelete_Ordered");
    runTree(&BinTree<int>::del, "TreeDelete_Random", true);

    file.close();

    file.open("/home/ellie/Documents/C++/SkipList/skip_list_tests_0.5.txt");

    file << std::fixed;

    file << "\nSkip List(given expected size + probability 0.5):\n";
    runSkipList(&SkipList<int>::insert, "SkipListInsert_Ordered", 0.5, false, false);
    runSkipList(&SkipList<int>::insert, "SkipListInsert_Random", 0.5, true, false);
    runSkipList(&SkipList<int>::find, "SkipListFind_Ordered", 0.5);
    runSkipList(&SkipList<int>::find, "SkipListFind_Random", 0.5, true);
    runSkipList(&SkipList<int>::del, "SkipListDelete_Ordered", 0.5);
    runSkipList(&SkipList<int>::del, "SkipListDelete_Random", 0.5, true);

    // Tests with the Skip List not given the expected size time out
    file.close();

    file.open("/home/ellie/Documents/C++/SkipList/skip_list_tests_0.4.txt");

    file << std::fixed;

    file << "\nSkip List(given expected size + probability 0.4):\n";
    runSkipList(&SkipList<int>::insert, "SkipListInsert_Ordered", 0.4, false, false);
    runSkipList(&SkipList<int>::insert, "SkipListInsert_Random", 0.4, true, false);
    runSkipList(&SkipList<int>::find, "SkipListFind_Ordered", 0.4);
    runSkipList(&SkipList<int>::find, "SkipListFind_Random", 0.4, true);
    runSkipList(&SkipList<int>::del, "SkipListDelete_Ordered", 0.4);
    runSkipList(&SkipList<int>::del, "SkipListDelete_Random", 0.4, true);

    file.close();

    file.open("/home/ellie/Documents/C++/SkipList/skip_list_tests_0.2.txt");

    file << std::fixed;

    file << "\nSkip List(given expected size + probability 0.2):\n";
    runSkipList(&SkipList<int>::insert, "SkipListInsert_Ordered", 0.2, false, false);
    runSkipList(&SkipList<int>::insert, "SkipListInsert_Random", 0.2, true, false);
    runSkipList(&SkipList<int>::find, "SkipListFind_Ordered", 0.2);
    runSkipList(&SkipList<int>::find, "SkipListFind_Random", 0.2, true);
    runSkipList(&SkipList<int>::del, "SkipListDelete_Ordered", 0.2);
    runSkipList(&SkipList<int>::del, "SkipListDelete_Random", 0.2, true);


    file.close();*/

    file.open("/home/ellie/Documents/C++/SkipList/skip_list_tests_0.6.txt");

    file << std::fixed;

    file << "\nSkip List(given expected size + probability 0.6):\n";
    runSkipList(&SkipList<int>::insert, "SkipListInsert_Ordered", 0.6, false, false);
    runSkipList(&SkipList<int>::insert, "SkipListInsert_Random", 0.6, true, false);
    runSkipList(&SkipList<int>::find, "SkipListFind_Ordered", 0.6);
    runSkipList(&SkipList<int>::find, "SkipListFind_Random", 0.6, true);
    runSkipList(&SkipList<int>::del, "SkipListDelete_Ordered", 0.6);
    runSkipList(&SkipList<int>::del, "SkipListDelete_Random", 0.6, true);

    // TODO Run tests again

}
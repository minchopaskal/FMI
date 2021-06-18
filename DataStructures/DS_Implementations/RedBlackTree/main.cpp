#include <chrono> 
#include <iostream>
#include "rbtree.cpp"

using rbti = red_black_tree<int>;

int main() {
  rbti t;

  t.insert(8);
  t.insert(10);
  t.insert(37);
  t.insert(2);
  t.insert(1);
  t.insert(90);
  t.insert(56);
  t.insert(98);
  t.insert(23);

  t.print();
}

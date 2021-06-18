#include <iostream>
#include <list>
#include <set>
#include <vector>

#include "functional.cpp"

void nl() {
  std::cout << std::endl;
}

template <class Container>
void print(const Container& c) {
  for(auto it = c.cbegin(); it != c.cend(); ++it) {
    std::cout << *it << " ";
  }
  nl();
}

int main() {
  std::vector<int> v { 1, 2, 3 };
  std::list<int> l { 2, 4, 6 };
  std::list<int> l2 { 10, 20, 30 };

  // Dereferencing set iterator returns ref to const obj
  // so map doesn't work here !
  /*
    std::set<int> s { 1, 2, 3, 4, 5 };
    Functional::map(s, [](int& el) -> void { el *= el + 1; });
    //                    ^^^^ 
   print(s); nl();
  */

  std::cout << "map ^2 { 1, 2, 3 }:\n";
  Functional::map(v, [](int& el) -> void { el *= el; });
  for(std::vector<int>::iterator it = v.begin(); it != v.end(); ++it) {
    std::cout << *it << " ";
  }
  nl();

  std::cout << "map +2 { 2, 4, 6}:\n";
  Functional::map(l, [](int& el) -> void { el += 2; });
  for(std::list<int>::iterator it = l.begin(); it != l.end(); ++it) {
    std::cout << *it << " ";
  }
  nl();

  std::cout << "reduce + { 1, 2, 3 }: "
            <<Functional::reduce(v.cbegin(), v.cend(), 0,
                                 [](int el, int sum) -> int { return el + sum; });
  nl();

  std::cout << "reduce * { 10, 20, 30}: "
            << Functional::reduce(l2, 1, [](int el, int p) -> int { return el * p; });
  nl();

  return 0;
}

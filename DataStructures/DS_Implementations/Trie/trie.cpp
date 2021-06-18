#ifndef TRIE_H
#define TRIE_H

#include <iostream>
#include <vector>
#include "../List/List.h"

template <typename T>
class Trie {
private:
    T data;
    List<Trie<T>> children;

    void prettyPrint(std::ostream& os = std::cout) {
        os << data;
        for(iterator it = begin(); it != end(); ++it) {
            os << " -> ";
            (*it).prettyPrint(os);
            os << ';';
        }
    }
public:
    using iterator = typename List<Trie<T>>::iterator;
    explicit Trie(T const& data = T()) : data(data) {}

    T root() const { return data; }

    Trie& addChild(Trie const& t) {
      children.push_back(t);
      return *this;
    }

    iterator begin() {
        return children.begin();
    }

    iterator end() {
        return children.end();
    }

    int distance(const T& el) {
        if(el == root()) {
            return 0;
        }
        for(iterator it = begin(); it != end(); ++it) {
            int d = (*it).distance(el);
            if(d >= 0) {
                return d + 1;
            }
        }
        return -1;
    }

    void printDOT(std::ostream& os = std::cout) {
        os << "{";
        prettyPrint(1, os);
        os << "}\n";
    }

};

#endif

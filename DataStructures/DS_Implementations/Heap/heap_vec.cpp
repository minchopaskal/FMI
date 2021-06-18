#ifndef HEAP_VEC_CPP
#define HEAP_VEC_CPP

#include <cassert>
#include <iostream>
#include <vector>
#include <utility>

template <class T, class Comparator>
class Heap {
private:
  std::vector<T> heap;

private:
  std::size_t parent(std::size_t pos) const {
    return (pos - 1) / 2;
  }
  
  std::size_t left(std::size_t pos) const {
    return 2 * pos + 1;
  }

  std::size_t right(std::size_t pos) const {
    return 2 * pos + 2;
  }

  bool hasChildren(std::size_t pos) const {
    if(left(pos) < size()) {
      return true;
    }
    return false;
  }

  void sink(std::size_t pos) {
    Comparator comp;
    std::size_t index = pos;

    while (index < size() && hasChildren(index)) { 
      std::size_t max = index;

      std::size_t leftChild = left(index);
      std::size_t rightChild = right(index);
      
      if(leftChild < heap.size() && comp(heap.at(index), heap.at(leftChild))) {
        max = leftChild;
      }

      if(rightChild < heap.size() && comp(heap.at(max), heap.at(rightChild))) {
        max = rightChild;
      }

      if(max == index) {
        index = heap.size();
      } else {
        std::swap(heap[index], heap[max]);
        index = max;
      }
    }
    
  }

  void floatUp(std::size_t pos) {
    Comparator comp;
    while(pos > 0 && comp(heap.at(parent(pos)), heap.at(pos))) {
      std::swap(heap[pos], heap[parent(pos)]);
      pos = parent(pos);
    }
  }
  
public:
  
  Heap(std::initializer_list<T> il) {
    for(auto it = il.begin(); it != il.end(); ++it) {
      insert(*it);
    }
  }

  Heap(std::vector<T>&& v) {
    for(auto it = v.begin(); it != v.end(); ++it) {
      insert(*it);
    }
  }

  bool empty() const {
    return heap.empty();
  }
  
  std::size_t size() const {
    return heap.size();
  }

  T top() const {
    return heap.front();
  }
  
  void pop() {
    std::swap(heap[0], heap[size() - 1]);
    heap.pop_back();
    sink(0);
  }

  /*void heapify() {
    for(int i = 0; i < size(); ++i) {
      sink(i);
    }
    printDOT();
  } */

  void insert(const T& data) {
    heap.push_back(data);
    floatUp(size() - 1);
  }

  void printDOT(std::ostream& os = std::cout) {
    os << "digraph Heap {\n";
    for(int i = 0; i < size(); ++i) {
      if(hasChildren(i)) {
        if(left(i) < size())
          os << heap.at(i) << " -> " << heap.at(left(i)) << ";\n";
        if(right(i) < size())
          os << heap.at(i) << " -> " << heap.at(right(i)) << ";\n";
      }
    }
    os << "}\n";
  }
  
};

class MaxHeapIntComparator {
public:
  bool operator() (int data1, int data2) {
    return data1 < data2;
  }
};
                                          
class MinHeapIntComparator {
public:
  bool operator() (int data1, int data2) {
    return data1 < data2;
  }
};

#endif


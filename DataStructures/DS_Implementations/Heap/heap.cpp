#ifndef HEAP_CPP
#define HEAP_CPP

#include <iostream>
#include <utility>
#include <vector>
#include <queue>

template <class T, class Compare>
class Heap {
private:
  struct node {
    T data;
    node* parent;
    node* left;
    node* right;
    node(T data, node* parent, node* left, node* right) :
      data(data), parent(parent), left(left), right(right) {}
  };

  node* root;

private:
  node* copy(node*, node*);
  
  void eraseHeap(node*);
  void bubbleUp(node*);
  void sinkDown(node*);
  void heapify(node*);
  void removeAll(node*, const T&);
  bool removeFirst(node*, const T&);
  node* getLast(node*);
  void deleteLast();
  
public:
  Heap();
  Heap(const Heap&);
  Heap& operator=(const Heap&);
  ~Heap();

  Heap& add(T data);
  void removeAll(const T& data);
  void removeFirst(const T& data);

  void pop();
  const T& getTop() const { return root->data; }

  std::vector<T> toVector() const;
};

template <class T, class Compare>
Heap<T, Compare>::Heap() : root(nullptr) {}

template <class T, class Compare>
Heap<T, Compare>::Heap(const Heap& other) : root(nullptr) {
  copy(other.root);
}

template <class T, class Compare>
Heap<T, Compare> & Heap<T, Compare>::operator=(const Heap& rhs) {
  if(this != &rhs) {
    eraseHeap(root);
    root = copy(root, rhs.root, nullptr);
  }
  return *this;
}

template <class T, class Compare>
Heap<T, Compare>::~Heap() {
  eraseHeap(root);
}

template <class T, class Compare>
typename Heap<T, Compare>::node* Heap<T, Compare>::copy(node* toCopy, node* parent) {
  if(toCopy == nullptr) {
    return nullptr;
  }
  node* copied = new node(toCopy->data, parent, nullptr, nullptr);
  copied->left = copy(toCopy->left, copied);
  copied->right = copy(toCopy->right, copied);
  return copied;
}

template <class T, class Compare>
void Heap<T, Compare>::eraseHeap(node* head) {
  if(head == nullptr) {
    return;
  }
  eraseHeap(head->left);
  eraseHeap(head->right);
  delete head;
}

template <class T, class Compare>
void Heap<T, Compare>::bubbleUp(node* toFloat)  {
  if(toFloat->parent == nullptr) {
    return;
  }
  Compare comp;
  if(comp(toFloat->data, toFloat->parent->data)) {
    return;
  }
  std::swap(toFloat->data, toFloat->parent->data);
  bubbleUp(toFloat->parent);
}

template <class T, class Compare>
void Heap<T, Compare>::sinkDown(node* toSink) {
  if(toSink->left == nullptr && toSink->right == nullptr) {
    return;
  }
  
  Compare comp;

  if(toSink->left != nullptr && comp(toSink->data, toSink->left->data)) {
    std::swap(toSink->data, toSink->left->data);
    sinkDown(toSink->left);
  } else if (toSink->right != nullptr && comp(toSink->data, toSink->right->data)) {
    std::swap(toSink->data, toSink->right->data);
    sinkDown(toSink->right);
  }
  return;
}

template <class T, class Compare>
void Heap<T, Compare>::heapify(node* curr) {
  // Leaf, so nothing to do in here.
  if(curr->left == nullptr && curr->right != nullptr) {
    return;
  }

  // Get `biger` child
  Compare comp;
  node* currMax = curr;
  bool right = false;
  if(curr->left != nullptr && comp(currMax->data, curr->left->data)) {
    currMax = curr->left;
  }

  if(curr->right != nullptr && comp(currMax->data, curr->left->data)) {
    currMax = curr->right;
    right = true;
  }

  // If curr is `max` element nothing to do.
  if(currMax == curr) {
    return;
  }

  // There is `bigger` than curr child so we swap the values...
  std::swap(curr->data, currMax->data);

  // ... and continue to recursivly heapify
  if(right) {
    heapify(curr->right);
  }
  heapify(curr->left);
}

template <class T, class Compare>
typename Heap<T, Compare>::node* Heap<T, Compare>::getLast(node* root) {
  // Leaf so we found the searched one
  if(root->left == nullptr && root->right == nullptr) {
    return root;
  }

  // If there is child to the right continue search
  if(root->right != nullptr) {
    return getLast(root->right);
  }

  // root->right is nullptr so for sure root->left is not
  return getLast(root->left);
}

template <class T, class Compare>
void Heap<T, Compare>::deleteLast() {
  node* last = getLast(root);

  // Before deleting the node we should make sure parents pointer is unhooked
  node* parent = last->parent;
  if(parent->left == last) {
    parent->left = nullptr;
  } else {
    parent->right = nullptr;
  }
  
  delete last;
}

template <class T, class Compare>
void Heap<T, Compare>::removeAll(node* curr, const T& data) {
  if(curr == nullptr) {
    return;
  }
  
  if(curr->data == data) {
    node* last = getLast(root);

    while(last->data == data) {
      deleteLast();
      last = getLast(root);
    }
    
    std::swap(curr->data, getLast(root)->data);
    deleteLast();
    sinkDown(curr);
  }
  
  removeAll(curr->left, data);
  removeAll(curr->right, data);
}

template <class T, class Compare>
bool Heap<T, Compare>::removeFirst(node* curr, const T& data) {
  if(curr == nullptr) {
    return false;
  }

  if(curr->data == data) {
    node* last = getLast(root);
    std::swap(curr->data, getLast(root)->data);
    deleteLast();
    // Sink down current element to keep heap ordering
    sinkDown(curr);
    return true;
  }
  if(!removeFirst(curr->left, data)) {
    removeFirst(curr->right, data);
  }
}

template <class T, class Compare>
Heap<T, Compare>& Heap<T, Compare>::add(T data) {
  if(root == nullptr) {
    root = new node(data, nullptr, nullptr, nullptr);
    return *this;
  }

  // Standart BFS
  std::queue<node*> bfs;
  bfs.push(root);
  while(!bfs.empty()) {
    node* curr = bfs.front();
    bfs.pop();

    if(curr->left == nullptr) {
      curr->left = new node(data, curr, nullptr, nullptr);
      bubbleUp(curr->left);
      return *this;
    } else {
      bfs.push(curr->left);
    }

    if(curr->right == nullptr) {
      curr->right = new node(data, curr, nullptr, nullptr);
      bubbleUp(curr->right);
      return *this;
    } else {
      bfs.push(curr->right);
    }
  }

  return *this;
}

template <class T, class Compare>
void Heap<T, Compare>::removeAll(const T& data) {
  removeAll(root, data);
  heapify(root);
}

template <class T, class Compare>
void Heap<T, Compare>::removeFirst(const T& data) {
  removeFirst(root, data);
  heapify(root);
}

template <class T, class Compare>
void Heap<T, Compare>::pop() {
  removeFirst(root->data);
}

template <class T, class Compare>
std::vector<T> Heap<T, Compare>::toVector() const {
  std::vector<T> heap;
  if(root == nullptr) {
    return heap;
  }

  // Standart BFS 
  std::queue<node*> q;
  q.push(root);
  while(!q.empty()) {
    node* top = q.front();
    const T& curr = top->data;
    q.pop();
    heap.push_back(curr);

    if(top->left != nullptr) {
      q.push(top->left);
    }

    if(top->right != nullptr) {
      q.push(top->right);
    }
  }
  return heap;
}

// Sample Comparator for `Max` heap
class MaxHeapComparator {
public:
  bool operator()(int data1, int data2) {
    return data1 < data2;
  }
};

#endif

#ifndef SPLAY_TREE_CPP
#define SPLAY_TREE_CPP

#include <cmath>
#include <functional>
#include <iostream>
#include <stack>
#include <vector>

template <class T, class Comparator = std::less<T>>
class SplayTree {
private:
  struct node {
    T data;
    node* left;
    node* right;
    
    node(const T& data, node* left = nullptr, node* right = nullptr) :
      data(data), left(left), right(right) {}
  };
  
public:
  SplayTree(node* root = nullptr) : root(root), size(0) {}
 
  SplayTree(const SplayTree& other) : root(nullptr), size(0) {
    root = copy(other.root);
    size = other.size;
  }

  SplayTree(SplayTree&& other) : root(std::move(other.root)), size(std::move(other.size)) {}

  ~SplayTree() {
    erase(root);
  }
  
  SplayTree&
  operator=(const SplayTree& rhs) {
    if (this != &rhs) {
      erase(root);
      root = copy(rhs.root);
      size = rhs.size;
    }
    return *this;
  }

  SplayTree&
  operator=(SplayTree&& rhs) {
    erase(root);
    root = nullptr;
    root = std::move(rhs.root);
    size = std::move(rhs.size);
    return *this;
  }

  bool
  search(const T& data) {
    _search(data);
    return root != nullptr && root->data == data;
  }

  bool
  insert(const T& data) {
    ++size;
    root = _insert(root, data);
    _search(data);
    return true; // for testing purposes
  }

  // for testing
  template <typename... Args> void 
  insert(const T& data, Args... args) {
    insert(data);
    insert(args...);
  }

  bool
  del(const T& data) {
    _search(data);
    if (root == nullptr || root->data != data) {
      return false;
    }
    
    node* leftMaxParent = find_max(root->left);
    node* tmp;
    if (leftMaxParent == nullptr) { // root has no left subtree
      tmp = root;
      root = root->right;
    } else if (leftMaxParent->right == nullptr) { // left child is biggest in the left subtree
      tmp = root;
      root = root->left;
      root->right = tmp->right;
    } else {
      root->data = leftMaxParent->right->data;
      tmp = leftMaxParent->right;
      leftMaxParent->right = tmp->left;
    }
    
    delete tmp;
    --size;
    return true;
  }

  bool
  empty() const {
    return size == 0;
  }
  
  void
  print() const {
    print(root);
    std::cout << std::endl;
  }
  
private:
  node* find_max(node* curr) {
    if (curr == nullptr || curr->right == nullptr) {
      return curr;
    }

    while (curr->right->right != nullptr) {
      curr = curr->right;
    }
    return curr;
  }
  
  void
  rotate_right(node*& parent, node* g_parent) {
    node* child = parent->left;
    parent->left = child->right;
    child->right = parent;

    // Fix grandparent ptr
    if (g_parent) {
      if (g_parent->left == parent) {
        g_parent->left = child;
      } else {
        g_parent->right = child;
      }
    }
    parent = child;
  }

  void
  rotate_left(node*& parent, node* g_parent) {
    node* child = parent->right;
    parent->right = child->left;
    child->left = parent;

    // Fix grandparent ptr
    if (g_parent) {
      if (g_parent->left == parent) {
        g_parent->left = child;
      } else {
        g_parent->right = child;
      }
    }
    parent = child;
  }
  
  
  void
  _search(const T& data) {
    if (root == nullptr || root->data == data) {
      return;
    }
    
    int arrSize = 2 * floor(log2l(size));
    node** arr = new node*[arrSize];
    int i = 0;
    arr[i] = root;

    // collect the path to the node
    while (arr[i] != nullptr && arr[i]->data != data) {
      while (i + 1 >= arrSize) {
        arr = resizeArr(arr, arrSize);
      }
      
      if (cmp(arr[i]->data, data)) {
        arr[i+1] = arr[i]->right;
      } else {
        arr[i+1] = arr[i]->left;
      }
      
      ++i;
    }
    
    // if last element is nullptr the data
    // is not in the tree - we move the closest
    // one to it on top
    if (arr[i] == nullptr) {
      --i;
    }
    
    // else it is and we should make rotations
    node* g_parent;
    while (i > 0) {
      bool isLeftChild = arr[i-1]->left == arr[i];

      if (i == 1) {
        if (isLeftChild) {
          rotate_right(arr[i-1], nullptr);
        } else {
          rotate_left(arr[i-1], nullptr);
        }
      } else if (isLeftChild) {
        if (arr[i-2]->left == arr[i-1]) {
          if (i > 2) g_parent = arr[i-3];
          else       g_parent = nullptr;
          
          rotate_right(arr[i-2], g_parent);
          rotate_right(arr[i-2], g_parent);
        } else { // arr[i-2]->right == arr[i-1]
          rotate_right(arr[i-1], arr[i-2]);

          if (i > 2) g_parent = arr[i-3];
          else g_parent = nullptr;

          rotate_left(arr[i-2], g_parent);
        }
      } else { // arr[i-1]->right == arr[i]
        if (arr[i-2]->right == arr[i-1]) {
          if (i > 2) g_parent = arr[i-3];
          else       g_parent = nullptr;

          rotate_left(arr[i-2], g_parent);
          rotate_left(arr[i-2], g_parent);
        } else { // arr[i-2]->left == arr[i-1]
          rotate_left(arr[i-1], arr[i-2]);

          if (i > 2) g_parent = arr[i-3];
          else       g_parent = nullptr;

          rotate_right(arr[i-2], g_parent);
        }
      }
      i -= 2;
    }

    root = arr[0];
    delete[] arr;
  }

  node*
  _insert(node* curr, const T& data) {
    if (curr == nullptr) {
      return new node(data);
    }
    
    if (cmp(data, curr->data)) {
      curr->left = _insert(curr->left, data);
    } else {
      curr->right = _insert(curr->right, data);
    }
    return curr;
  }

  node**
  resizeArr(node** arr, int& s) {
    node** newArr = new node*[2 * s];
    for (int i = 0; i < s; ++i) {
      newArr[i] = arr[i];
    }

    s *= 2;
    delete[] arr;
    return newArr;
  }
  
  void
  erase(node* root) {
    if (root == nullptr) {
      return;
    }

    std::stack<node*> s;
    while(!s.empty()) {
      node* curr = s.top();
      s.pop();
      if(curr->left) s.push(curr->left);
      if(curr->right) s.push(curr->right);
      delete curr;
    }
  }

  node*
  copy(const node* toCopy) {
    if (toCopy == nullptr) {
      return nullptr;
    }
    
    node* curr = new node(toCopy->data, copy(toCopy->left), copy(toCopy->right));
    return curr;
  }
                          
  void
  print(const node* curr, std::ostream& os = std::cout) const {
    if (curr == nullptr) {
      os << "()";
      return;
    }
    
    os << '(' << curr->data << ' ';
    print(curr->left);
    os << ' ';
    print(curr->right);
    os << ')';
  }
  
private:
  node* root;
  std::size_t size;
  Comparator cmp;
};

#endif

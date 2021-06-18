#include <functional> // std::less
#include <iostream>   // for debuging
#include <utility>    // std::move

#include "utils.h"

template <class T, class Compare = std::less<T>>
class red_black_tree {
public:
  using color_t = bool;
  
private:
  struct node {
    T data;
#if defined(HAS_COLOR_MEMBER_DATA)
    color_t color;
#endif
    node *left, *right;

    node (const T& data = T(),
          color_t color = RED,
          node* left = nullptr,
          node* right = nullptr)
      : data(data),
        left(left),
        right(right)
    {
      set_color(color);
    }

#if defined(HAS_COLOR_MEMBER_DATA)
    inline node* get_left() { return left; }

    inline void set_left(node* newLeft) {
      left = newLeft;
    }

    inline void set_color(color_t color) { this->color = color; }
    inline bool is_red() const { return color; } 
#else
    // We ASSUME that the compiler aligns the memory
    
    node* get_left() {
      node* tmp = left;
      return reinterpret_cast<node*>(reinterpret_cast<std::size_t>(tmp) & ~1);
    }

    void set_left(node* newLeft) {
      color_t c = is_red();
      left = newLeft;
      set_color(c);
    }

    void set_color(color_t color) {
      std::size_t addr = reinterpret_cast<std::size_t>(left);
      if (color == RED) {
        left = reinterpret_cast<node*>(addr | 1);
      } else {
        left = reinterpret_cast<node*>(addr & ~1);
      }
    }

    
    inline bool is_red() const {
      return reinterpret_cast<std::size_t>(left) & 1;
    } 
#endif
    // for consistency:
    inline node* get_right() { return right; }
    inline void set_right(node* newRight) { right = newRight; } 
  };

private:
  node* root;
  Compare compare;
  node** backtrack;
  std::size_t backtrackSize;

public:
  // ==================== Constructors ==================== //
  red_black_tree() :
    root(nullptr),
    backtrack(nullptr),
    backtrackSize(20) /* ~~ 2 * log (1 000 000 000) */
  { }

  red_black_tree(const red_black_tree& other) :
    root(nullptr),
    backtrack(nullptr),
    backtrackSize(other.backtrackSize)
  {
    root = copy(other.root);
  }

  red_black_tree(red_black_tree&& other) :
    root(std::move(other.root)),
    backtrack(nullptr),
    backtrackSize(std::move(other.backtrackSize))
  { }

  // =============== Assignment Operators ================ //
  red_black_tree& operator=(const red_black_tree& rhs) {
    if (this != &rhs) {
      erase(root);
      root = copy(rhs.root);
      backtrackSize = rhs.backtrackSize;
    }

    return *this;
  }

  red_black_tree& operator=(red_black_tree&& rhs) {
    if (this != &rhs) {
      erase(root);
      root = nullptr;
      root = std::move(rhs.root);
      backtrackSize = std::move(rhs.backtrackSize);
    }

    return *this;
  }

  // =================== Destructor =================== //
  ~red_black_tree() {
    if (backtrack != nullptr) delete [] backtrack;
    erase(root);
  }
  
  // ============= Basic Tree Operations ============= //
  void insert(const T& value) {
    std::size_t index;
    if (backtrack != nullptr) {
      delete[] backtrack;
    }
    backtrack = new node*[backtrackSize];
    search_with_backtrack(value, index);
    
    if (index == 0) {
      root = new node(value, BLACK);
      return;
    }

    node* newNode = new node(value);
    if (compare(value, backtrack[index-1]->data)) {
      backtrack[index-1]->set_left(newNode);
    } else {
      backtrack[index-1]->set_right(newNode);
    }
    backtrack[index] = newNode;
    insert_fix(index);       
  }
  
  bool remove(const T& value) {
    std::size_t index;
    search_with_backtrack(value, index, true);

    if (backtrack[index]->data != value) { // We didn't find the data
      return false;
    }

    node* toFix;
    color_t toFixColor = backtrack[index]->is_red();
    if (backtrack[index]->get_left() == nullptr) {
      toFix = backtrack[index]->get_right();
      transplant(backtrack[index],
                 index > 0 ? backtrack[index-1] : nullptr,
                 toFix);
      backtrack[index] = toFix;
    }
    else if (backtrack[index]->get_right() == nullptr) {
      toFix = backtrack[index]->get_left();
      transplant(backtrack[index],
                 index > 0 ? backtrack[index-1] : nullptr,
                 toFix);
      backtrack[index] = toFix;
    }
    else {
      std::size_t oldIndex = index; // the node we were to remove
      node* toReplaceWith = find_min_node(backtrack[index]->get_right(), index);

      backtrack[oldIndex]->data = toReplaceWith->data;
      toFix = (backtrack[index] = backtrack[index]->get_right());

      // We would actually remove that node so we care about
      // ITS color
      toFixColor = toReplaceWith->is_red();
      
      if (index == oldIndex + 1) { // toReplaceWith is direct child of the node we remove
        transplant(toReplaceWith, backtrack[oldIndex], toFix);
      } else { // ToReplaceWith is not a direct child of the node we remove
        transplant(toReplaceWith, backtrack[index-1], toFix);        
      }
      
    }

    /*if (toFix != nullptr && toFixColor == BLACK) {
      return remove_fix(index);
      }*/
    return true;
  }

  bool search(const T& value) const {
    node* curr = root;
    while (curr != nullptr) {
      if (curr->data == value) {
        return true;
      }
      
      if (compare(value, curr->data)) {
        curr = curr->get_left();
      } else {
        curr = curr->get_right();
      }
    }

    return false;
  }
#if defined(DEBUG_MODE)
  void print() const {
    print(root);
    std::cout << std::endl;
  }
#endif

private:
  // ================ Tree Fixup Methods ================ //
  void remove_fix(int curr) {

  }
  
  void insert_fix(int curr) {
    /* backtrack[curr] == current node
     * backtrack[curr-1] == it's parent
     * backtrack[curr-2] == curr node's grandparent
     */
    node *parent, *gparent;
    while (curr > 0 && (parent = backtrack[curr-1])->is_red()) {
      if (parent == (gparent = backtrack[curr-2])->get_left()) {
        node* uncle = gparent->get_right();
        if (uncle != nullptr && uncle->is_red()) {
          // Drop the blackness down the tree
          parent->set_color(BLACK);
          uncle->set_color(BLACK);
          gparent->set_color(RED);
          curr = curr - 2;
        } else { // Rotate so that we drop the height by 1
          if (backtrack[curr] == parent->get_right()) {
            rotate_left(parent, gparent);
          }
          parent->set_color(BLACK);
          gparent->set_color(RED);
          rotate_right(gparent, (curr < 3 ? nullptr : backtrack[curr-3]));
          curr -= 3;
        }
      } else { // parent is right child of grand-parent
        node* uncle = gparent->get_left();
        if (uncle != nullptr && uncle->is_red()) {
          parent->set_color(BLACK);
          uncle->set_color(BLACK);
          gparent->set_color(RED);
          curr = curr - 2;
        } else {
          if (backtrack[curr] == parent->get_left()) {
            rotate_right(parent, gparent);
          }
          parent->set_color(BLACK);
          gparent->set_color(RED);
          rotate_left(gparent, (curr < 3 ? nullptr : backtrack[curr-3]));
          curr -= 3;
        }
      }
    }
    root->set_color(BLACK);
  }

private:
  // ================== Helper Methods ================== //
#if defined(DEBUG_MODE)
  char name(bool c) const {
    return c ? 'r' : 'b';
  }
  
  void print(node* curr) const {
    if (curr == nullptr) {
      std::cout << "NIL";
      return;
    }
    std::cout << "([" << curr->data << ", " << name(curr->is_red()) << "] ";
    print(curr->get_left());
    std::cout << ' ';
    print(curr->get_right());
    std::cout << ")";
    return;
  }
#endif
  
  node* copy(node* other) {
    if (other == nullptr) {
      return nullptr;
    }

    node* newNode = new node();
    newNode->set_left(copy(other->get_left()));
    newNode->set_right(copy(other->get_right()));
    newNode->data = other->data;
    newNode->set_color(other->get_color());

    return newNode;
  }

  void erase(node* curr) {
    if (curr == nullptr) {
      return;
    }

    erase(curr->get_left());
    erase(curr->get_right());
    delete curr;
    
  }

  void search_with_backtrack(const T& value, std::size_t& index, bool match = false) {
    index = 0;
    backtrack[0] = root;

    while (backtrack[index] != nullptr) {
      ++index;
      if (compare(value, backtrack[index-1]->data)) {
        backtrack[index] = backtrack[index-1]->get_left();
      } else {
        if (match && value == backtrack[index-1]->data) {
          --index;
          return;
        }
        backtrack[index] = backtrack[index-1]->get_right();
      }
      
      if (index == backtrackSize) {
        resize_array(backtrack, backtrackSize);
      }
    }
  }

  node* find_min_node(node* curr, std::size_t& index) {
    if (curr == nullptr) {
      return nullptr;
    }

    do {
      if (++index == backtrackSize) {
        resize_array(backtrack, backtrackSize);
      }
      
      backtrack[index] = curr;
    
    } while (curr->get_left() != nullptr && (curr = curr->get_left()));

    return curr;
  }
  
  void transplant(node* n, node* nParent, node* toTransplant) {
    if (nParent == nullptr) {
      root = toTransplant;
    } else if (nParent->get_left() == n) {
      nParent->set_left(toTransplant);
    } else {
      nParent->set_right(toTransplant);
    }
    delete n;
  }

  void rotate_left(node*& toRotate, node* parent) {
    node* rightChild = toRotate->get_right();

    if (parent == nullptr) {
      root = rightChild;
    } else  if (parent->get_left() == toRotate) {
      parent->set_left(rightChild);
    } else {
      parent->set_right(rightChild);
    }
    
    toRotate->set_right(rightChild->get_left());
    rightChild->set_left(toRotate);
    toRotate = rightChild;
  }

  void rotate_right(node*& toRotate, node* parent) {
    node* leftChild = toRotate->get_left();

    if (parent == nullptr) {
      root = leftChild;
    } else if (parent->get_left() == toRotate) {
      parent->set_left(leftChild);
    } else {
      parent->set_right(leftChild);
    }
    
    toRotate->set_left(leftChild->get_right());
    leftChild->set_right(toRotate);
    toRotate = leftChild;
  }
};





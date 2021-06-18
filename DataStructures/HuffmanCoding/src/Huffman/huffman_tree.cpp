#include "huffman_tree.h"
using namespace Huffman;

/* HuffmanTree::iterator */
using iterator = HuffmanTree::iterator;

iterator::iterator(HuffmanTree::node* ptr) : ptr(ptr) {}

bool iterator::is_valid() const {
  return ptr != nullptr;
}

void
iterator::nextLeft() {
  if(is_valid()) {
    ptr = ptr->left;
  }
}

void
iterator::nextRight() {
  if(is_valid()) {
    ptr = ptr->right;
  }
}

iterator&
iterator::operator--() {
  nextLeft();
  return *this;
}

iterator&
iterator::operator++() {
  nextRight();
  return *this;
}

iterator
iterator::operator--(int) {
  iterator tmp(*this);
  nextLeft();
  return tmp;
}

iterator
iterator::operator++(int) {
  iterator tmp(*this);
  nextRight();
  return tmp;
}

bool
iterator::operator==(const iterator& other) const {
  return ptr == other.ptr;
}

bool
iterator::operator!=(const iterator& other) const {
  return ptr != other.ptr;
}

const HuffmanTree::node&
iterator::operator*() {
  return *ptr;
}

const HuffmanTree::node*
iterator::operator->() const {
  return ptr;
}
/* End HuffmanTree::iterator */

HuffmanTree::HuffmanTree() : root(nullptr), size_tree(0) {}

HuffmanTree::HuffmanTree(const HuffmanTable& frequencyTable)
  : root(nullptr), size_tree(0)
{
  buildTree(frequencyTable.getTable());
  size_tree = size(root);
}

HuffmanTree::HuffmanTree(std::stringstream& treeEncoding) : root(nullptr), size_tree(0) {
  root = readTree(treeEncoding);
  size_tree = size(root);
}

HuffmanTree::HuffmanTree(const std::string& treeEncoding) : root(nullptr), size_tree(0) {
  std::stringstream ss(treeEncoding);
  root = readTree(ss);
  size_tree = size(root);
}

HuffmanTree::HuffmanTree(const HuffmanTree& other) : root(nullptr), size_tree(0) {
  root = copy(other.root, root);
  size_tree = other.size_tree;
}

HuffmanTree::HuffmanTree(HuffmanTree&& other)
  : root(nullptr), size_tree(std::move(other.size_tree)) {
  std::swap(root, other.root);
}

HuffmanTree&
HuffmanTree::operator=(const HuffmanTree& rhs) {
  if(this != &rhs) {
    clear();
    root = copy(rhs.root, root);
    size_tree = rhs.size_tree;
  }
  return *this;
}

HuffmanTree&
HuffmanTree::operator=(HuffmanTree&& rhs) {
  clear();
  root = nullptr; // Crucial for rhs's destructor
  std::swap(root, rhs.root);
  size_tree = std::move(rhs.size_tree);
}

HuffmanTree::~HuffmanTree() {
  clear();
}

HuffmanTree::node*
HuffmanTree::copy(const node* currFrom, node* currTo) {
  if(currFrom == nullptr) {
    return nullptr;
  }
  currTo = new node(currFrom->symbol,
                    currFrom->freq,
                    copy(currFrom->left, nullptr),
                    copy(currFrom->right, nullptr));
  return currTo;
}

void
HuffmanTree::clear() {
  if(root == nullptr) {
    return;
  }
  
  std::queue<node*> DFS;
  DFS.push(root);

  while(!DFS.empty()) {
    node* curr = DFS.front();
    DFS.pop();
    if(curr->isLeaf()) {
      delete curr;
    } else {
      if(curr->left) {
        DFS.push(curr->left);
      }
      if(curr->right) {
        DFS.push(curr->right);
      }
      delete curr;
    }
  }
}

uint
HuffmanTree::countLeaves(const node* curr) const {
  if(curr == nullptr) {
    return 0;
  }

  if(curr->isLeaf()) {
    return 1;
  }

  return countLeaves(curr->left) + countLeaves(curr->right);
}

void
HuffmanTree::buildTree(const hTable& frequencyTable) {
  uint count = frequencyTable.size();

  if(count == 0) {
    return;
  }
  
  // Make a heap from the huffman hash table
  std::priority_queue<node*, std::vector<node*>, NodeComparator> heap;
  for(auto it = frequencyTable.begin(); it != frequencyTable.end(); ++it) {
    node* leafNode = new node(it->first, it->second);
    heap.push(leafNode);
  }
  
  // There are n elements so we will build n - 1 connections
  // in the binary treey
  for (uint i = 0; i < count - 1; ++i) {
    node* left = heap.top();
    heap.pop();
    node* right = heap.top();
    heap.pop();
    
    node* parentNode = new node(VOID_SYMBOL,
                                left->freq + right->freq,
                                left,
                                right);
    heap.push(parentNode);
  }

  // Only one element is left in the heap
  root = heap.top();
}

std::string
HuffmanTree::getCode(node* curr, const std::string& trace, char c) const {
  if (curr->isLeaf()) {
    if (curr->symbol == c) {
      return trace;
    }
    return "nil";
  }

  // If not leaf children are non-null for sure
  std::string leftStr = trace + '0';
  std::string left = getCode(curr->left, leftStr, c);
  if(left != "nil") {
    return left;
  }
  
  std::string rightStr = trace + '1';
  std::string right = getCode(curr->right, rightStr, c);
  if(right != "nil") {
    return right;
  }

  return "nil";
}

std::string
HuffmanTree::getCode(char c) const {
  return getCode(root, "", c);
}

iterator
HuffmanTree::begin() const {
  return it(root);
}

iterator
HuffmanTree::end() const {
  return nullptr;
}

hCoding*
HuffmanTree::getCodeTable() const {
  if(root == nullptr) {
    return nullptr;
  }

  hCoding* table = new hCoding;
  
  std::queue<node*> DFS;
  DFS.push(root);
  
  while(!DFS.empty()) {
    node* curr = DFS.front();
    DFS.pop();
    if(curr->isLeaf()) {
      char symbol = curr->symbol;
      (*table)[symbol] = getCode(symbol);
    } else {
      if(curr->left) {
        DFS.push(curr->left);
      }
      if(curr->right) {
        DFS.push(curr->right);
      }
    }
  }
  return table;
}


/*
 * Tree encoding algo:
 * 8 bits for the count of bits of the tree representation.
 * If node isnt't leaf put 0. Then put left child, then right one.
 * If node is leaf put 1 then sizeof(char) bits representing the char
 * of the leaf.
 */ 
std::string
HuffmanTree::encodeTree(uint& treeEncodingCountBits) const {
  std::stringstream stream;

  // Encoding of tree could be at most 320B = 2560 bits so 2B should
  // be enough to store such a number;
  uint leaves = countLeaves(root);
  treeEncodingCountBits = size_tree + leaves * BYTE_SIZE;
  std::bitset<2 * BYTE_SIZE> numOfBitsAsBits(treeEncodingCountBits);

  stream << numOfBitsAsBits.to_string();

  std::queue<node*> BFS;
  BFS.push(root);
  while(!BFS.empty()) {
    node* curr = BFS.front();
    BFS.pop();
    
    if(curr->isLeaf()) {
      stream << '1';

      // Get the binary representation of the current char
      std::bitset<BYTE_SIZE> currSymbol(curr->symbol);
      
      stream << currSymbol.to_string();
    } else {
      stream << '0';
      
      if(curr->left) {
        BFS.push(curr->left);
      }

      if(curr->right) {
        BFS.push(curr->right);
      }
    }
  }

  return stream.str();
}

void
HuffmanTree::readTree(const std::string& code) {
  clear();
  std::stringstream codeStream(code);
  root = readTree(codeStream);
  size_tree = size(root);
}

HuffmanTree::node*
HuffmanTree::readTree(std::stringstream& code) {
  char c;
  // Edge case: Last non-leaf has only one child
  if(!(code >> c)) {
    return nullptr;
  }
  
  node* curr = new node();
  /*
   * Read the node's value
   * if 0 -> hasChildren
   * if 1 -> isLeaf so return
   */
  if(c == '1') {
    // We have to read the leaf's symbol, which is 1 byte - 8 bits
    std::string currSymbolBits;
    for(int i = 0; i < BYTE_SIZE; ++i) {
      c = code.get();
      currSymbolBits.push_back(c);
      std::bitset<BYTE_SIZE> currByte(currSymbolBits);
      int currByteInt = currByte.to_ulong();
      c = currByteInt;
    }
    curr->symbol = c;
    return curr;
  }
  // Else c == '0'
  node* left = readTree(code);
  node* right = readTree(code);

  curr->left = left;
  curr->right = right;
  return curr;
}

uint
HuffmanTree::size() const {
  return size_tree;
}

uint
HuffmanTree::size(const node* curr) const {
  if(curr == nullptr) {
    return 0;
  }

  return 1 + size(curr->left) + size(curr->right);
}
                       

void
HuffmanTree::printDOT() const {
  if(root == nullptr) {
    std::cout << "Empty!\n";
    return;
  }
  
  std::queue<node*> BFS;
  BFS.push(root);
  std::cout << "digraph {\n";

  while(!BFS.empty()) {
    node* curr = BFS.front();

    if (curr->left) {
      std::cout << curr->symbol << curr->freq
                << " -> " << curr->left->symbol
                << curr->left->freq << std::endl;
      BFS.push(curr->left);
    }

    if(curr->right) {
      std::cout << curr->symbol << curr->freq
                << " -> " << curr->right->symbol
                << curr->right->freq << std::endl;
      BFS.push(curr->right);
    }

    BFS.pop();
    
  }

  std::cout << "}\n";
}

void
HuffmanTree::printLISP() const {
  printLISP(root);
}

void
HuffmanTree::printLISP(const node* root) const {
  if(root == nullptr) {
    std::cout << "()";
    return;
  }
  std::cout << '(' << root->symbol;
  std::cout << ' ';
  printLISP(root->left);
  std::cout << ' ';
  printLISP(root->right);
  std::cout << ')';
}

#ifndef HUFFMAN_TREE_H
#define HUFFMAN_TREE_H

#include <bitset>
#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <vector>

#include "huffman.h"
#include "huffman_table.h"

namespace Huffman {

  class HuffmanTree {
  private:
    struct node {
      char symbol;
      uint freq;
      node* left;
      node* right;
      
      node(char symbol = VOID_SYMBOL, uint freq = 0, node* left = nullptr, node* right = nullptr) :
        symbol(symbol), freq(freq), left(left), right(right) {}

      bool isLeaf() const {
        return !left && !right;
      }
    };

    /* 
     * Basically std::greater over node*.
     * Used for min-heap.
     */
    class NodeComparator {
    public:
      bool operator()(node* n1, node* n2) {
        return n1->freq > n2->freq;
      }
    };

  public:
    class iterator {
    private:
      const HuffmanTree::node* ptr;
    public:
      iterator(HuffmanTree::node*);

      bool is_valid() const;
      
      void nextLeft();
      void nextRight();

      iterator& operator--();
      iterator& operator++();

      iterator operator--(int);
      iterator operator++(int);

      bool operator==(const iterator&) const;
      bool operator!=(const iterator&) const;

      const HuffmanTree::node& operator*();
      const HuffmanTree::node* operator->() const;
    };
  
  public:
    using it = iterator;
    
    // For Adaptive huffman coding:
    //HuffmanTree(const std::string&);

    HuffmanTree();
    
    // Standart Huffman coding
    HuffmanTree(const HuffmanTable&);
    HuffmanTree(std::stringstream&);
    HuffmanTree(const std::string&);

    HuffmanTree(const HuffmanTree&);
    HuffmanTree(HuffmanTree&&);
    HuffmanTree& operator=(const HuffmanTree&);
    HuffmanTree& operator=(HuffmanTree&&);
    ~HuffmanTree();
    
    // Element access
    std::string getCode(char) const;

    // Iterators
    it begin() const;
    it end() const;

    // Algorithm specific methods
    hCoding* getCodeTable() const;

    // Read HuffmanTree from a file
    void readTree(const std::string&);

    // Encode Huffman tree
    std::string encodeTree(uint&) const;

    // Capacity
    uint size() const;
    
    // Testing
    // TODO remove after done testing
    void printDOT() const;
    void printLISP() const;

  private:
    node* root;
    uint size_tree; // No need of std::size_t as tree could only get 511 nodes big

  private:
    void clear();
    node* copy(const node*, node*);
    
    void buildTree(const hTable&);
    std::string getCode(node*, const std::string&, char) const;
    uint countLeaves(const node*) const;
    uint size(const node*) const;

    node* readTree(std::stringstream&);

    void printLISP(const node *) const;
  };
  
};

#endif

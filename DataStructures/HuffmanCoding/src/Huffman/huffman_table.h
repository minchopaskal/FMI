#ifndef HUFFMAN_TABLE_H
#define HUFFMAN_TABLE_H

#include <fstream>
#include <iostream>

#include "huffman.h"

namespace Huffman {
  
  class HuffmanTable {
  private:
    hTable frequencyTable;

    void clear();
    void buildTableString(const std::string&);
    bool buildTableFile(const std::string&);
    
  public:
    HuffmanTable();
    HuffmanTable(const std::string&, bool isFile = true);

    bool readFile(const std::string&);
    void readString(const std::string&);

    const hTable& getTable() const;
    std::size_t size() const;
    bool empty() const;
  };
  
};

#endif

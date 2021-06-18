#ifndef HUFFMAN_CODES_H
#define HUFFMAN_CODES_H

#include <fstream>

#include "huffman.h"
#include "huffman_table.h"
#include "huffman_tree.h"

namespace Huffman {

  class HuffmanCodes {
  private:
    HuffmanTable freqTable;
    HuffmanTree ht;
    hCoding codeTable;
    
    std::string inputFile;
    std::string outputFile;

    bool encodeMode;

    void buildCodeTable();
    bool encodeText(std::stringstream&);

    void streamTreeEncoding(std::stringstream&, std::stringstream&) {}
    bool iterateOverTree(const HuffmanTree&, std::ifstream&, std::ofstream&);
    
  public:
    HuffmanCodes(const std::string&, const std::string&, bool encodeMode = true);
    
    bool encode();
    bool decode();
  };

};

#endif

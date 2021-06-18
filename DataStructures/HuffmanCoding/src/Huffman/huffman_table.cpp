#include "huffman_table.h"
using namespace Huffman;

HuffmanTable::HuffmanTable() {}

HuffmanTable::HuffmanTable(const std::string& str, bool isFile) {
  if(isFile) {
    if(!buildTableFile(str)) {
      std::cout << "Unable to build table! Check file name!\n";
    }
  } else {
    buildTableString(str);
  }
}

void
HuffmanTable::clear() {
  frequencyTable.clear();
}
                    

bool
HuffmanTable::buildTableFile(const std::string& ifile) {
  std::ifstream ifs(ifile, std::ios::in);

  if(ifs.is_open()) {
    char c;
    while(ifs.get(c)) {
      ++frequencyTable[c];
    }

    return true;
  }

  return false;
}

void
HuffmanTable::buildTableString(const std::string& s) {
  for(auto it = s.begin(); it != s.end(); ++it) {
    ++frequencyTable[*it];
  }
}

bool
HuffmanTable::readFile(const std::string& ifile) {
  clear();
  if(buildTableFile(ifile)) {
    return true;
  }
  
  std::cout << "Unable to build table!\n";
  return false;
}

void
HuffmanTable::readString(const std::string& s) {
  clear();
  buildTableString(s);
}

const hTable&
HuffmanTable::getTable() const {
  return frequencyTable;
}

std::size_t
HuffmanTable::size() const {
  return frequencyTable.size();
}

bool
HuffmanTable::empty() const {
  return frequencyTable.empty();
}
    

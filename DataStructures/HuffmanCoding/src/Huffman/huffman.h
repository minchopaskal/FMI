#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <unordered_map>

namespace Huffman {
  /* Constants */
  const char VOID_SYMBOL = '`';
  const uint BYTE_SIZE = sizeof(char) * 8;

  /* Typedefs */
  using uint = unsigned int;
  using hTable = std::unordered_map<char, uint>;
  using hCoding = std::unordered_map<char, std::string>;
};

#endif

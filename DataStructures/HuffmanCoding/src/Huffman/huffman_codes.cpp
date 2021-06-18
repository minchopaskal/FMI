#include "huffman_codes.h"
using namespace Huffman;

HuffmanCodes::HuffmanCodes(const std::string& inputFile,
                           const std::string& outputFile,
                           bool encodeMode)
  : inputFile(inputFile), outputFile(outputFile),
    encodeMode(encodeMode) {}

bool
HuffmanCodes::encodeText(std::stringstream& code) {
  std::ifstream input(inputFile, std::ios::in);
  std::ofstream output(outputFile, std::ios::out | std::ios::binary);
  
  if(!input.is_open() || !output.is_open()) {
    std::cout << "Unable to open files!\n";
    return false;
  }
  
  char c;
  std::stringstream encoding;
  while(input.get(c)) {
    encoding << codeTable[c];
  }

  std::string encodingStr = encoding.str();
  int encodingSize = encodingStr.size();
  int trailingZeroes = BYTE_SIZE - encodingSize % BYTE_SIZE;
  std::bitset<BYTE_SIZE> zeroesCnt(trailingZeroes);
  code << zeroesCnt.to_string();
  code << encodingStr;

  
  while(code >> c) {
    // We compose the string of the next BYTE_SIZE bits to be written.
    // Adding trailing zeroes at the end in case the encoded text's bits
    // are not multiple of BYTE_SIZE ( it is needed because of the
    // behavior of std::bitset:
    // std::bitset<8>("101").to_string() == "00000101". It would disrupt
    // the decoding process.)
    std::string curr;
    curr.push_back(c);
    for(int i = 1; i < BYTE_SIZE; ++i) {
      if(code >> c) {
        curr.push_back(c);
      } else {
        for(int j = 0; j < trailingZeroes; ++j) {
          curr.push_back('0');
        }
        i = BYTE_SIZE;
      }
    }
    
    std::bitset<BYTE_SIZE> byte(curr);
    int currByteInt = byte.to_ulong();
    char currByte = currByteInt;
    output.write(&currByte, sizeof(char));
  }
  
  return true;
}

bool
HuffmanCodes::encode() {
  
  if(encodeMode) {
    freqTable = HuffmanTable(inputFile);
    ht = HuffmanTree(freqTable);  
    /*
     * Tree encoding
     * I. 1 byte for the count of the bits (=n).
     * II. n / sizeof(char) + (n % sizeof(char) == 0) bytes for the tree encoding itself.
     */
    std::stringstream code;

    uint treeBits = 0;
    std::string encodeTree = ht.encodeTree(treeBits);
    code << encodeTree;
    uint trailingZeroes = BYTE_SIZE - treeBits % BYTE_SIZE;
  
    while(trailingZeroes-->0) {
      code << '0';
    }

    // We should encode with the same tree we decode
    // For compression purposes encodeTree() may not be read as the same
    // tree we build with the input file, therefore the code table
    // may not be the same. As tree size is no more than 511 nodes, that
    // doesn't affect performance.
    std::stringstream treeStreamBits(encodeTree.substr(2 * BYTE_SIZE, encodeTree.size() - 1));
    ht = HuffmanTree(treeStreamBits);
    codeTable = *ht.getCodeTable();

    if(encodeText(code)) {
      return true;
    }
    return false;
  }
  std::cout << "Wrong mode!\n";
  return false;
}

bool
HuffmanCodes::decode() {
  if (!encodeMode) {

    std::ifstream input(inputFile, std::ios::in | std::ios::binary);
    std::ofstream output(outputFile, std::ios::out);

    if(!input.is_open() || !output.is_open()) {
      std::cout << "Unable to open files!\n";
      return false;
    }
    
    char c;

    // Read the tree's bits count
    std::string treebits;
    for(int i = 0; i < 2; ++i) {
      input.read(&c, sizeof(c));
      std::bitset<BYTE_SIZE> bits(c);
      treebits.append(bits.to_string());
    }
    
    std::bitset<2 * BYTE_SIZE> treeBits(treebits);
    ulong n = treeBits.to_ulong();
    uint treeBytes = n / BYTE_SIZE + (n % BYTE_SIZE != 0);
    
    std::stringstream treeEncodingBits;
    for(int i = 0; i < treeBytes; ++i) {
      input.read(&c, sizeof(c));
      std::bitset<BYTE_SIZE> bits(c);
      std::string curr = bits.to_string();
      treeEncodingBits << curr;
    }

    HuffmanTree tree(treeEncodingBits);
    iterateOverTree(tree, input, output);
    
    return true;
  }
  std::cout << "Wrong mode!\n";
  return false;
}

bool
HuffmanCodes::iterateOverTree(const HuffmanTree& tree, std::ifstream& input, std::ofstream& output) {
  char c;
  char testing;
  // Read the count of trailing zeroes
  input.read(&c, sizeof(c));
  int trailingZeroesCnt = c;
  int endBit = 0;
  
  auto it = tree.begin();
  while(true) {
    input.read(&c, sizeof(c));
    std::bitset<BYTE_SIZE> currBit(c);

    // If we've read the last byte
    // we should not read the trailing zeroes
    testing = input.peek();
    if(testing == EOF) {  
      endBit = trailingZeroesCnt;
    }
    
    for(int i = BYTE_SIZE - 1; i >= endBit; --i) {
      if(currBit.test(i)) {
        ++it;
      } else {
        --it;
      }

      if(it->isLeaf()) {
        output << it->symbol;
        it = tree.begin();
      }
    }
    
    if(testing == EOF) {
      break;
    }
  }
}

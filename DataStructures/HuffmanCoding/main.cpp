#include <iostream>

#include "lib/CommandLineParser/command_line_parser.h"
#include "src/Huffman/huffman_codes.h"

int main(int argc, char** argv) {
 
  CLI::CommandLineParser clp(argv, argc);
  clp.execute();

  return 0;
}

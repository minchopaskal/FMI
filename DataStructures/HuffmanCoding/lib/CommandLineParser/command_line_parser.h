#ifndef COMMAND_LINE_PARSER_H
#define COMMAND_LINE_PARSER_H

#include <vector>

#include "cli_constants.h"
#include "command.h"
#include "../../src/Huffman/huffman_codes.h"

namespace CLI {
  class CommandLineParser {
  private:
    char** argv;
    int argc;

    std::string input;
    std::string output;
    bool mode;

    bool valid;

    bool parse();
  public:
    CommandLineParser(char** argv, int argc);

    Command readCommand(const char*);

    void execute();
  };
};
#endif

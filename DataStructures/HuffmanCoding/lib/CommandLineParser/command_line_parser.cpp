#include "command_line_parser.h"
using namespace CLI;

using cmd = Command::cmd;

CommandLineParser::CommandLineParser(char** argv, int argc)
  : argv(argv), argc(argc), valid(true) {
  if(argc < 6 || !parse()) {
    std::cout << CLI::USAGE_MESSAGE << std::endl;
    valid = false;
  }
}

bool
CommandLineParser::parse() {
  std::vector<Command> commands;

  Command curr;
  for(int i = 0; i < argc; ++i) {
    curr = readCommand(argv[i]);
    commands.push_back(curr);
  }

  if(commands[1].getCmd() != cmd::COMPRESS &&
     commands[1].getCmd() != cmd::DECOMPRESS) {
    return false;
  }
  
  if(commands[2].getCmd() != cmd::INPUT) {
    return false;
  }

  if(commands[4].getCmd() != cmd::OUTPUT) {
    return false;
  }

  mode = commands[1].getCmd() == cmd::COMPRESS;
  input = commands[3].getText();
  output = commands[5].getText();
}

Command
CommandLineParser::readCommand(const char* cmd) {
  Command curr(cmd);
  
  if(cmd == COMPRESS_LONG || cmd == COMPRESS_SHORT) {
    curr.setCmd(cmd::COMPRESS);
  }
  else if(cmd == DECOMPRESS_LONG || cmd == DECOMPRESS_SHORT) {
    curr.setCmd(cmd::DECOMPRESS);
  }
  else if(cmd == INPUT_LONG || cmd == INPUT_SHORT) {
    curr.setCmd(cmd::INPUT);
  } else if (cmd == OUTPUT_LONG || cmd == OUTPUT_SHORT) {
    curr.setCmd(cmd::OUTPUT);
  } else {
    curr.setCmd(cmd::ARGUMENT);
  }

  return curr;
}

void
CommandLineParser::execute() {
  if(!valid) {
    return;
  }
  
  Huffman::HuffmanCodes hc(input, output, mode);

  if(mode) {
    hc.encode();
  } else {
    hc.decode();
  }
}

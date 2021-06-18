#include "command.h"
using namespace CLI;

Command::Command(const char* cmdText)
  : cmdText(cmdText), cmdEnum(cmd::ARGUMENT) {}

void
Command::setCmd(cmd cmdEnum) {
  this->cmdEnum = cmdEnum;
}

Command::cmd
Command::getCmd() const {
  return cmdEnum;
}

std::string
Command::getText() const {
  return cmdText;
}




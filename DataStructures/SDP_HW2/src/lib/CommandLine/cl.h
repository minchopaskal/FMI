#ifndef __COMMAND_LINE_H
#define __COMMAND_LINE_H

#include <iostream>
#include "../../browser/browser.h"
#include "../Utils/Utils.h"

class CommandLine {
private:
  enum class Command {
    GO = 10,
    INSERT,
    SORT,
    BACK = 1,
    FORWARD,
    REMOVE,
    PRINT,
    EXIT,
    INVALID,
    HELP
  };
  
  Browser br;
  bool over;
  
public:
  CommandLine();
  
  void performCommand(std::string);
  Command readCommand(std::string);
  bool exit() const;
  void listCommands() const;

  const Browser& getBrowser() const;
};

#endif

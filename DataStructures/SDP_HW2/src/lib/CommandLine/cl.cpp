#include "cl.h"

CommandLine::CommandLine() : br(Browser()), over(false) {}

CommandLine::Command CommandLine::readCommand(std::string command) {
  std::istringstream stream(command);
  std::string cmd;
  stream >> cmd;
  if(cmd == UTILS::cGO) {
    return Command::GO;
  } else if (cmd == UTILS::cINSERT) {
    return Command::INSERT;
  } else if (cmd == UTILS::cBACK) {
    return Command::BACK;
  } else if (cmd == UTILS::cFORWARD) {
    return Command::FORWARD;
  } else if (cmd == UTILS::cREMOVE) {
    return Command::REMOVE;
  } else if (cmd == UTILS::cPRINT) {
    return Command::PRINT;
  } else if (cmd == UTILS::cSORT) {
    return Command::SORT;
  } else if (cmd == UTILS::cEXIT) {
    return Command::EXIT;
  } else if (cmd == UTILS::cHELP ||
	     cmd == UTILS::cLIST ||
	     cmd == UTILS::cQUESTION_MARK) {
    return Command::HELP;
  } else {
    return Command::INVALID;
  }
}

void CommandLine::performCommand(std::string command) {
  std::istringstream stream(command);
  std::string commandArg;
  stream >> commandArg;
  Command cmd = readCommand(commandArg);
  int weight = static_cast<int>(cmd);
  if(weight >= 10) {
    if(stream.rdbuf()->in_avail() != 0) {
      stream >> commandArg;
    }
    else {
      if(cmd == Command::SORT) commandArg = "";
      else commandArg = br.getHomepage();
    }
  }

  switch(cmd) {
  case Command::HELP:
    listCommands();
    break;
  case Command::BACK:
    br.back();
    break;
  case Command::FORWARD:
    br.forward();
    break;
  case Command::REMOVE:
    br.remove();
    break;
  case Command::PRINT:
    br.print();
    break;
  case Command::EXIT:
    over = true;
    break;
  case Command::GO:
    br.go(commandArg);
    break;
  case Command::INSERT:
    br.insert(commandArg);
    break;
  case Command::SORT:
    {
      int byURL = commandArg.compare("URL") == 0 ? true
	: commandArg.compare("TIME") == 0 ? false : -1;
      if (byURL != -1) {
	br.sort(byURL);
	break;
      }
    }
    std::cout << "Invalid argument!\n";
    break;
  case Command::INVALID:
    std::cout << "Invalid command!\n";
    break;
  }
}

bool CommandLine::exit() const {
  return this->over;
}

void CommandLine::listCommands() const {
  std::cout << "Available commands:\n\n"
	    << "GO <url> - goes to <url> in current tab. If <url> is blank, goes to Homepage.\n\n"
	    << "INSERT <url> - opens <url> in new tab. If <url> is blank, goes to Homepage.\n\n"
	    << "BACK - goes to previos tab (if there is one).\n\n"
	    << "FORWARD - goes to next tab (if there is one).\n\n"
	    << "REMOVE - closes current tab and goes to the next one.\n"
	    << "\tIf no such tab is open, goes to the previous one, and\n"
	    << "\tif it was the last tab, opens the Homepage.\n\n"
	    << "PRINT - prints all open tabs.\n\n"
	    << "SORT <by> - sorts the open tabs either lexicographically (if <by> = URL )\n"
	    << "\tor by timestamp (if <by> = TIME ). If <by> is not provided nothing happens.\n\n"
	    << "HELP - prints the current list of commands.\n";
	    
}

const Browser& CommandLine::getBrowser() const {
  return this->br;
}

#include <iostream>
#include "src/lib/CommandLine/cl.h"

int main() {
  CommandLine cl;
  bool exit = false;
  std::string name;
  std::cout << "Welcome to the best browser out there. Google and Mozilla\n"
	    << "are ashamed of theirselves.\n";
  std::cout << "Please enter your name: ";
  std::cin >> name;
  std::cin.get();

  std::cout << "Thank you! For list of commands enter HELP, LIST or ?\n";
  
  while(!exit) {
    std::string command;
    std::cout << name << " - Tab: " << cl.getBrowser().getCurrent().getURL() << "> ";
    std::getline(std::cin, command);
    cl.performCommand(command);
    exit = cl.exit();
  }
  return 0;
}

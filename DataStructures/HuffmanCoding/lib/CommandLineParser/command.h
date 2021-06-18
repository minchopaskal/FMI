#ifndef COMMAND_H
#define COMMAND_H

#include <string>

namespace CLI {
  class Command {
  public:   
    enum cmd {
      COMPRESS,
      DECOMPRESS,
      INPUT,
      OUTPUT,
      ARGUMENT
    };
  public:
    Command(const char* cmdText = "invalid");

    void setCmd(cmd);
    cmd getCmd() const;
    std::string getText() const;

  private:
    std::string cmdText;
    cmd cmdEnum;
  };
};

#endif

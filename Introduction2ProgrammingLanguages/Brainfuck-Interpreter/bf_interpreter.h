#ifndef BRAINFUCK_INTERPRETER_H
#define BRAINFUCK_INTERPRETER_H

#include <cstddef>
#include <iostream>
#include <stack>

class BFInterpreter {
public:
  using uchar = unsigned char;
  
  BFInterpreter(const char *program, std::istream &input = std::cin, std::ostream &output = std::cout);
  ~BFInterpreter();

  void compile();
  
private:
  void jumpForwards(int steps);
  void jumpBackwards(int steps);
  
private:
  std::istream &m_Input;
  std::ostream &m_Output;
  std::stack<char*> m_LoopStack;
  size_t m_MemorySize;
  
  uchar *m_MemoryBlocks;
  uchar *m_Head;
  char *m_Program;
  char *m_CurrentInstr;
};

#endif /* BRAINFUCK_INTERPRETER_H */

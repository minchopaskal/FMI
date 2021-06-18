#include "bf_interpreter.h"

#include <cassert>
#include <cmath>
#include <cstring>

#ifndef MEMSIZE
#define MEMSIZE 30000
#endif

BFInterpreter::BFInterpreter(const char *program, std::istream &input, std::ostream &output) :
  m_MemoryBlocks(nullptr), m_Head(nullptr),
  m_Program(nullptr), m_Input(input), m_Output(output),
  m_MemorySize(MEMSIZE) {
  
  m_Program = new char[strlen(program) + 1];
  strncpy(m_Program, program, strlen(program) + 1);
  m_CurrentInstr = m_Program;

  m_MemoryBlocks = new uchar[m_MemorySize];
  for (int i = 0; i < m_MemorySize; ++i) {
    m_MemoryBlocks[i] = 0;
  }
  m_Head = m_MemoryBlocks;
}

BFInterpreter::~BFInterpreter()
{
  delete [] m_MemoryBlocks;
  delete [] m_Program;
}

void BFInterpreter::compile()
{
  while (*m_CurrentInstr)
  {
    switch (*m_CurrentInstr)
    {
    case '+':
      ++*m_Head;
      break;
    case '-':
      --*m_Head;
      break;
    case '.':
      m_Output.put(*m_Head);
      break;
    case ',':
      *m_Head = m_Input.get();
      break;
    case '<':
      if (m_Head == m_MemoryBlocks)
        m_Head = m_MemoryBlocks + (m_MemorySize - 1);
      else 
        --m_Head;
      break;
    case '>':
      if (m_Head == m_MemoryBlocks + (m_MemorySize - 1))
        m_Head = m_MemoryBlocks;
      else
        ++m_Head;
      break;
    case '[':
      m_LoopStack.push(m_CurrentInstr + 1);
      break;
    case ']':
      assert(!m_LoopStack.empty() && "Closing unopened loop");
      
      if (!*m_Head)
      {
        m_LoopStack.pop();
      }
      else
      {
        m_CurrentInstr = m_LoopStack.top();
        --m_CurrentInstr;
      }
      break;
    /* Custom instructions */
    case '!':
      *m_Head = 0;
      break;
    case '^':
      jumpForwards(*m_Head);   
      break;
    case '&':
      jumpBackwards(*m_Head);
      break;
    case '#':
      if (!*m_Head)
      {
        break;
      }

      jumpForwards(m_Head == m_MemoryBlocks ?
                   *(m_MemoryBlocks + m_MemorySize - 1) :
                   *(m_Head - 1));
      break;
    case '$':
      if (!*m_Head)
      {
        break;
      }

      jumpBackwards(m_Head == (m_MemoryBlocks + m_MemorySize - 1) ?
                    *(m_MemoryBlocks) :
                    *(m_Head + 1));
      break;
    default: // ignore 
      break;
    }
    ++m_CurrentInstr;
  }
  m_Output.put('\n');
#ifdef DEBUG
#ifdef CELLS_TO_PRINT
  if (CELLS_TO_PRINT > m_MemorySize) return;
  
  for (int i = 0; i < CELLS_TO_PRINT; ++i)
  {
    m_Output.put('|');
    m_Output.put(m_MemoryBlocks[i] + '0');
    m_Output.put('|');
  }
  m_Output.put('\n');
#endif
#endif
}

void BFInterpreter::jumpForwards(int steps)
{
  if (m_MemorySize - (m_Head - m_MemoryBlocks) < steps) // overflow
  {
    m_Head = m_MemoryBlocks + (steps - (m_MemorySize - (m_Head - m_MemoryBlocks)));
  }
  else
  {
    m_Head += (int) steps;
  }
}

void BFInterpreter::jumpBackwards(int steps)
{
  if (m_Head - m_MemoryBlocks < steps) // underflow
  {
    m_Head = m_MemoryBlocks + (m_MemorySize - (steps - (m_Head - m_MemoryBlocks)));
  }
  else
  {
    m_Head -= (int) steps;
  }
}

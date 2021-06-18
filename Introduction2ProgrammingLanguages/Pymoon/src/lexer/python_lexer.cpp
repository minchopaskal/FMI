#include "python_lexer.hpp"
#ifdef DEBUG
#include <iostream>
#endif
// namespace pymoon
// {

HashTable<String, TokenType> GetKeyWords();

class Tokenizer
{
private:
  enum class Sign
  {
    PlusSign,
    MinusSign,
    NoSign
  };
  
public:
  Tokenizer(const char *source, TokenizerOptions options);
  TokenizerResult tokenize(Vector<Token> &result);

private:
  bool handleIndent(Vector<Token> &result);
  
  Token next();
  void nextLine();
  char nextSymbol();
  inline char currentSymbol() const { return m_Source[m_Pos]; }
  inline char peek() const { return m_Source[m_Pos+1]; }
  inline char prevSymbol() const { return m_Source[m_Pos - 1]; }
  
  bool match(char c);
  bool matchTimes(unsigned times, char c);

  Token makeToken(TokenType type) const;
  Token makeToken(TokenType type, String &value) const;
  Token makeToken(TokenType type, const char *value) const;
  Token makeToken(TokenType type, String &lexeme, double value) const;

  Token parseNumber(Sign sign = Sign::NoSign);
  Token parseString(char quote);
  Token parseComment();
  Token parseIdentifier();
  
private:
  HashTable<String, TokenType> m_Keywords;

  TokenizerOptions m_Options;
  const char *m_Source;
  size_t m_Pos;
  size_t m_Line;
  size_t m_Column;
  size_t m_CurrentIndent;
  size_t m_PrevColumn;
  size_t m_PrevIndent;
};

Tokenizer::Tokenizer(const char *source, TokenizerOptions options) :
  m_Options(options),
  m_Source(source),
  m_Pos(0),
  m_Line(1),
  m_Column(0),
  m_CurrentIndent(0),
  m_PrevColumn(0),
  m_PrevIndent(0) { }

TokenizerResult
Tokenizer::tokenize(Vector<Token> &result)
{
  Token t;
  while (t = next())
  {
    result.push_back(t);

    if (t.type == TokenType::NewLine)
    {
      if (!handleIndent(result))
      {
          break;
      }
    }
  }
  
  if (t.type == TokenType::Eof)
  {
    result.push_back(t);
    return TokenizerResult {true, "", {}};
  }
  
  return TokenizerResult {false, t.lexeme, t}; 
}

bool Tokenizer::handleIndent(Vector<Token> &result)
{
  if (currentSymbol() != ' ' && m_PrevIndent > 0)
  {
    m_PrevIndent = m_CurrentIndent = 0;
    result.push_back(makeToken(TokenType::Dedent));
    return true;
  }
  
  auto indent = m_Options.Indentation - 1;
  
  while (match(' '))
  {
    while (indent > 0 && match(' '))
    {
      --indent;
    }
    
    if (indent != 0)
    {
      result.emplace_back(makeToken(TokenType::Invalid, "Invalid size of indentation"));
      return false;
    }
    ++m_CurrentIndent;
    indent = m_Options.Indentation - 1;
  }

  if (m_CurrentIndent == m_PrevIndent)
  {
    return true;
  }

  if (m_CurrentIndent ==  m_PrevIndent - 1)
  {
    result.emplace_back(makeToken(TokenType::Dedent));
    return true;
  }

  if (m_CurrentIndent == m_PrevIndent + 1)
  {
    result.emplace_back(makeToken(TokenType::Indent));
    return true;
  }

  std::cout << "Indent: " << m_CurrentIndent << " Prev: " << m_PrevIndent << std::endl;
  result.emplace_back(makeToken(TokenType::Invalid, "Too much/little indentation"));
  return false;
}

Token Tokenizer::next()
{
  Token result;
  switch (currentSymbol())
  {
  case EOF: return makeToken(TokenType::Eof);
  case '\n':
    nextLine();
    return makeToken(TokenType::NewLine);
  case '\t': /* When it comes to indentation these are handled in tokenize(). */
  case ' ':
    nextSymbol();
    return next();
  case '(': nextSymbol(); return makeToken(TokenType::LeftParen);
  case ')': nextSymbol(); return makeToken(TokenType::RightParen);
  case '{': nextSymbol(); return makeToken(TokenType::LeftBrace);
  case '}': nextSymbol(); return makeToken(TokenType::RightBrace);
  case '[': nextSymbol(); return makeToken(TokenType::LeftBracket);
  case ']': nextSymbol(); return makeToken(TokenType::RightBracket);
  case ',': nextSymbol(); return makeToken(TokenType::Comma);
  case ':': nextSymbol(); return makeToken(TokenType::Colon);
  case ';': nextSymbol(); return makeToken(TokenType::SemiColon);
  case '.': nextSymbol();
    return
      match('.') ?
      (match('.') ?
       makeToken(TokenType::Ellipsis) :
       makeToken(TokenType::Invalid, "Excpected \'...\', got \'..\'")) :
      makeToken(TokenType::Dot);
  case '@': nextSymbol(); return makeToken(TokenType::At);
  case '&': nextSymbol();
    return
      match('=') ?
      makeToken(TokenType::AndAssign) :
      makeToken(TokenType::BitwiseAnd);
  case '|': nextSymbol();
    return
      match('=') ?
      makeToken(TokenType::OrAssign) :
      makeToken(TokenType::BitwiseOr);
  case '^': nextSymbol();
    return
      match('=') ?
      makeToken(TokenType::XorAssign) :
      makeToken(TokenType::BitwiseXor);
  case '~': nextSymbol(); return makeToken(TokenType::BitwiseNot);
  case '=': nextSymbol();
    return
      match('=') ?
      makeToken(TokenType::Equal) :
      makeToken(TokenType::Assign);
  case '!': nextSymbol();
    return
      match('=') ?
      makeToken(TokenType::NotEqual) :
      makeToken(TokenType::Invalid, "Syntax error \'!\'");
  case '-': nextSymbol();
    return
      match('>') ?
      makeToken(TokenType::Arrow) :
      (match('=') ? /* yeah, I like lisp */
       makeToken(TokenType::MinusAssign) :
       (isdigit(currentSymbol()) || currentSymbol() == '.' ?
        parseNumber(Sign::MinusSign) :
        makeToken(TokenType::Minus)));
  case '+': nextSymbol();
    return
      match('=') ?
      makeToken(TokenType::PlusAssign) :
      (isdigit(currentSymbol()) || currentSymbol() == '.' ?
       parseNumber(Sign::PlusSign) :
       makeToken(TokenType::Plus));
  case '*': nextSymbol();
    return
      match('*') ?
      (match('=') ?
       makeToken(TokenType::ExpAssign) :
       makeToken(TokenType::Exp)) :
      (match('=') ?
       makeToken(TokenType::MulAssign) :
       makeToken(TokenType::Mul));
  case '/': nextSymbol();
    return 
      match('/') ?
      (match('=') ?
       makeToken(TokenType::DivDivAssign) :
       makeToken(TokenType::DivDiv)) :
      (match('=') ?
       makeToken(TokenType::DivAssign) :
       makeToken(TokenType::Div));
  case '%': nextSymbol();
    return
      match('=') ?
      makeToken(TokenType::ModAssign) :
      makeToken(TokenType::Mod);
  case '<': nextSymbol();
    return
      match('<') ?
      (match('=') ?
       makeToken(TokenType::ShiftLeftAssign) :
       makeToken(TokenType::ShiftLeft)) :
      (match('=') ?
       makeToken(TokenType::LessEqual) :
       makeToken(TokenType::Less));
  case '>': nextSymbol();
    return
      match('>') ?
      (match('=') ?
       makeToken(TokenType::ShiftRightAssign) :
       makeToken(TokenType::ShiftRight)) :
      (match('=') ?
       makeToken(TokenType::MoreEqual) :
       makeToken(TokenType::More));
  case '\\':
    nextSymbol();
    if (match('\n'))
    {
      nextSymbol();
      return next();
    }
    else
    {
      return makeToken(TokenType::Invalid, "No new line after backslash");
    }
  case '#':
    if (m_Options.TokenizeComments)
    {
      return parseComment();
    }
    while (currentSymbol() != '\n') nextSymbol();
    return next();
  case '\'':
  case '\"':
    return parseString(currentSymbol());
    break;
  }

  if (isalpha(currentSymbol()) || currentSymbol() == '_')
  {
    return parseIdentifier();
  }

  if (isdigit(currentSymbol()))
  {
    return parseNumber();
  }

  return makeToken(TokenType::Invalid, "Syntax error");
}

void Tokenizer::nextLine()
{
  ++m_Pos;
  ++m_Line;
  m_PrevColumn = m_Column;
  m_Column = 0;
  m_PrevIndent = m_CurrentIndent;
  m_CurrentIndent = currentSymbol() == '\n' ? m_CurrentIndent : 0;
}

char Tokenizer::nextSymbol()
{
  ++m_Pos;
  m_PrevColumn = m_Column++;
  return currentSymbol();
}

Token Tokenizer::makeToken(TokenType type) const 
{
  return Token {type, m_Line - (type == TokenType::NewLine), m_PrevColumn};
}

Token Tokenizer::makeToken(TokenType type, String &value) const
{
  return Token {type, m_Line, m_PrevColumn, value};
}

Token Tokenizer::makeToken(TokenType type, const char *value) const
{
  return Token {type, m_Line, m_PrevColumn, value};
}

Token Tokenizer::makeToken(TokenType type, String &lexeme, double value) const
{
  return Token {type, m_Line, m_PrevColumn, lexeme, value};
}

bool Tokenizer::match(char c)
{
  if (m_Source[m_Pos] != c)
  {
    return false;
  }

  nextSymbol();
  return true;
}

bool Tokenizer::matchTimes(unsigned times, char c)
{
  size_t pos = m_Pos;
  char curr = m_Source[pos++];
  while (curr != EOF && times > 0)
  {
    if (curr != c)
    {
      return false;
    }

    curr = m_Source[pos++];
    --times;
  }
  
  return true;
}

Token Tokenizer::parseComment()
{
  String result;
  
  match('#');
  while (currentSymbol() != '\n')
  {
    result.push_back(currentSymbol());
    nextSymbol();
  }

  return makeToken(TokenType::Comment, result);
}

Token Tokenizer::parseIdentifier()
{
  String result;
  if (currentSymbol() == 'r' && (peek() == '\'' || peek() == '\"'))
  {
    Token r = parseString(nextSymbol());
    r.number = -1; // Raw string
    return r;
  }
  
  while (isalpha(currentSymbol()) || isdigit(currentSymbol()) || currentSymbol() == '_') {
    result.push_back(m_Source[m_Pos]);
    nextSymbol();
  }

  if (m_Keywords.empty()) 
  {
    m_Keywords = GetKeyWords();
  }
  
  if (m_Keywords.find(result) != m_Keywords.end()) {
    return makeToken(m_Keywords[result], result);
  }

  return makeToken(TokenType::Identifier, result);
}

Token Tokenizer::parseNumber(Sign sign)
{
  String result;
  if (sign == Sign::MinusSign)
  {
      result += "-";
  }
  if (sign == Sign::PlusSign)
  {
      result += "+";
  }

  double number;
  while (isdigit(currentSymbol()))
  {
    number = number * 10 + (currentSymbol() - '0');
    result.push_back(currentSymbol());
    nextSymbol();
  }

  return makeToken(TokenType::Number, result, number);
  // Integers for now
  /* if (currentSymbol() == '.') ... */
}

Token Tokenizer::parseString(char quote)
{
  String result;
  if (match(quote) && match(quote) && match(quote)) 
  {
    while (!match(EOF) && !matchTimes(3, quote))
    {
      if (currentSymbol() == '\\' && peek() == '\n')
      {
        nextSymbol();
        nextSymbol();
        ++m_Line;
        m_Column = 0;
        continue;
      }
      if (currentSymbol() == '\n')
      {
        nextSymbol();
        ++m_Line;
        m_Column = 0;
        continue;
      }
      result.push_back(currentSymbol());
      nextSymbol();
    }

    if (!(match(quote) && match(quote) && match(quote)))
    {
      return makeToken(TokenType::Invalid, "String not closed properly");
    }
    return makeToken(TokenType::String, result);
  }

  while ((currentSymbol() != quote || prevSymbol() == '\\') &&
         (currentSymbol() != '\n' || prevSymbol() == '\\'))
  {
    result.push_back(currentSymbol());
    nextSymbol();
  }

  if (currentSymbol() == '\n')
  {
    return makeToken(TokenType::Invalid, "New line in string");
  }
  
  if (!match(quote))
  {
    return makeToken(TokenType::Invalid, "String not closed properly");
  }
  
  return makeToken(TokenType::String, result);
}

TokenizerResult Tokenize(String source, Vector<Token> &result, TokenizerOptions options)
{
  return Tokenize(source.c_str(), result, options);
}

TokenizerResult Tokenize(const char *source, Vector<Token> &result, TokenizerOptions options)
{
  Tokenizer t{source, options};
  return t.tokenize(result);
}

HashTable<String, TokenType> GetKeyWords()
{
  HashTable<String, TokenType> keywords;

  keywords["and"] = TokenType::And;
  keywords["as"] = TokenType::As;
  keywords["assert"] = TokenType::Assert;
  keywords["break"] = TokenType::Break;
  keywords["class"] = TokenType::Class;
  keywords["continue"] = TokenType::Continue;
  keywords["def"] = TokenType::Def;
  keywords["del"] = TokenType::Del;
  keywords["elif"] = TokenType::Elif;
  keywords["else"] = TokenType::Else;
  keywords["except"] = TokenType::Except;
  keywords["False"] = TokenType::False;
  keywords["finally"] = TokenType::Finally;
  keywords["for"] = TokenType::For;
  keywords["from"] = TokenType::From;
  keywords["global"] = TokenType::Global;
  keywords["if"] = TokenType::If;
  keywords["import"] = TokenType::Import;
  keywords["in"] = TokenType::In;
  keywords["is"] = TokenType::Is;
  keywords["lambda"] = TokenType::Lambda;
  keywords["None"] = TokenType::None;
  keywords["nonlocal"] = TokenType::NonLocal;
  keywords["not"] = TokenType::Not;
  keywords["or"] = TokenType::Or;
  keywords["pass"] = TokenType::Pass;
  keywords["raise"] = TokenType::Raise;
  keywords["return"] = TokenType::Return;
  keywords["self"] = TokenType::Self;
  keywords["True"] = TokenType::True;
  keywords["try"] = TokenType::Try;
  keywords["while"] = TokenType::While;
  keywords["with"] = TokenType::With;
  keywords["yield"] = TokenType::Yield;
  keywords["await"] = TokenType::Await;
  return keywords;
}

// } /* Namespace pymoon */

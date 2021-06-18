#pragma once
#include "../types.hpp"

enum class TokenType : int
{
  // Delimiters
  LeftParen = 0, 
  RightParen,
  LeftBrace,
  RightBrace,
  LeftBracket,
  RightBracket,
  Comma,
  Colon,
  SemiColon,
  Dot,
  At,
  Arrow,

  // Identifiers/Literals
  Identifier = 50,
  Ellipsis,
  Comment,
  String,

  // Numbers
  Integer = 100,
  Floating,
  Binary,
  Hex,
  Oct,
  Complex = 105,
  Number,

  // Operators
  Plus = 200,
  Minus,
  Mul,
  Div,
  DivDiv,
  Exp,
  Mod,
  BitwiseAnd,
  BitwiseOr,
  BitwiseNot,
  BitwiseXor,
  ShiftLeft,
  ShiftRight,
  Assign,
  PlusAssign = 230,
  MinusAssign,
  MulAssign,
  DivAssign,
  DivDivAssign,
  ExpAssign,
  ModAssign,
  AndAssign,
  OrAssign,
  XorAssign,
  ShiftLeftAssign,
  ShiftRightAssign,
  Equal = 250,
  NotEqual,
  MoreEqual,
  LessEqual,
  More,
  Less,
  
  // Keywords
  And = 300,
  Or,
  Not,
  As,
  Assert,
  Break,
  Class,
  Continue,
  Def,
  Del,
  Elif,
  Else,
  Except,
  False,
  Finally,
  For,
  From,
  Global,
  If,
  Import,
  In = 350,
  Is,
  Lambda,
  None,
  NonLocal,
  Pass,
  Raise,
  Return,
  Self,
  True,
  Try,
  While,
  With,
  Yield,
  Await,
  Async,

  // Misc
  Eof = 400,
  NewLine,
  Indent,
  Dedent,
  Invalid = 404 
};

enum class SingletonTokenType : int
{
  None = static_cast<int>(TokenType::None),
  True = static_cast<int>(TokenType::True),
  False = static_cast<int>(TokenType::False)
};

struct Token
{
  TokenType type;
  size_t line;
  size_t column;
  String lexeme;
  double number;
  
  operator bool() const {
    return type != TokenType::Eof && type != TokenType::Invalid;
  }
};

struct TokenizerOptions
{
  short Indentation = 4;
  bool TokenizeComments = false;
};

struct TokenizerResult
{
  bool Success;
  String What;
  Token ErrorToken;
};

#pragma once

#include "ast.hpp"
#include "../lexer/token_types.hpp"
#include "../types.hpp"

struct ParserOptions
{
  _TODO_
};

struct ParserResult
{
  Token Error;
  String What;
  bool Success;
};

AstNodePtr Parse(const Vector<Token> &tokens, ParserResult &result, ParserOptions Options = ParserOptions());

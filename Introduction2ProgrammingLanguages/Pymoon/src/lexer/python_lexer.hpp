#pragma once

#include "token_types.hpp"

//namespace pymoon {

//HashMap<TokenType, String> GetTokenNames(); Maybe a Macro ?

TokenizerResult Tokenize(String source, Vector<Token> &result, TokenizerOptions options = TokenizerOptions());
TokenizerResult Tokenize(const char *source, Vector<Token>& result, TokenizerOptions options = TokenizerOptions());
// template <typename IStream>
// TokenizerResult Tokenize(IStream sourceStream, Vector<Token>& result, TokenizerOptions options);

//} /* Namespace pymoon */

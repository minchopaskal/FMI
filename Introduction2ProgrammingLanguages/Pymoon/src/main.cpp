#include "types.hpp"
#include "lexer/python_lexer.hpp"
#include "parser/python_parser.hpp"
#include "parser/print_visitor.hpp"
#include "parser/translator.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

using AstPtr = SharedPtr<AstNode>;

int main(int argc, char **argv)
{
  if (argc != 3)
  {
    std::cout << "Usage: ./pymoon <source-file> <output-file>\n";
    return 2;
  }
  std::ifstream ifs(argv[1]);

  String file { std::istreambuf_iterator<char>(ifs),
                std::istreambuf_iterator<char>() };
  file.push_back(EOF);
  Vector<Token> tokens;
  auto res = Tokenize(file, tokens);

  if (!res.Success)
  {
    std::cout << "Tokenizer failed!\n";
    std::cout << res.What << " at "
              << res.ErrorToken.line << ":" << res.ErrorToken.column
              << std::endl;
    return 1;
  }

  // for (auto &t : tokens)
  // {
  //   std::cout << static_cast<int>(t.type) << std::endl;
  // }
  
  ParserResult p;
  auto root = Parse(tokens, p);

  std::filebuf fb;
  fb.open(argv[2], std::ios::out);
  PrintVisitor t;
  t.Print(root.get());

  OutputStream os(&fb);
  Translator tr (os);
  tr.Translate(root.get());
}

#include <algorithm>
#include "python_parser.hpp"

#ifdef DEBUG
#include <iostream>
#include <stack>
#endif

class Parser
{
public:
  using Tokens = const Vector<Token>&;
  using Pos = VectorCPos<Token>;
  using MemFn = AstNodePtr (Parser::*)();
public:
  Parser(Tokens tokens, ParserOptions options);
  AstNodePtr Parse(ParserResult &result);

private:
  template <typename T, typename U>
  T EnumCast(U toConver);

  template <typename CtxExpr>
  void SetContext(AstNodePtr &expr, ExprContext ctx);

  // Lists are expected to have at least one element
  // so empty vector on return counts as invalid
  Vector<AstNodePtr> GetList(MemFn f, TokenType separator = TokenType::Comma);
  Vector<AstNodePtr> GetList(Vector<MemFn> f);
  
  void ErrorMessage(const char *msg);
  void Success();
  inline Token Peek() const { return *m_Pos; }
  inline TokenType PeekType() const { return Peek().type; }
  inline TokenType PeekNext() const { return (m_Pos + 1)->type; }
  inline void Advance() { ++m_Pos; }
  bool Match(TokenType type, bool advance = true);
  bool MatchOneOf(Vector<TokenType> types, bool advance = true);

  IdentifierPtr GetName(bool advance = true);
  
  AstNodePtr FileInput();
  AstNodePtr Decorator();
  Vector<AstNodePtr> Decorators();
  AstNodePtr Decorated();
  AstNodePtr Parameters();
  AstNodePtr VarArgsList();
  AstNodePtr FpDef();
  Vector<AstNodePtr> FpList();
  AstNodePtr FuncDef(bool async = false, Vector<AstNodePtr> decorators = {});
  Vector<AstNodePtr> Stmt();
  Vector<AstNodePtr> SimpleStmt();
  AstNodePtr CompoundStmt();
  AstNodePtr AsyncStmt();
  AstNodePtr SmallStmt();
  AstNodePtr ExprStmt();
  Vector<AstNodePtr> TestListStarExpr();
  AstNodePtr DelStmt();
  AstNodePtr PassStmt();
  AstNodePtr FlowStmt();
  AstNodePtr ImportStmt();
  AstNodePtr GlobalStmt();
  AstNodePtr NonLocalStmt();
  AstNodePtr AssertStmt();
  AstNodePtr YieldExpr();
  Vector<AstNodePtr> YieldArg();
  Vector<AstNodePtr> TestList();
  Vector<AstNodePtr> ExprList();
  AstNodePtr BreakStmt();
  AstNodePtr ContinueStmt();
  AstNodePtr ReturnStmt();
  AstNodePtr RaiseStmt();
  AstNodePtr YieldStmt();
  AstNodePtr ImportName();
  AstNodePtr ImportFrom();
  Vector<AstNodePtr> ImportAsNames();
  AstNodePtr ImportAsName();
  Vector<AstNodePtr> DottedAsNames();
  AstNodePtr DottedAsName();
  AstNodePtr DottedName();
  AstNodePtr Test();
  AstNodePtr Expr();
  AstNodePtr XorExpr();
  AstNodePtr AndExpr();
  AstNodePtr ShiftExpr();
  AstNodePtr ArithExpr();
  AstNodePtr Term();
  AstNodePtr Factor();
  AstNodePtr Power();
  AstNodePtr AtomExpr();
  AstNodePtr Atom();
  AstNodePtr Trailer(AstNodePtr expr);
  AstNodePtr Call(AstNodePtr func);
  AstNodePtr Attribute(AstNodePtr value);
  AstNodePtr SubscriptExpr(AstNodePtr value);
  Vector<AstNodePtr> SubscriptList();
  AstNodePtr Subscript();
  AstNodePtr SliceOp();
  AstNodePtr DictOrSetMarker();
  AstNodePtr StarExpr();
  AstNodePtr OrTest();
  AstNodePtr AndTest();
  AstNodePtr NotTest();
  AstNodePtr Comparison();
  AstNodePtr LambdaDef();
  
  // Compund Statements
  AstNodePtr IfStmt(bool elif = false);
  Vector<AstNodePtr> Suite();
  AstNodePtr WhileStmt();
  AstNodePtr ForStmt(bool async = false);
  AstNodePtr TryStmt();
  AstNodePtr WithStmt(bool async = false);
  AstNodePtr WithItem();
  AstNodePtr ClassDef(Vector<AstNodePtr> decorators = {});

  Vector<AstNodePtr> ArgList();
  AstNodePtr Argument();
  Vector<AstNodePtr> CompFor();
  Vector<AstNodePtr> CompIf();
  AstNodePtr TestNoCond();
  AstNodePtr LambdaDefNoCond();
  
private:
  Tokens m_Tokens;
  Pos m_Pos;
  ParserOptions m_Options;
  ParserResult m_ParserResult;

  #ifdef DEBUG
  std::stack<const char*> stacktrace;
  #endif
  
};

Parser::Parser(Parser::Tokens tokens, ParserOptions options) :
  m_Tokens(tokens),
  m_Options(options),
  m_ParserResult() { }

AstNodePtr Parser::Parse(ParserResult &parserResult)
{
  if (m_Tokens.empty())
  {
    return nullptr;
  }
  
  m_Pos = m_Tokens.begin();
  auto result = FileInput();
  if (result == nullptr)
  {
    return nullptr;
  }
  Success();
  parserResult = m_ParserResult;
  return result;
}

template <typename T, typename U>
T Parser::EnumCast(U toConvert)
{
  return static_cast<T>(toConvert);
}

template <typename T>
void Parser::SetContext(AstNodePtr &expr, ExprContext ctx)
{
  auto p = dynamic_cast<T*>(expr.get());
  p->SetCtx(ctx);
}

Vector<AstNodePtr> Parser::GetList(MemFn f, TokenType separator)
{
  Vector<AstNodePtr> result;

  do
  {
    auto el = (this->*f)();
    if (el == nullptr)
    {
      break;
    }
    result.push_back(el);
  } while (Match(separator));

  return result;
}

Vector<AstNodePtr> Parser::GetList(Vector<MemFn> fs)
{
  Vector<AstNodePtr> result;
  AstNodePtr el;
  do
  {
    for (MemFn f : fs)
    {
      if (el = (this->*f)())
      {
        result.push_back(el);
        goto exit;
      }
    }
  exit:
    if (el == nullptr)
    {
      break;
    }
  } while (Match(TokenType::Comma));


  return result;
}

void Parser::ErrorMessage(const char *msg)
{
#ifdef DEBUG
  //std::cout << msg << std::endl;
  stacktrace.push(msg);
#endif
  m_ParserResult.Error = *m_Pos;
  m_ParserResult.What = msg;
  m_ParserResult.Success = false;
}

void Parser::Success()
{
  m_ParserResult.Success = true;
}

bool Parser::Match(TokenType type, bool advance)
{
  if (m_Pos->type != type)
  { 
    return false;
  }

#ifdef DEBUG
  if (type == TokenType::Eof) std::cout << "EOF\n";
  else std::cout << "Match:" << (m_Pos->type == TokenType::NewLine ? "nl" : m_Pos->lexeme) << ":end\n";
#endif
  
  if (advance)
  {
    ++m_Pos;
  }

  return true;
}

bool Parser::MatchOneOf(Vector<TokenType> types, bool advance)
{
  if (std::find(types.begin(), types.end(), m_Pos->type) == types.end())
  {
    return false;
  }

  if (advance)
  {
    ++m_Pos;
  }
  return true;
}

IdentifierPtr Parser::GetName(bool advance)
{
  if (PeekType() != TokenType::Identifier)
  {
    return nullptr;
  }
  
  auto id = Peek().lexeme;
  if (advance) Advance();
  return MakeShared<Identifier>(id);
}

AstNodePtr Parser::FileInput()
{
  Vector<AstNodePtr> body;
  
  while (!Match(TokenType::Eof))
  {
    if (Match(TokenType::NewLine)) continue;

    auto stmts = Stmt();
    
    if (stmts.empty())
    {
      break;
    }
    
    for (auto &stmt : stmts)
    {
      body.push_back(stmt);
    }
  }

#ifdef DEBUG
  std::cout << "stacktrace\n";
  while (!stacktrace.empty())
  {
    std::cout << stacktrace.top() << std::endl;;
    stacktrace.pop();
  }
#endif
  return MakeShared<PyModule>(body);
}

AstNodePtr Parser::Decorator()
{
  if (!Match(TokenType::At))
  {
    return nullptr;
  }

  auto name = DottedName();
  Vector<AstNodePtr> args;
  
  if (!Match(TokenType::LeftParen))
  {
    if (!Match(TokenType::NewLine))
    {
      ErrorMessage("Missing new line after decorator statement");
      return nullptr;
    }
  
    return MakeShared<::Decorator>(name, args);
  }

  args = ArgList();

  if (!Match(TokenType::RightParen))
  {
    ErrorMessage("Missing closing parenthesis");
    return nullptr;
  }

  return MakeShared<::Decorator>(name, args);
}

Vector<AstNodePtr> Parser::Decorators()
{
  return GetList(&Parser::Decorator);
}

AstNodePtr Parser::Decorated()
{
  auto decorators = Decorators();
  if (decorators.empty())
  {
    ErrorMessage("Statement not decorated");
    return nullptr;
  }

  AstNodePtr stmt;
  if (stmt = ClassDef(decorators)) return stmt;
  if (stmt = FuncDef(false, decorators)) return stmt;
  if (stmt = FuncDef(true, decorators)) return stmt;

  ErrorMessage("Decorators without statements");
  return nullptr;
}

AstNodePtr Parser::Parameters()
{
#ifdef DEBUG
  std::cout << "Parameters()\n";
#endif
  if (!Match(TokenType::LeftParen))
  {
    return nullptr;
  }

  auto args = VarArgsList();

  if (!Match(TokenType::RightParen))
  {
    ErrorMessage("Missing closing paren after parameter list");
    return nullptr;
  }

  return args;
}

AstNodePtr Parser::VarArgsList()
{
  Vector<AstNodePtr> args;
  Vector<AstNodePtr> defaults;
  AstNodePtr vararg;
  AstNodePtr kwarg;
  
  while (true)
  {
    auto param = FpDef();
    if (param == nullptr)
    {
      break;
    }
    args.push_back(param);
    
    if (Match(TokenType::Assign))
    {
      auto rightSide = Test();
      if (rightSide == nullptr)
      {
        ErrorMessage("Syntax error while parsing VarArgsList");
        return nullptr;
      }

      defaults.push_back(rightSide);
    }
    else
    {
      defaults.push_back(nullptr);
    }

    if (!Match(TokenType::Comma))
    {
      break;
    }
  }
  
  if (Match(TokenType::Mul))
  {
    auto name = GetName();
    if (name == nullptr)
    {
      ErrorMessage("Invalid vararg argument");
      return nullptr;
    }

    auto ctx = ExprContext::None;
    vararg = MakeShared<StarredExpression>(name, ctx);
  }

  AstNodePtr kwargId = nullptr;
  if (Match(TokenType::Exp))
  {
    auto name = GetName();
    if (name == nullptr)
    {
      ErrorMessage("Invalid kwarg argument");
      return nullptr;
    }

    auto ctx = ExprContext::None;
    vararg = MakeShared<StarredExpression>(kwargId, ctx);
  }

  return MakeShared<Arguments>(args, vararg, kwarg, defaults);
}

AstNodePtr Parser::FpDef()
{
 
  if (PeekType() != TokenType::Identifier)
  {
    if (!Match(TokenType::LeftParen))
    {
      return nullptr;
    }

    auto list = FpList();
    if (list.empty())
    {
      ErrorMessage("Expected non-empty list");
      return nullptr;
    }

    if (!Match(TokenType::LeftParen))
    {
      ErrorMessage("Missing closing paren");
      return nullptr;
    }

    auto ctx = ExprContext::Param;
    return MakeShared<TupleExpression>(list, ctx);
  }

  auto lexeme = Peek().lexeme;
  auto id = MakeShared<Identifier>(lexeme);
  Advance();

  AstNodePtr annotation;
  if (Match(TokenType::Colon))
  {
    if (PeekType() != TokenType::Identifier)
    {
      ErrorMessage("Expected identifier");
      return nullptr;
    }
    auto annonId = Peek().lexeme;
    Advance();
    annotation = MakeShared<Identifier>(annonId);
  }

  return MakeShared<Arg>(id, annotation);
}

Vector<AstNodePtr> Parser::FpList()
{
  return GetList(&Parser::FpDef);
}



AstNodePtr Parser::FuncDef(bool async, Vector<AstNodePtr> decorators)
{
#ifdef DEBUG
  std::cout << "FuncDef()\n";
#endif
  /*
    (Async)FunctionDef(identifier name, arguments args,
    stmt* body, expr* decorator_list, expr? returns) */

  /*funcdef: 'def' NAME parameters ['->' test] ':' suite */

  if (!Match(TokenType::Def))
  {
    return nullptr;
  }

  if (PeekType() != TokenType::Identifier)
  {
    return nullptr;
  }

  auto id = Peek().lexeme;
  Advance();
  auto name = MakeShared<Identifier>(id);

  auto args = Parameters();

  AstNodePtr returns = nullptr;
  if (Match(TokenType::Arrow))
  {
    returns = Test();
    if (returns == nullptr)
    {
      ErrorMessage("Missing statement after arrow token");
      return nullptr;
    }
  }

  if (!Match(TokenType::Colon))
  {
    ErrorMessage("Missing colon after function parameter list");
    return nullptr;
  }

  auto body = Suite();
  if (body.empty())
  {
    ErrorMessage("Empty function body");
    return nullptr;
  }

  if (async)
  {
    return MakeShared<AsyncFunctionDefStatement>(name, args, body, decorators, returns);
  }

  return MakeShared<FunctionDefStatement>(name, args, body, decorators, returns);
}

Vector<AstNodePtr> Parser::Stmt()
{
#ifdef DEBUG
  std::cout << "Stmt()\n";
#endif
  Vector<AstNodePtr> result = SimpleStmt();
  if (!result.empty())
  {
    return result;
  }
  
  AstNodePtr stmt = CompoundStmt();
  if (stmt)
  {
    return {stmt};
  }
  
  return {};
}

Vector<AstNodePtr> Parser::SimpleStmt()
{
#ifdef DEBUG
  std::cout << "SimpleStmt()\n";
#endif

  auto stmts = GetList(&Parser::SmallStmt, TokenType::SemiColon);
  if (!Match(TokenType::NewLine))
  {
    ErrorMessage("No new line at the end of statement");
    return {};
  }
  
  return stmts;
}

AstNodePtr Parser::CompoundStmt()
{
#ifdef DEBUG
  std::cout << "CompundStmt()\n";
#endif

  AstNodePtr stmt;
  if (stmt = IfStmt()) return stmt;
  if (stmt = WhileStmt()) return stmt;
  if (stmt = ForStmt()) return stmt;
  if (stmt = TryStmt()) return stmt;
  if (stmt = WithStmt()) return stmt;
  if (stmt = FuncDef()) return stmt;
  if (stmt = ClassDef()) return stmt;
  if (stmt = Decorated()) return stmt;
  if (stmt = AsyncStmt()) return stmt;

  return nullptr;
}

AstNodePtr Parser::AsyncStmt()
{
#ifdef DEBUG
  std::cout << "AsyncStmt()\n";
#endif
  /* async_stmt: ASYNC (funcdef | with_stmt | for_stmt) */
  if (!Match(TokenType::Async))
  {
    return nullptr;
  }

  AstNodePtr stmt;
  if ((stmt = FuncDef(true))) return stmt;
  if ((stmt = WithStmt(true))) return stmt;
  if ((stmt = ForStmt(true))) return stmt;

  ErrorMessage("Invalid async statement");
  return nullptr;
}

AstNodePtr Parser::SmallStmt()
{
#ifdef DEBUG
  std::cout << "SmallStmt()\n";
#endif

  AstNodePtr result;
  if ((result = ExprStmt()) ||
      (result = DelStmt()) ||
      (result = PassStmt()) ||
      (result = FlowStmt()) ||
      (result = ImportStmt()) ||
      (result = GlobalStmt()) ||
      (result = NonLocalStmt()) ||
      (result = AssertStmt()))
  {
    return result;
  }

  return nullptr;
}

AstNodePtr Parser::ExprStmt()
{
#ifdef DEBUG
  std::cout << "ExprStmt()\n";
#endif
  Vector<AstNodePtr> leftSide = TestListStarExpr();
  if (leftSide.empty())
  {
    return nullptr;
  }
  
  if (MatchOneOf({TokenType::PlusAssign,
                  TokenType::MinusAssign,
                  TokenType::MulAssign,
                  TokenType::DivAssign,
                  TokenType::ModAssign,
                  TokenType::AndAssign,
                  TokenType::OrAssign,
                  TokenType::XorAssign,
                  TokenType::ShiftLeftAssign,
                  TokenType::ShiftRightAssign,
                  TokenType::ExpAssign,
                  TokenType::DivDivAssign}, false))
  {
    if (leftSide.size() > 1)
    {
      ErrorMessage("Illegal expression on left side of augmented assign");
      return nullptr;
    }

    // TODO: Fix this; make another enum for augmeneted assign
    int plusAssign = EnumCast<int>(TokenType::PlusAssign);
    int plus = EnumCast<int>(TokenType::Plus);
    int normalOp = EnumCast<int>(PeekType()) - (plusAssign - plus);
    Advance();
    Operator op = EnumCast<Operator>(normalOp);
    
    auto value = Test();
    if (value == nullptr)
    {
      ErrorMessage("Illegal expression on right side of augmented assign");
      return nullptr;
    }

    return MakeShared<AugAssignStatement>(leftSide.front(), op, value);
  }

  // TODO: Give appropriate context
  Vector<AstNodePtr> exprs = leftSide;
  Vector<AstNodePtr> rightSide;
  while (true)
  {
    if (!Match(TokenType::Assign))
    {
      for (auto &_ : rightSide)
        exprs.pop_back();
      break;
    }
    if (!(rightSide = TestListStarExpr()).empty())
    {
      for (auto &p : rightSide)
        exprs.push_back(p);
    }
    else
    {
      ErrorMessage("Error in assignment");
      return nullptr;
    }
  }

  return MakeShared<AssignStatement>(exprs, rightSide);
}

Vector<AstNodePtr> Parser::TestListStarExpr()
{
#ifdef DEBUG
  std::cout << "TestListStarStmt()\n";
#endif

  return GetList({&Parser::Test, &Parser::StarExpr});
}

AstNodePtr Parser::DelStmt()
{
#ifdef DEBUG
  std::cout << "DelStmt()\n";
#endif

  if (!Match(TokenType::Del))
  {
    return nullptr;
  }

  auto exprList = ExprList();
  return MakeShared<DeleteStatement>(exprList);
}

AstNodePtr Parser::PassStmt()
{
#ifdef DEBUG
  std::cout << "PassStmt()\n";
#endif

  if(!Match(TokenType::Pass))
  {
    return nullptr;
  }

  return MakeShared<PassStatement>();
}

AstNodePtr Parser::FlowStmt()
{
#ifdef DEBUG
  std::cout << "FlowStmt()\n";
#endif

  AstNodePtr result;
  if (result = BreakStmt()) return result;
  if (result = ContinueStmt()) return result;
  if (result = ReturnStmt()) return result;
  if (result = RaiseStmt()) return result;
  if (result = YieldStmt()) return result;

  return nullptr;
}

AstNodePtr Parser::ImportStmt()
{
#ifdef DEBUG
  std::cout << "ImportStmt()\n";
#endif

  AstNodePtr result;
  if (result = ImportName()) return result;
  if (result = ImportFrom()) return result;
  return nullptr;
}

AstNodePtr Parser::GlobalStmt()
{
#ifdef DEBUG
  std::cout << "GlobalStmt()\n";
#endif

  if (!Match(TokenType::Global))
  {
    return nullptr;
  }

  Vector<AstNodePtr> names;
  
  Token t;
  do
  {
    if (!Match(TokenType::Identifier, false))
    {
      ErrorMessage("Invalid 'global' statement");
      return nullptr;
    }
    auto lex = Peek().lexeme;
    names.emplace_back(MakeShared<Identifier>(lex));
    Advance();
  } while (Match(TokenType::Comma));

  return MakeShared<GlobalStatement>(names);
}

AstNodePtr Parser::NonLocalStmt()
{
#ifdef DEBUG
  std::cout << "NonLocalStmt()\n";
#endif

  if (!Match(TokenType::NonLocal))
  {
    return nullptr;
  }

  Vector<AstNodePtr> names;
  
  Token t;
  do
  {
    if (!Match(TokenType::Identifier, false))
    {
      ErrorMessage("Invalid 'nonlocal' statement");
      return nullptr;
    }
    auto lexeme = Peek().lexeme;
    names.emplace_back(MakeShared<Identifier>(lexeme));
    Advance();
  } while (Match(TokenType::Comma));

  return MakeShared<NonlocalStatement>(names);
}

AstNodePtr Parser::AssertStmt()
{
#ifdef DEBUG
  std::cout << "AssertStmt()\n";
#endif

  if (!Match(TokenType::Assert))
  {
    return nullptr;
  }

  AstNodePtr test = Test();
  if (test == nullptr)
  {
    ErrorMessage("Invalid 'assert' expression");
    return nullptr;
  }
  
  AstNodePtr msg = nullptr;
  if (Match(TokenType::Comma))
  {
    msg = Test();
    if (msg == nullptr)
    {
      ErrorMessage("Invalid 'assert' expression");
      return nullptr;
    }
  }
  
  return MakeShared<AssertStatement>(test, msg); 
}

AstNodePtr Parser::YieldExpr()
{
  if (!Match(TokenType::Yield))
  {
    ErrorMessage("Invalid 'yield' expression");
    return nullptr;
  }

  auto arg = YieldArg();
  //TODO: Support 'yield from'
  //if (arg == nullptr)
  //{
  //  return MakeShared<YieldExpression>(arg);
  //}
  return MakeShared<YieldExpression>(arg);
}

Vector<AstNodePtr> Parser::YieldArg()
{
  // if (Match(TokenType::From))
  // {
  //   auto val = Test();
  //   if (val == nullptr)
  //   {
  //     ErrorMessage("Invalid 'yield from' statement");
  //     return nullptr;
  //   }
  //   return val;
  // }

  // auto list = TestList();
  // auto ctx = ExprContext::None;
  // return MakeShared<ListExpression>(list, ctx);
  return TestList();
}

Vector<AstNodePtr> Parser::TestList()
{
  return GetList(&Parser::Test);
}

Vector<AstNodePtr> Parser::ExprList()
{
  return GetList({&Parser::Expr, &Parser::StarExpr});
}

AstNodePtr Parser::BreakStmt()
{
  if (!Match(TokenType::Break))
  {
    return nullptr;
  }

  return MakeShared<BreakStatement>();
}

AstNodePtr Parser::ContinueStmt()
{
  if (!Match(TokenType::Continue))
  {
    return nullptr;
  }

  return MakeShared<ContinueStatement>();
}

AstNodePtr Parser::ReturnStmt()
{
  if (!Match(TokenType::Return))
  {
    return nullptr;
  }

  auto list = TestList();
  auto ctx = ExprContext::Load;
  auto result = MakeShared<ListExpression>(list, ctx);
  return MakeShared<ReturnStatement>(result);
}

AstNodePtr Parser::RaiseStmt()
{
  if (!Match(TokenType::Raise))
  {
    return nullptr;
  }

  auto exc = Test();
  AstNodePtr cause;
  if (exc && Match(TokenType::From))
  {
    cause = Test();
  }
  
  return MakeShared<RaiseStatement>(exc, cause);
}

AstNodePtr Parser::YieldStmt()
{
  return YieldExpr();
}

AstNodePtr Parser::ImportName()
{
  if (!Match(TokenType::Import))
  {
    return nullptr;
  }

  auto names = DottedAsNames();
  if (names.empty())
  {
    ErrorMessage("Invalid import statement");
    return nullptr;
  }

  return MakeShared<ImportStatement>(names);
}

AstNodePtr Parser::ImportFrom()
{
  if (!Match(TokenType::From))
  {
    return nullptr;
  }

  String dottedNameAsString;
  auto dottedName = DottedName();

  int level;
  if (dottedName == nullptr)
  {
    while (true)
    {
      if (Match(TokenType::Dot)) dottedNameAsString.push_back('.');
      if (Match(TokenType::Ellipsis)) dottedNameAsString.append("...");
    }
    if (dottedName = DottedName())
    {
      dottedNameAsString.append(dynamic_cast<Identifier*>(dottedName.get())->GetId());
    }
    dynamic_cast<Identifier*>(dottedName.get())->SetId(dottedNameAsString);
  }
  
  if (!Match(TokenType::Import))
  {
    ErrorMessage("Invalid 'import from' statement");
    return nullptr;
  }

  if (Match(TokenType::Mul))
  {
    auto star = MakeShared<Identifier>("*");
    return SharedPtr<ImportFromStatement>(new ImportFromStatement(dottedName, {star}, level));
  }

  if (Match(TokenType::LeftParen))
  {
    auto aliases = ImportAsNames();
    if (aliases.empty() || !Match(TokenType::RightParen))
    {
      ErrorMessage("Invalid 'import from' statement - wrong alias definition");
      return nullptr;
    }

    return MakeShared<ImportFromStatement>(dottedName, aliases, level);
  }

  auto aliases = ImportAsNames();
  if (aliases.empty() || !Match(TokenType::RightParen))
  {
    ErrorMessage("Invalid 'import from' statement - wrong alias definition");
    return nullptr;
  }

  return MakeShared<ImportFromStatement>(dottedName, aliases, level);
}

Vector<AstNodePtr> Parser::ImportAsNames()
{
  return GetList(&Parser::ImportAsName);
}

AstNodePtr Parser::ImportAsName()
{
  if (PeekType() != TokenType::Identifier)
  {
    return nullptr;
  }

  auto impId = Peek().lexeme;
  Advance();
  auto imp = MakeShared<Identifier>(impId);

  AstNodePtr alias = nullptr;
  if (!Match(TokenType::As))
  {
    return MakeShared<Alias>(imp, alias);
  }

  if (PeekType() != TokenType::Identifier)
  {
    ErrorMessage("Missing alias after token 'as'");
    return nullptr;
  }

  impId = Peek().lexeme;
  Advance();
  alias = MakeShared<Identifier>(impId);

  return MakeShared<Alias>(imp, alias);
}

Vector<AstNodePtr> Parser::DottedAsNames()
{
  /* dotted_as_names: dotted_as_name (',' dotted_as_name)* */
  // Cant use GetList here because because last comma is not allowed

  Vector<AstNodePtr> result;
  AstNodePtr name;

  do
  {
    name = DottedAsName();
    if (name == nullptr)
    {
      return {};
    }
    result.push_back(name);
  } while (Match(TokenType::Comma));

  return result;
}

AstNodePtr Parser::DottedAsName()
{
  auto name = DottedName();
  if (name == nullptr)
  {
    return nullptr;
  }

  AstNodePtr alias = nullptr;
  if (Match(TokenType::As))
  {
    if (PeekType() != TokenType::Identifier)
    {
      ErrorMessage("Missing import alias");
      return nullptr;
    }

    auto id = Peek().lexeme;
    Advance();
    alias = MakeShared<Identifier>(id);
  }

  return MakeShared<Alias>(name, alias);
}

AstNodePtr Parser::DottedName()
{
  String name;

  do
  {
    if (PeekType() != TokenType::Identifier)
    {
      return nullptr;
    }
    name.append(Peek().lexeme);
    Advance();
  } while (Match(TokenType::Dot));

  return MakeShared<Identifier>(name);
}

AstNodePtr Parser::Test()
{
#ifdef DEBUG
  std::cout << "Test()\n";
#endif
  auto body = OrTest();
  if (body == nullptr)
  {
    return LambdaDef();
  }

  if (!Match(TokenType::If))
  {
    return body;
  }

  auto cond = OrTest();
  if (cond == nullptr)
  {
    ErrorMessage("Invalid test statement, missing condition");
    return nullptr;
  }

  AstNodePtr elseExpr;
  if (!Match(TokenType::Else) || !(elseExpr = OrTest()))
  {
    ErrorMessage("Invalid else statement in test");
    return nullptr;
  }

  return MakeShared<IfExpression>(cond, body, elseExpr);
}

AstNodePtr Parser::Expr()
{
#ifdef DEBUG
  std::cout << "Expr\n";
#endif
  auto left = XorExpr();

  if (left == nullptr)
  {
    return nullptr;
  }

  AstNodePtr expr = left;
  auto op = Operator::BitwiseOr;
  while (true)
  {
    if (!Match(TokenType::BitwiseOr))
    {
      break;
    }

    auto right = XorExpr();
    if (right == nullptr)
    {
      return nullptr;
    }
    
    expr = MakeShared<BinOpExpression>(expr, op, right);
  }

  return expr;
}

AstNodePtr Parser::XorExpr()
{
#ifdef DEBUG
  std::cout << "XorExpr\n";
#endif
  auto left = AndExpr();

  if (left == nullptr)
  {
    return nullptr;
  }

  AstNodePtr expr = left;
  auto op = Operator::BitwiseXor;

  while (true)
  {
    if (!Match(TokenType::BitwiseXor))
    {
      break;
    }

    auto right = AndExpr();
    if (right == nullptr)
    {
      return nullptr;
    }
    expr = MakeShared<BinOpExpression>(expr, op, right);
  }

  return expr;
}

AstNodePtr Parser::AndExpr()
{
#ifdef DEBUG
  std::cout << "AndExpr\n";
#endif
  auto left = ShiftExpr();

  if (left == nullptr)
  {
    return nullptr;
  }

  AstNodePtr expr = left;
  auto op = Operator::BitwiseAnd;
  while (true)
  {
    if (!Match(TokenType::BitwiseAnd))
    {
      break;
    }

    auto right = ShiftExpr();
    if (right == nullptr)
    {
      ErrorMessage("Syntax error in right operand of shift expression");
      return nullptr;
    }

    expr = MakeShared<BinOpExpression>(expr, op, right);
  }

  return expr;
}

AstNodePtr Parser::ShiftExpr()
{
#ifdef DEBUG
  std::cout << "ShiftExpr\n";
#endif
  auto left = ArithExpr();

  if (left == nullptr)
  {
    return nullptr;
  }

  AstNodePtr expr = left;
  while (true)
  {
    if (Match(TokenType::ShiftLeft, false) || Match(TokenType::ShiftRight, false))
    {
      auto op = EnumCast<Operator>(PeekType());
      Advance();
      
      auto right = ArithExpr();
      if (right == nullptr)
      {
        return nullptr;
      }
      expr = MakeShared<BinOpExpression>(expr, op, right);
    }
    break;
  }
  
  return expr;
}

AstNodePtr Parser::ArithExpr()
{
#ifdef DEBUG
  std::cout << "ArithExpr\n";
#endif
  auto left = Term();

  if (left == nullptr)
  {
    return nullptr;
  }

  AstNodePtr expr = left;
  while (true)
  {
    if (MatchOneOf({TokenType::Plus, TokenType::Minus}, false))
    {
      auto op = EnumCast<Operator>(PeekType());
      Advance();
      
      auto right = Term();
      if (right == nullptr)
      {
        return nullptr;
      }
      expr = MakeShared<BinOpExpression>(expr, op, right);
    }
    
    break;
  }

  return expr;  
}

AstNodePtr Parser::Term()
{
#ifdef DEBUG
  std::cout << "Term\n";
#endif
  auto left = Factor();

  if (left == nullptr)
  {
    return nullptr;
  }
  
  AstNodePtr expr = left;
  while (true)
  {
    if (MatchOneOf({TokenType::Mul, TokenType::Div, TokenType::Mod, TokenType::DivDiv}, false))
    {
      auto op = EnumCast<Operator>(PeekType());
      Advance();
  
      auto right = Term();
      if (right == nullptr)
      {
        return nullptr;
      }
      expr = MakeShared<BinOpExpression>(expr, op, right);
    }
    
    break;
  }

  return expr;
}

AstNodePtr Parser::Factor()
{
#ifdef DEBUG
  std::cout << "Factor()\n";
#endif  
  if (!MatchOneOf({TokenType::Plus, TokenType::Minus, TokenType::BitwiseNot}, false))
  {
    return Power();
  }
  
  auto op = EnumCast<UnaryOp>(Peek().type);
  Advance();
  
  auto operand = Factor();
  if (operand == nullptr)
  {
    ErrorMessage("Invalid operand in unary expression");
    return nullptr;
  }
  return MakeShared<UnaryOpExpression>(op, operand);
}

AstNodePtr Parser::Power()
{
#ifdef DEBUG
  std::cout << "Power()\n";
#endif  
  auto left = AtomExpr();
  if (left == nullptr)
  {
    return nullptr;
  }

  if (!Match(TokenType::Exp))
  {
    return left; 
  }

  auto right = Factor();
  if (right == nullptr)
  {
    return nullptr;
  }

  auto op = Operator::Exp;
  return MakeShared<BinOpExpression>(left, op, right);
}

AstNodePtr Parser::AtomExpr()
{
#ifdef DEBUG
  std::cout << "AtomExpr()\n";
#endif  
  /* atom_expr: [AWAIT] atom trailer* */

  bool await = Match(TokenType::Await);

  auto atom = Atom();
  if (atom == nullptr)
  {
    if (await)
    {
      ErrorMessage("Syntax error");
    }
    return nullptr;
  }

  auto expr = atom;
  while (true)
  {
    auto topExpr = Trailer(expr);
    if (topExpr == nullptr)
    {
      break;
    }
    expr = topExpr;
  }

  if (await)
  {
    return MakeShared<AwaitExpression>(expr);
  }

  return expr;
}

AstNodePtr Parser::Atom()
{
#ifdef DEBUG
  std::cout << "Atom()\n";
#endif  
  AstNodePtr expr;
  Vector<AstNodePtr> elts;
  if (Match(TokenType::LeftParen)) 
  {
    if (expr == YieldExpr())
    {
      if (!Match(TokenType::RightParen))
      {
        ErrorMessage("Missing closing parenthesis");
        return nullptr;
      }
      elts.push_back(expr);
    }
    else if ((expr == Test()) || (expr == StarExpr()))
    {
      // Either list comprehension or just list
      auto comp = CompFor();
      if (!comp.empty())
      {
        if (!Match(TokenType::RightParen))
        {
          ErrorMessage("Missing closing parenthesis");
          return nullptr;
        }
        return MakeShared<ListCompExpression>(expr, comp);
      }

      elts.push_back(expr);
      auto list = TestListStarExpr();
      for (auto &el : list)
      {
        elts.push_back(el);
      }
      if (!Match(TokenType::RightParen))
      {
        ErrorMessage("Missing closing parenthesis");
        return nullptr;
      }
    }
    else
    {
      ErrorMessage("Syntax error in tuple declaration");
      return nullptr;
    }
    
    return SharedPtr<TupleExpression>(new TupleExpression(elts, ExprContext::None));
  }

  if (Match(TokenType::LeftBracket))
  {
    if ((expr == Test()) || (expr == StarExpr()))
    {
      // Either list comprehension or just list
      auto comp = CompFor();
      if (!comp.empty())
      {
        if (!Match(TokenType::RightParen))
        {
          ErrorMessage("Missing closing parenthesis");
          return nullptr;
        }
        return MakeShared<ListCompExpression>(expr, comp);
      }

      elts.push_back(expr);
      auto list = TestListStarExpr();
      for (auto &el : list)
      {
        elts.push_back(el);
      }
      if (!Match(TokenType::RightParen))
      {
        ErrorMessage("Missing closing parenthesis");
        return nullptr;
      }
    }
    else
    {
      ErrorMessage("Syntax error in tuple declaration");
      return nullptr;
    }
    return SharedPtr<ListExpression>(new ListExpression(elts, ExprContext::None));
  }

  if (Match(TokenType::LeftBrace))
  {
    if (expr == DictOrSetMarker())
    {
      if (!Match(TokenType::RightBrace))
      {
        ErrorMessage("Missing closing brace");
        return nullptr;
      }
      return expr;
    }
    if (!Match(TokenType::RightBrace))
    {
      ErrorMessage("Missing closing brace");
      return nullptr;
    }
    return expr;
  }

  if (Match(TokenType::Identifier, false))
  {
    auto lexeme = Peek().lexeme;
    auto res = MakeShared<Identifier>(lexeme);
    Advance();
    return res;
  }

  if (Match(TokenType::Number, false))
  {
    auto res = SharedPtr<NumExpression>(new NumExpression(Peek().number));
    Advance();
    return res;
  }

  if (Match(TokenType::String, false))
  {
    auto lexeme = Peek().lexeme;
    auto res = MakeShared<Identifier>(lexeme);
    Advance();
    return res;
  }

  if (Match(TokenType::Ellipsis))
  {
    return SharedPtr<EllipsisExpression>();
  }

  if (MatchOneOf({TokenType::None, TokenType::False, TokenType::True}, false))
  {
    auto t = EnumCast<SingletonTokenType>(PeekType());
    Advance();
    return MakeShared<NameConstantExpression>(t);
  }

  ErrorMessage("Invalid syntax");
  return nullptr;
}

AstNodePtr Parser::Trailer(AstNodePtr expr)
{
#ifdef DEBUG
  std::cout << "Trailer()\n";
#endif
  switch (PeekType())
  {
  case TokenType::LeftParen:
    return Call(expr);
  case TokenType::LeftBrace:
    return SubscriptExpr(expr);
  case TokenType::Dot:
    return Attribute(expr);
  default:
    return nullptr;
  }
}

AstNodePtr Parser::Call(AstNodePtr func)
{
  
  return nullptr;
  /* Call(expr func, expr* args, keyword* keywords) */
  if (!Match(TokenType::LeftParen))
  {
    return nullptr;
  }

  // TODO: Args and Keywords
  // Handle arglist !!!! ?!?!?!?!
}

AstNodePtr Parser::Attribute(AstNodePtr value)
{
  if (!Match(TokenType::Dot))
  {
    return nullptr;
  }

  auto attr = GetName();
  if (attr == nullptr)
  {
    ErrorMessage("Syntax error in access");
    return nullptr;
  }

  auto ctx = ExprContext::None;
  return MakeShared<AttributeExpression>(value, attr, ctx);
}

AstNodePtr Parser::SubscriptExpr(AstNodePtr value)
{
  /* Subscript(expr value, slice slice, expr_context ctx) */
  if (!Match(TokenType::LeftBracket))
  {
    return nullptr;
  }

  auto dims = SubscriptList();

  if (dims.empty())
  {
    ErrorMessage("Invalid subscript access");
    return nullptr;
  }

  AstNodePtr slice;
  if (dims.size() == 1) // 1 dim
  {  
    slice = dims.front();
  }
  else
  {
    slice = MakeShared<ExtSlice>(dims);
  }

  auto ctx = ExprContext::None;
  return MakeShared<SubscriptExpression>(value, slice, ctx);
}

Vector<AstNodePtr> Parser::SubscriptList()
{
  return GetList(&Parser::Subscript);
}

AstNodePtr Parser::Subscript()
{
  auto low = Test();

  if (low == nullptr)
  {
    return nullptr;
  }
  
  if (!Match(TokenType::Colon))
  {
    return MakeShared<IndexSlice>(low);
  }

  auto high = Test();
  if (high == nullptr)
  {
    return SharedPtr<Slice>(new Slice(low, nullptr, nullptr));
  }

  auto step = SliceOp();
  if (step == nullptr)
  {
    return SharedPtr<Slice>(new Slice(low, high, nullptr));
  }

  return MakeShared<Slice>(low, high, step);
}

AstNodePtr Parser::SliceOp()
{
  if (!Match(TokenType::Colon))
  {
    return nullptr;
  }

  return Test();
}

AstNodePtr Parser::DictOrSetMarker()
{
  auto key = Test();

  Vector<AstNodePtr> dictToExpand;
  Vector<AstNodePtr> setToExpand;
  AstNodePtr value;
  
  if (key == nullptr)
  {
    if (Match(TokenType::Exp))
    {
      if (!(key = Expr()))
      {
        ErrorMessage("Invalid dict declaration");
        return nullptr;
      }
      dictToExpand.push_back(key);
      goto comp_or_dict;
    }

    if (!(key = StarExpr()))
    {
      ErrorMessage("Invalid set declaration");
      return nullptr;
    }

    setToExpand.push_back(key);
    goto comp_or_set;
  }

  if (Match(TokenType::Colon))
  {
    value = Test();
    if (value == nullptr)
    {
      return nullptr;
    }
  comp_or_dict:
    auto comprehension = CompFor();
    if (!comprehension.empty())
    {
      Match(TokenType::Comma);
      return MakeShared<DictCompExpression>(key, value, comprehension);
    }
    
    if (!Match(TokenType::Comma))
    {
      return SharedPtr<DictExpression>(new DictExpression({key}, {value}));
    }

    // Missing parsing an optional comma here but no wories
    
    Vector<AstNodePtr> keys{key};
    Vector<AstNodePtr> values{value};
    while (true)
    {
      if (key = Test())
      {
        if (!Match(TokenType::Colon) || !(value = Test()))
        {
          ErrorMessage("Invalid dictionary declaration");
          return nullptr;
        }
        keys.push_back(key);
        values.push_back(value);
        if (!Match(TokenType::Comma)) break;
      }
      if (Match(TokenType::Exp))
      {
        auto expr = Expr();
        // TODO: expand arguments or store them somehow for later expansion
        dictToExpand.push_back(expr);
        if (!Match(TokenType::Comma)) break;  
      }
      return MakeShared<DictExpression>(keys, values);
    }
    Match(TokenType::Comma);
    // TODO: expand arguments or store them somehow for later expansion
    return MakeShared<DictExpression>(keys, values);
  }

 comp_or_set:
  auto comprehension = CompFor();
  if (!comprehension.empty())
  {
    return MakeShared<SetCompExpression>(key, comprehension);
  }

  if (!Match(TokenType::Comma))
  {
    return SharedPtr<SetExpression>(new SetExpression({key}));
  }

  Vector<AstNodePtr> elems{key};
  Vector<AstNodePtr> toExpand;
  while (true)
  {
    if (key = Test())
    {
      elems.push_back(key);
      if (!Match(TokenType::Comma)) break;
    }
    if (key = StarExpr())
    {
      toExpand.push_back(key);
      if (!Match(TokenType::Comma)) break;
    }

    return MakeShared<SetExpression>(elems);
  }
  Match(TokenType::Comma);
  return MakeShared<SetExpression>(elems);
}

AstNodePtr Parser::StarExpr()
{
  if (!Match(TokenType::Mul))
  {
    return nullptr;
  }

  return Expr();
}

AstNodePtr Parser::OrTest()
{
#ifdef DEBUG
  std::cout << "OrTest()\n";
#endif
  AstNodePtr expr;
  Vector<AstNodePtr> values;
  
  do
  {
    expr = AndTest();

    if (expr == nullptr)
    {
      return nullptr;
    }
    values.push_back(expr);
  } while (Match(TokenType::Or));

  if (values.size() == 1)
  {
    return values[0];
  }   
  
  auto op = BoolOp::Or;
  return MakeShared<BoolOpExpression>(op, values);
}

AstNodePtr Parser::AndTest()
{
  
#ifdef DEBUG
  std::cout << "AndTest()\n";
#endif
  AstNodePtr expr;
  Vector<AstNodePtr> values;

  do
  {
    expr = NotTest();
    
    if (expr == nullptr)
    {
      return nullptr;
    }
    
    values.push_back(expr);
  } while (Match(TokenType::And));

  if (values.size() == 1)
  {
    return values[0];
  }   
  
  auto op = BoolOp::And;
  return MakeShared<BoolOpExpression>(op, values);
}

AstNodePtr Parser::NotTest()
{ 
#ifdef DEBUG
  std::cout << "NotTest()\n";
#endif
  if (!Match(TokenType::Not))
  {
    return Comparison();
  }
  
  auto operand = NotTest();
  auto op = UnaryOp::Not;
  return MakeShared<UnaryOpExpression>(op, operand);
}

AstNodePtr Parser::Comparison()
{
#ifdef DEBUG
    std::cout << "Comparison()\n";
#endif

  auto left = Expr();
  if (left == nullptr)
  {
    return nullptr;
  }
  
  Vector<CmpOp> ops;
  Vector<AstNodePtr> comparators;
  while (true)
  {
    if (MatchOneOf({TokenType::Less,
                    TokenType::More,
                    TokenType::Equal,
                    TokenType::MoreEqual,
                    TokenType::LessEqual,
                    TokenType::NotEqual,
                    TokenType::In,
                    TokenType::Is,
                    TokenType::Not}, false))
    {
      auto op = EnumCast<CmpOp>(PeekType());

      if (PeekType() == TokenType::Is && PeekNext() == TokenType::Not)
      {
        op = CmpOp::IsNot;
        Advance();
      }
      
      if (PeekType() == TokenType::Not)
      {
        if (PeekNext() != TokenType::In)
        {
          return nullptr;
        }
        op = CmpOp::NotIn;
        Advance();
      }
      
      Advance();
      
      auto right = Expr();
      if (right == nullptr)
      {
        ErrorMessage("Invalid comparison expression");
        return nullptr;
      }
      ops.push_back(op);
      comparators.push_back(right);
    }

    break;
  }

  if (ops.empty())
  {
    return left;
  }

  return MakeShared<CompareExpression>(left, ops, comparators);
}

AstNodePtr Parser::LambdaDef()
{
  if (!Match(TokenType::Lambda))
  {
    return nullptr;
  }

  auto varArgsList = VarArgsList();

  if (!Match(TokenType::Colon))
  {
    ErrorMessage("Missing colon after lambda");
    return nullptr;
  }

  auto body = Test();
  if (body == nullptr)
  {
    ErrorMessage("Empty lambda body");
    return nullptr;
  }

  return MakeShared<LambdaExpression>(varArgsList, body);
}

AstNodePtr Parser::IfStmt(bool elif)
{
#ifdef DEBUG
  std::cout << "IfStmt()\n";
#endif
  if (!elif && !Match(TokenType::If))
  {
    return nullptr;
  }

  if (elif && !Match(TokenType::Elif))
  {
    return nullptr;
  }

  auto condition = Test();

  // TODO: Error messages depending on 'elif' param
  if (!Match(TokenType::Colon))
  {
    ErrorMessage("Missing colon after if condition");
    return nullptr;
  }
  
  auto body = Suite(); // TODO: Suite returns Vector<Statement> <-- talking about asdl nodes here
  if (body.empty())
  {
    ErrorMessage("Missing if statement body"); 
    return nullptr;  
  }

  Vector<AstNodePtr> orElseStmts;
  if (Match(TokenType::Elif, false))
  {
    auto orElse = IfStmt(true);
    if (orElse == nullptr)
    {
      ErrorMessage("Missing elif body");
      return nullptr;
    }
    
    orElseStmts.push_back(orElse);
    return MakeShared<IfStatement>(condition, body, orElseStmts);
  }

  if (Match(TokenType::Else))
  {
    if (!Match(TokenType::Colon))
    {
      ErrorMessage("Missing colon after else");
      return nullptr;
    }

    orElseStmts = Suite();
    if (orElseStmts.empty())
    {
      ErrorMessage("Missing else body");
      return nullptr;
    }
    
    return MakeShared<IfStatement>(condition, body, orElseStmts);
  }

  return MakeShared<IfStatement>(condition, body, orElseStmts);
}

Vector<AstNodePtr> Parser::Suite()
{
#ifdef DEBUG
  std::cout << "Suite()\n";
#endif
  /* suite: simple_stmt | NEWLINE INDENT stmt+ DEDENT */
  Vector<AstNodePtr> result;

  if (!Match(TokenType::NewLine))
  {
    result = SimpleStmt();
    if (!result.empty())
    {
      return result;
    }

    ErrorMessage("No new line after colon");
    return {};
  }

  if (!Match(TokenType::Indent))
  {
    ErrorMessage("No indent");
    return {};
  }

  
  while (!Match(TokenType::Dedent))
  {
    auto stmts = Stmt();
    if (stmts.empty())
    {
      break;
    }

    for (auto &stmt : stmts)
    {
      result.push_back(stmt);
    }
  }

  return result;
}

AstNodePtr Parser::WhileStmt()
{
#ifdef DEBUG
  std::cout << "WhileStmt()\n";
#endif
  /* asdl While(expr test, stmt* body, stmt* orelse) */
  /* while_stmt: 'while' test ':' suite ['else' ':' suite] */

  if (!Match(TokenType::While))
  {
    return nullptr;
  }

  auto test = Test();

  if (test == nullptr) /* why do i write things like I would write them in go or C */
  {
    ErrorMessage("Missing while test");
    return nullptr;
  }

  if (!Match(TokenType::Colon))
  {
    ErrorMessage("Missing colon after while test");
    return nullptr;
  }

  auto body = Suite();
  if (body.empty())
  {
    ErrorMessage("Syntax error in body of while");
    return nullptr;
  }
  
  Vector<AstNodePtr> orElse;

  if (Match(TokenType::Else))
  {
    if (!Match(TokenType::Colon))
    {
      ErrorMessage("Missing colon after else");
      return nullptr;
    }

    orElse = Suite();
    if (orElse.empty())
    {
      ErrorMessage("Syntax error in body of else");
      return nullptr;
    }
  }

  return MakeShared<WhileStatement>(test, body, orElse);
}

AstNodePtr Parser::ForStmt(bool async)
{
#ifdef DEBUG
  std::cout << "ForStmt()\n";
#endif
  /* asdl (Async)For(expr target, expr iter, stmt* body, stmt* orelse) */
  /* for_stmt: 'for' exprlist 'in' testlist ':' suite ['else' ':' suite] */

  if (!Match(TokenType::For))
  {
    return nullptr;
  }

  auto targets = ExprList();

  if (targets.empty()) /* why do i write things like I would write them in go or C */
  {
    ErrorMessage("Missing 'for' iterator(s)");
    return nullptr;
  }

  auto storeCtx = ExprContext::Store;
  auto target = MakeShared<TupleExpression>(targets, storeCtx);

  if (!Match(TokenType::In))
  {
    ErrorMessage("Missing 'in' token");
    return nullptr;
  }

  auto toIterate = TestList();  
  if (toIterate.empty()) /* why do i write things like I would write them in go or C */
  {
    ErrorMessage("Missing 'for' target object");
    return nullptr;
  }

  if (!Match(TokenType::Colon))
  {
    ErrorMessage("Missing colon after 'for' statement");
    return nullptr;
  }

  auto loadCtx = ExprContext::Load;
  auto iter = MakeShared<TupleExpression>(toIterate, loadCtx);

  auto body = Suite();
  if (body.empty())
  {
    ErrorMessage("Syntax error in body of while");
    return nullptr;
  }
  
  Vector<AstNodePtr> orElse;

  if (Match(TokenType::Else))
  {
    if (!Match(TokenType::Colon))
    {
      ErrorMessage("Missing colon after else");
      return nullptr;
    }

    orElse = Suite();
    if (orElse.empty())
    {
      ErrorMessage("Syntax error in body of else");
      return nullptr;
    }
  }

  if (async)
  {
    return MakeShared<AsyncForStatement>(target, iter, body, orElse);
  }

  return MakeShared<ForStatement>(target, iter, body, orElse);
}

AstNodePtr Parser::TryStmt()
{
#ifdef DEBUG
  std::cout << "TryStmt()\n";
#endif
  return nullptr;
}

AstNodePtr Parser::WithStmt(bool async)
{
#ifdef DEBUG
  std::cout << "WithStmt()\n";
#endif
  return nullptr;
}

AstNodePtr Parser::WithItem()
{
  auto name = Test();
  if (name == nullptr)
  {
    return nullptr;
  }

  AstNodePtr alias = nullptr;
  if (!Match(TokenType::As))
  {
    return MakeShared<Alias>(name, alias);
  }

  alias = Expr();
  if (alias == nullptr)
  {
    ErrorMessage("Missing alias for 'with' statement");
    return nullptr;
  }
  
  return MakeShared<Alias>(name, alias);
}

AstNodePtr Parser::ClassDef(Vector<AstNodePtr> decorators)
{
#ifdef DEBUG
  std::cout << "ClassDef()\n";
#endif
  /* asdl 
     ClassDef(identifier name,
              expr* bases,
              keyword* keywords,
              stmt* body,
              expr* decorator_list) */
  /* classdef: 'class' NAME ['(' [arglist] ')'] ':' suite */

  if (!Match(TokenType::Class))
  {
    return nullptr;
  }

  auto name = GetName(false);
  if (name == nullptr)
  {
    ErrorMessage("Missing class name");
    return nullptr;
  }

  Vector<AstNodePtr> bases;
  if (Match(TokenType::LeftParen))
  {
    bases = ArgList();
    if (!Match(TokenType::RightParen))
    {
      ErrorMessage("Missing closing parenthesis after class bases declaration");
      return nullptr;
    }
  }

  if (!Match(TokenType::Colon))
  {
    ErrorMessage("Missing colon after class definition");
    return nullptr;
  }

  auto body = Suite();
  if (body.empty())
  {
    ErrorMessage("Missing class body");
    return nullptr;
  }

  Vector<AstNodePtr> keywords; // TODO
  return MakeShared<ClassDefStatement>(name, bases, keywords, body, decorators);
}

Vector<AstNodePtr> Parser::ArgList()
{
  return GetList(&Parser::Argument);
}

AstNodePtr Parser::Argument()
{
  /*
    argument:
    ( NAME [comp_for] |
      keyword '=' test |
      '**' NAME |
      '*' NAME )
  */
  
  if (PeekType() == TokenType::Identifier)
  {
    auto name = GetName();
    if (Match(TokenType::Assign))
    {
      auto name = GetName();

      Match(TokenType::Assign);
      
      auto value = Test();
      if (value = nullptr)
      {
        ErrorMessage("Invalid keywords use");
        return nullptr;
      }

      return MakeShared<Keyword>(name, value);
    }
    // Else generator
    auto comprehensions = CompFor();
    return MakeShared<GeneratorExpression>(name, comprehensions);
  }

  AstNodePtr kwargsId = nullptr;
  auto ctx = ExprContext::None;
  if (Match(TokenType::Exp))
  {
    auto identifier = GetName();
    if (identifier == nullptr)
    {
      ErrorMessage("Invalid kwargs argument");
      return nullptr;
    }

    auto value = MakeShared<StarredExpression>(identifier, ctx);
    return MakeShared<Keyword>(kwargsId, value);
  }

  if (Match(TokenType::Mul))
  {
    auto identifier = GetName();
    if (identifier == nullptr)
    {
      ErrorMessage("Invalid varargs argument");
      return nullptr;
    }

    auto value = MakeShared<StarredExpression>(identifier, ctx);
    return MakeShared<Keyword>(identifier, value);
  }

  return nullptr;
}

Vector<AstNodePtr> Parser::CompFor()
{

  /* comprehension = (expr target, expr iter, expr* ifs) */
  /* comp_for: 'for' exprlist 'in' or_test [comp_iter] */
  Vector<AstNodePtr> generators;
  while (Match(TokenType::For))
  {
    auto targets = ExprList();

    if (targets.empty())
    {
      ErrorMessage("Missing 'for' iterator(s)");
      return {};
    }

    auto storeCtx = ExprContext::Store;
    auto target = MakeShared<TupleExpression>(targets, storeCtx);

    if (!Match(TokenType::In))
    {
      ErrorMessage("Missing 'in' token");
      return {};
    }

    auto toIterate = TestList();  
    if (toIterate.empty()) /* why do i write things like I would write them in go or C */
    {
      ErrorMessage("Missing 'for' iteration object(s)");
      return {};
    }

    auto loadCtx = ExprContext::Load;
    auto iter = MakeShared<TupleExpression>(toIterate, loadCtx);
    
    auto ifs = CompIf(); // May be empty
    
    auto comp = MakeShared<Comprehension>(target, iter, ifs);
    generators.push_back(comp);
  }

  return generators;
}

Vector<AstNodePtr> Parser::CompIf()
{
  if (!Match(TokenType::If))
  {
    return {};
  }
  
  Vector<AstNodePtr> ifs;
  AstNodePtr test;
  while ((test = TestNoCond()))
  {
    ifs.push_back(test);
  }

  return ifs;
}

AstNodePtr Parser::TestNoCond()
{
  AstNodePtr test;
  if (test = OrTest()) return test;
  if (test = LambdaDefNoCond()) return test;

  ErrorMessage("Syntax error in comprehension condition");
  return nullptr;
}

AstNodePtr Parser::LambdaDefNoCond()
{
  if (!Match(TokenType::Lambda))
  {
    return nullptr;
  }

  auto args = VarArgsList();

  if (!Match(TokenType::Colon))
  {
    ErrorMessage("No colon after lambda declaration in comprehension");
    return nullptr;
  }

  auto body = TestNoCond();
  if(body == nullptr)
  {
    ErrorMessage("Missing lambda body in comprehension");
    return nullptr;
  }
  
  return MakeShared<LambdaExpression>(args, body);
} 

/* API */
AstNodePtr Parse(const Vector<Token> &tokens, ParserResult &result, ParserOptions options)
{
  Parser p(tokens, options);
  return p.Parse(result);
}

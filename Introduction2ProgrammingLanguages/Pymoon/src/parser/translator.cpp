#include <cassert>
#include "ast.hpp"
#include "translator.hpp"

Translator::Translator(OutputStream &os) :
  m_Output(os),
  m_Indent(0) {}

void Translator::Translate(AstNode *v)
{
  Visit(v);
}

void Translator::Visit(AstNode *v)
{
  v->Accept(this);
}

void Translator::Indent()
{
  assert(m_Indent % 4 == 0);
  m_Indent += 4;
}

void Translator::Dedent()
{
  assert(m_Indent > 0 && m_Indent % 4 == 0);
  m_Indent -= 4;
}

void Translator::PrintIndent()
{
  for (int i = 0; i < m_Indent; ++i)
  {
    m_Output << ' ';
  }
}

void Translator::Visit(Identifier *v)
{
  m_Output << v->GetId();
}

void Translator::Visit(PyModule *v)
{
  auto body = v->GetBody();
  for (auto &stmt : body)
  {
    Visit(stmt.get());
  }  
}

void Translator::Visit(Statement *v)
{
  // NOT IMPLEMENTED
}

void Translator::Visit(FunctionDefStatement *v)
{
  /* FunctionDef(identifier name, arguments args,
     stmt* body, expr* decorator_list, expr? returns) */

  assert(m_Indent == 0);
  auto name = v->GetName();
  auto args = v->GetArgs();
  // varargs, kwargs, keywords...
  auto body = v->GetBody();

  assert(name != nullptr);
  assert(args != nullptr);
  assert(!body.empty());
  // decorators, return type...
  m_Output << "function ";
  Visit(name.get());
  m_Output << " (";
  Visit(args.get());
  m_Output << ")\n";
  Indent();
  for (auto &stmt : body)
  {
    Visit(stmt.get());
  }
  Dedent();
  m_Output << "end\n\n";
}

void Translator::Visit(AsyncFunctionDefStatement *v)
{
  // NOT IMPLEMENTED
}

void Translator::Visit(ClassDefStatement *v)
{
  // NOT IMPLEMENTED
}

void Translator::Visit(ReturnStatement *v)
{
  auto value = v->GetValue();
  PrintIndent();
  m_Output << "return ";
  if (value)
  {
    Visit(value.get());
  }
  m_Output << "\n";
}

void Translator::Visit(DeleteStatement *v)
{
  // NOT IMPLEMENTED
}

void Translator::Visit(AssignStatement *v)
{
  auto targets = v->GetTargets();
  auto values = v->GetValues();
  assert(targets.size() == values.size());
  for (int i = 0; i < targets.size(); ++i)
  {
    PrintIndent();
    Visit(targets[i].get());
    m_Output << " = ";
    Visit(values[i].get());
    m_Output << "\n";
  }
}

void Translator::Visit(AugAssignStatement *v)
{
  PrintIndent();
  auto target = v->GetTarget();
  auto value = v->GetValue();
  Visit(target.get());
  m_Output << " = ";
  Visit(target.get());
  switch (v->GetOp())
  {
  case Operator::Plus:
    m_Output << " + ";
    break;
  case Operator::Minus:
    m_Output << " - ";
    break;
  default:
    m_Output << " ? ";
    break;
  }
  Visit(value.get());
  m_Output << "\n";  
}

void Translator::Visit(ForStatement *v)
{
  // NOT IMPLEMENTED
}

void Translator::Visit(AsyncForStatement *v)
{
  // NOT IMPLEMENTED
}

void Translator::Visit(WhileStatement *v)
{
  PrintIndent();

  m_Output << "while ";
  Visit(v->GetTest().get());
  m_Output << " do\n";
  Indent();
  for (auto &stmt : v->GetBody()) Visit(stmt.get());
  Dedent();
  // else
  m_Output << "end\n\n";
}

void Translator::Visit(IfStatement *v)
{
  PrintIndent();
  m_Output << "if ";
  Visit(v->GetTest().get());
  m_Output << " then\n";
  Indent();
  for (auto &stmt : v->GetBody()) Visit(stmt.get());
  Dedent();
  // else
  m_Output << "end\n\n";
}

void Translator::Visit(WithStatement *v)
{
  // NOT IMPLEMENTED
}

void Translator::Visit(AsyncWithStatement *v)
{
  // NOT IMPLEMENTED
}

void Translator::Visit(RaiseStatement *v)
{
  // NOT IMPLEMENTED
}

void Translator::Visit(TryStatement *v)
{
  // NOT IMPLEMENTED
}

void Translator::Visit(AssertStatement *v)
{
  // NOT IMPLEMENTED
}

void Translator::Visit(ImportStatement *v)
{
  // NOT IMPLEMENTED
}

void Translator::Visit(ImportFromStatement *v)
{
  // NOT IMPLEMENTED
}

void Translator::Visit(GlobalStatement *v)
{
  // NOT IMPLEMENTED
}

void Translator::Visit(NonlocalStatement *v)
{
  // NOT IMPLEMENTED
}

void Translator::Visit(ExprStatement *v)
{
  // NOT IMPLEMENTED
}

void Translator::Visit(PassStatement *v)
{
  // NOT IMPLEMENTED
}

void Translator::Visit(BreakStatement *v)
{
  // NOT IMPLEMENTED
}

void Translator::Visit(ContinueStatement *v)
{
  // NOT IMPLEMENTED
}

void Translator::Visit(Expression *v)
{
  // NOT IMPLEMENTED
}

void Translator::Visit(BoolOpExpression *v)
{
  // NOT IMPLEMENTED
}

void Translator::Visit(BinOpExpression *v)
{
  // NOT IMPLEMENTED
}

void Translator::Visit(UnaryOpExpression *v)
{
  // NOT IMPLEMENTED
}

void Translator::Visit(LambdaExpression *v)
{
  // NOT IMPLEMENTED
}

void Translator::Visit(IfExpression *v)
{
  // NOT IMPLEMENTED
}

void Translator::Visit(DictExpression *v)
{
  // NOT IMPLEMENTED
}

void Translator::Visit(SetExpression *v)
{
  // NOT IMPLEMENTED 
}

void Translator::Visit(ListCompExpression *v)
{
  // NOT IMPLEMENTED
}

void Translator::Visit(SetCompExpression *v)
{
  // NOT IMPLEMENTED
}

void Translator::Visit(DictCompExpression *v)
{
  // NOT IMPLEMENTED
}

void Translator::Visit(GeneratorExpression *v)
{
  // NOT IMPLEMENTED
}

void Translator::Visit(AwaitExpression *v)
{
  // NOT IMPLEMENTED
}

void Translator::Visit(YieldExpression *v)
{
  // NOT IMPLEMENTED
}

void Translator::Visit(YieldFromExpression *v)
{
  // NOT IMPLEMENTED
}

void Translator::Visit(CompareExpression *v)
{
  //#ifdef DEBUG
  std::cout << "Translator::Visit(CompareExpression*)\n";
  //#endif
  auto left = v->GetLeft();
  auto ops = v->GetOps();
  auto comparators = v->GetComparators();

  Visit(left.get());
  for (int i = 0; i < ops.size(); ++i)
  {
    switch (ops[i])
    {
    case CmpOp::Equal:
      m_Output << " == ";
      break;
    case CmpOp::More:
      m_Output << " > ";
      break;
    default:
      std::cout << "cmp op: " << static_cast<int>(ops[i]) << std::endl;
      m_Output << " cmpOp ";
      break;
    }
    Visit(comparators[i].get());
    if (i != ops.size() - 1)
    {
      m_Output << " and ";
      Visit(comparators[i].get());
    }
  }
}

void Translator::Visit(CallExpression *v)
{
  // NOT IMPLEMENTED
}

void Translator::Visit(NumExpression *v)
{
  m_Output << v->GetN();
}

void Translator::Visit(StrExpression *v)
{
  m_Output << '"' << v->GetS() << '"';
}

void Translator::Visit(NameConstantExpression *v)
{
  // NOT IMPLEMENTED
}

void Translator::Visit(EllipsisExpression *v)
{
  // NOT IMPLEMENTED
}

void Translator::Visit(AttributeExpression *v)
{
  // NOT IMPLEMENTED
}

void Translator::Visit(SubscriptExpression *v)
{
  // NOT IMPLEMENTED
}

void Translator::Visit(StarredExpression *v)
{
  // NOT IMPLEMENTED
}

void Translator::Visit(NameExpression *v)
{
  // NOT IMPLEMENTED
}

void Translator::Visit(ListExpression *v)
{
  for (auto &el : v->GetElts())
  {
    Visit(el.get());
  }
}

void Translator::Visit(TupleExpression *v)
{
  // NOT IMPLEMENTED
}

void Translator::Visit(Slice *v)
{
  // NOT IMPLEMENTED
}

void Translator::Visit(ExtSlice *v)
{
  // NOT IMPLEMENTED
}

void Translator::Visit(IndexSlice *v)
{
  // NOT IMPLEMENTED
}

void Translator::Visit(Comprehension *v)
{
  // NOT IMPLEMENTED
}

void Translator::Visit(ExceptHandler *v)
{
  // NOT IMPLEMENTED
}

void Translator::Visit(Arguments *v)
{
  auto args = v->GetArgs();

  for (int i = 0; i < args.size(); ++i)
  {
    Visit(args[i].get());
    if (i < args.size() - 1)
    {
      m_Output << ", ";
    }
  }
}

void Translator::Visit(Arg *v)
{
  Visit(v->GetArg().get());
}

void Translator::Visit(Keyword *v)
{
  // NOT IMPLEMENTED
}

void Translator::Visit(Alias *v)
{
  // NOT IMPLEMENTED
}

void Translator::Visit(WithItem *v)
{
  // NOT IMPLEMENTED
}

void Translator::Visit(Decorator *v)
{
  // NOT IMPLEMENTED
}


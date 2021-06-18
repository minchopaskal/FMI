#pragma once
#include "node_visitor.hpp"
#include "../types.hpp"

class Translator : public NodeVisitor
{
public:
  Translator(OutputStream &os);
  virtual ~Translator() = default;

  void Translate(AstNode *v);
  
  virtual void Visit(Identifier *v);
  virtual void Visit(PyModule *v);
  virtual void Visit(Statement *v);
  virtual void Visit(FunctionDefStatement *v);
  virtual void Visit(AsyncFunctionDefStatement *v);
  virtual void Visit(ClassDefStatement *v);
  virtual void Visit(ReturnStatement *v);
  virtual void Visit(DeleteStatement *v);
  virtual void Visit(AssignStatement *v);
  virtual void Visit(AugAssignStatement *v);
  virtual void Visit(ForStatement *v);
  virtual void Visit(AsyncForStatement *v);
  virtual void Visit(WhileStatement *v);
  virtual void Visit(IfStatement *v);
  virtual void Visit(WithStatement *v);
  virtual void Visit(AsyncWithStatement *v);
  virtual void Visit(RaiseStatement *v);
  virtual void Visit(TryStatement *v);
  virtual void Visit(AssertStatement *v);
  virtual void Visit(ImportStatement *v);
  virtual void Visit(ImportFromStatement *v);
  virtual void Visit(GlobalStatement *v);
  virtual void Visit(NonlocalStatement *v);
  virtual void Visit(ExprStatement *v);
  virtual void Visit(PassStatement *v);
  virtual void Visit(BreakStatement *v);
  virtual void Visit(ContinueStatement *v);
  virtual void Visit(Expression *v);
  virtual void Visit(BoolOpExpression *v);
  virtual void Visit(BinOpExpression *v);
  virtual void Visit(UnaryOpExpression *v);
  virtual void Visit(LambdaExpression *v);
  virtual void Visit(IfExpression *v);
  virtual void Visit(DictExpression *v);
  virtual void Visit(SetExpression *v);
  virtual void Visit(ListCompExpression *v);
  virtual void Visit(SetCompExpression *v);
  virtual void Visit(DictCompExpression *v);
  virtual void Visit(GeneratorExpression *v);
  virtual void Visit(AwaitExpression *v);
  virtual void Visit(YieldExpression *v);
  virtual void Visit(YieldFromExpression *v);
  virtual void Visit(CompareExpression *v);
  virtual void Visit(CallExpression *v);
  virtual void Visit(NumExpression *v);
  virtual void Visit(StrExpression *v);
  virtual void Visit(NameConstantExpression *v);
  virtual void Visit(EllipsisExpression *v);
  virtual void Visit(AttributeExpression *v);
  virtual void Visit(SubscriptExpression *v);
  virtual void Visit(StarredExpression *v);
  virtual void Visit(NameExpression *v);
  virtual void Visit(ListExpression *v);
  virtual void Visit(TupleExpression *v);
  virtual void Visit(Slice *v);
  virtual void Visit(ExtSlice *v);
  virtual void Visit(IndexSlice *v);
  virtual void Visit(Comprehension *v);
  virtual void Visit(ExceptHandler *v);
  virtual void Visit(Arguments *v);
  virtual void Visit(Arg *v);
  virtual void Visit(Keyword *v);
  virtual void Visit(Alias *v);
  virtual void Visit(WithItem *v);
  virtual void Visit(Decorator *v);

private:
  void Visit(AstNode *v);
  void Indent();
  void Dedent();
  void PrintIndent();
  
private:
  OutputStream& m_Output;
  unsigned m_Indent;
};

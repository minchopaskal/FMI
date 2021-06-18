#include <iostream>
#include "print_visitor.hpp"

void PrintVisitor::Print(AstNode *v)
{
  v->Accept(this);
}

void PrintVisitor::Visit(Identifier *v)
{
  auto id = v->GetId();
  std::cout << "Identifier: " << id << std::endl;
}

void PrintVisitor::Visit(PyModule *v)
{
  auto body = v->GetBody();
  for (auto &stmt : body)
  {
    Print(stmt.get());
  }  
}

void PrintVisitor::Visit(Statement *v)
{
  std::cout << "Statement" << std::endl;
}

void PrintVisitor::Visit(FunctionDefStatement *v)
{
  std::cout << "FunctionDefStatement" << std::endl;
}

void PrintVisitor::Visit(AsyncFunctionDefStatement *v)
{
  std::cout << "AsyncFunctionDefStatement" << std::endl;
}

void PrintVisitor::Visit(ClassDefStatement *v)
{
  std::cout << "ClassDefStatement" << std::endl;
}

void PrintVisitor::Visit(ReturnStatement *v)
{
  std::cout << "ReturnStatement" << std::endl;
}

void PrintVisitor::Visit(DeleteStatement *v)
{
  std::cout << "DeleteStatement" << std::endl;
}

void PrintVisitor::Visit(AssignStatement *v)
{
  auto target = v->GetTargets();
  auto value = v->GetValues();
  std::cout << "Assign:\n";
  std::cout << "Targets: ";
  for (auto &t : target)
  {
    Print(t.get());
  }
  std::cout << "value: ";
  for (auto &v : value) Print(v.get());
  std::cout << "\n";
}

void PrintVisitor::Visit(AugAssignStatement *v)
{
  std::cout << "AugAssignStatement" << std::endl;
}

void PrintVisitor::Visit(ForStatement *v)
{
  std::cout << "ForStatement" << std::endl;
}

void PrintVisitor::Visit(AsyncForStatement *v)
{
  std::cout << "AsyncForStatement" << std::endl;
}

void PrintVisitor::Visit(WhileStatement *v)
{
  std::cout << "WhileStatement" << std::endl;
}

void PrintVisitor::Visit(IfStatement *v)
{
  auto test = v->GetTest();
  auto body = v->GetBody();
  auto orelse = v->GetBody();
  std::cout << "IfStatement:\n";
  Print(test.get());
  for (auto &b : body) Print(b.get());
}

void PrintVisitor::Visit(WithStatement *v)
{
  std::cout << "WithStatement" << std::endl;
}

void PrintVisitor::Visit(AsyncWithStatement *v)
{
  std::cout << "AsyncWithStatement" << std::endl;
}

void PrintVisitor::Visit(RaiseStatement *v)
{
  std::cout << "RaiseStatement" << std::endl;
}

void PrintVisitor::Visit(TryStatement *v)
{
  std::cout << "TryStatement" << std::endl;
}

void PrintVisitor::Visit(AssertStatement *v)
{
  std::cout << "AssertStatement" << std::endl;
}

void PrintVisitor::Visit(ImportStatement *v)
{
  std::cout << "ImportStatement" << std::endl;
}

void PrintVisitor::Visit(ImportFromStatement *v)
{
  std::cout << "ImportFromStatement" << std::endl;
}

void PrintVisitor::Visit(GlobalStatement *v)
{
  std::cout << "GlobalStatement" << std::endl;
}

void PrintVisitor::Visit(NonlocalStatement *v)
{
  std::cout << "NonlocalStatement" << std::endl;
}

void PrintVisitor::Visit(ExprStatement *v)
{
  std::cout << "ExprStatement" << std::endl;
}

void PrintVisitor::Visit(PassStatement *v)
{
  std::cout << "PassStatement" << std::endl;
}

void PrintVisitor::Visit(BreakStatement *v)
{
  std::cout << "BreakStatement" << std::endl;
}

void PrintVisitor::Visit(ContinueStatement *v)
{
  std::cout << "ContinueStatement" << std::endl;
}

void PrintVisitor::Visit(Expression *v)
{
  std::cout << "Expression" << std::endl;
}

void PrintVisitor::Visit(BoolOpExpression *v)
{
  std::cout << "BoolOpExpression" << std::endl;
  // std::cout << "Operator: " << static_cast<int>(v->GetOp());
  // swtich ...
  // for (auto &val : v->GetValues()) Print(val.get());
}

void PrintVisitor::Visit(BinOpExpression *v)
{
  std::cout << "BinOpExpression" << std::endl;
}

void PrintVisitor::Visit(UnaryOpExpression *v)
{
  std::cout << "UnaryOpExpression" << std::endl;
}

void PrintVisitor::Visit(LambdaExpression *v)
{
  std::cout << "LambdaExpression" << std::endl;
}

void PrintVisitor::Visit(IfExpression *v)
{
  std::cout << "IfExpression" << std::endl;
}

void PrintVisitor::Visit(DictExpression *v)
{
  std::cout << "DictExpression" << std::endl;
}

void PrintVisitor::Visit(SetExpression *v)
{
  std::cout << "SetExpression" << std::endl;
}

void PrintVisitor::Visit(ListCompExpression *v)
{
  std::cout << "ListCompExpression" << std::endl;
}

void PrintVisitor::Visit(SetCompExpression *v)
{
  std::cout << "SetCompExpression" << std::endl;
}

void PrintVisitor::Visit(DictCompExpression *v)
{
  std::cout << "DictCompExpression" << std::endl;
}

void PrintVisitor::Visit(GeneratorExpression *v)
{
  std::cout << "GeneratorExpression" << std::endl;
}

void PrintVisitor::Visit(AwaitExpression *v)
{
  std::cout << "AwaitExpression" << std::endl;
}

void PrintVisitor::Visit(YieldExpression *v)
{
  std::cout << "YieldExpression" << std::endl;
}

void PrintVisitor::Visit(YieldFromExpression *v)
{
  std::cout << "YieldFromExpression" << std::endl;
}

void PrintVisitor::Visit(CompareExpression *v)
{
  std::cout << "CompareExpression" << std::endl;
}

void PrintVisitor::Visit(CallExpression *v)
{
  std::cout << "CallExpression" << std::endl;
}

void PrintVisitor::Visit(NumExpression *v)
{
  std::cout << "NumExpression: " << v->GetN() << std::endl;
}

void PrintVisitor::Visit(StrExpression *v)
{
  std::cout << "StrExpression" << std::endl;
}

void PrintVisitor::Visit(NameConstantExpression *v)
{
  std::cout << "NameConstantExpression" << std::endl;
}

void PrintVisitor::Visit(EllipsisExpression *v)
{
  std::cout << "EllipsisExpression" << std::endl;
}

void PrintVisitor::Visit(AttributeExpression *v)
{
  std::cout << "AttributeExpression" << std::endl;
}

void PrintVisitor::Visit(SubscriptExpression *v)
{
  std::cout << "SubscriptExpression" << std::endl;
}

void PrintVisitor::Visit(StarredExpression *v)
{
  std::cout << "StarredExpression" << std::endl;
}

void PrintVisitor::Visit(NameExpression *v)
{
  std::cout << "NameExpression" << std::endl;
}

void PrintVisitor::Visit(ListExpression *v)
{
  std::cout << "ListExpression" << std::endl;
}

void PrintVisitor::Visit(TupleExpression *v)
{
  std::cout << "TupleExpression" << std::endl;
}

void PrintVisitor::Visit(Slice *v)
{
  std::cout << "Slice" << std::endl;
}

void PrintVisitor::Visit(ExtSlice *v)
{
  std::cout << "ExtSlice" << std::endl;
}

void PrintVisitor::Visit(IndexSlice *v)
{
  std::cout << "IndexSlice" << std::endl;
}

void PrintVisitor::Visit(Comprehension *v)
{
  std::cout << "Comprehension" << std::endl;
}

void PrintVisitor::Visit(ExceptHandler *v)
{
  std::cout << "ExceptHandler" << std::endl;
}

void PrintVisitor::Visit(Arguments *v)
{
  std::cout << "Arguments" << std::endl;
}

void PrintVisitor::Visit(Arg *v)
{
  std::cout << "Arg" << std::endl;
}

void PrintVisitor::Visit(Keyword *v)
{
  std::cout << "Keyword" << std::endl;
}

void PrintVisitor::Visit(Alias *v)
{
  std::cout << "Alias" << std::endl;
}

void PrintVisitor::Visit(WithItem *v)
{
  std::cout << "WithItem" << std::endl;
}

void PrintVisitor::Visit(Decorator *v)
{
  std::cout << "Decorator" << std::endl;
}


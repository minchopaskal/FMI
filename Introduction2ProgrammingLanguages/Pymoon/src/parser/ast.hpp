#pragma once

#include "ast_definitions.hpp"
#include "node_visitor.hpp"
#include "../types.hpp"

enum class Operator
{
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
  ShiftRight
};

enum class BoolOp
{
  And = 300,
  Or
};

enum class UnaryOp
{
  Invert = 210,
  Not = 302,
  UAdd = 200,
  USub = 201
};

enum class CmpOp
{
  Equal = 250,
  NotEqual,
  MoreEqual,
  LessEqual,
  More,
  Less,
  In = 350,
  Is,
  IsNot,
  NotIn
};

enum class ExprContext
{
  Load,
  Store,
  Del,
  AugLoad,
  AugStore,
  Param,
  None
};

class AstNode : public EnableShared<AstNode>
{
public:
  AstNode() :
    m_Line(0),
    m_Column(0) {}
  virtual ~AstNode() = default;
  virtual void Accept(NodeVisitor *v) = 0;
  inline size_t GetLine() const { return m_Line; }
  inline size_t GetColumn() const { return m_Column; }
  void SetPosition(size_t line, size_t col) { m_Line = line; m_Column = col; }
private:
  size_t m_Line;
  size_t m_Column;
};

#define INITALIZER_LIST(Name, Type) \
  Type Name,

#define EXPAND_INITIALIZER_LIST(Name, Type) \
  m_##Name(Name),

#define DEFINE_GETTERS(Name, Type)              \
  virtual Type& Get##Name() { return m_##Name; }        \
  virtual const Type& Get##Name() const { return m_##Name; }

#define DEFINE_MEMBERS(Name, Type) \
  Type m_##Name;

#define DEFINE_SETTERS(Name, Type) \
  void Set##Name(Type Name) { m_##Name = Name; }

#define DEFINE_AST_NODE(ClassName, ITERATOR)     \
class ClassName : public AstNode            \
{                                                        \
public:                                                  \
  ClassName(ITERATOR(INITALIZER_LIST) bool dummy = 0) \
    :                                                             \
    ITERATOR(EXPAND_INITIALIZER_LIST) m_Dummy(dummy) {}       \
  virtual ~ClassName() = default;                        \
  ITERATOR(DEFINE_GETTERS)                       \
  ITERATOR(DEFINE_SETTERS) \
  virtual void Accept(NodeVisitor *v) override { v->Visit(this); } \
private:                                         \
  ITERATOR(DEFINE_MEMBERS)                       \
  bool m_Dummy;                                    \
};

AST_NODE_ITERATOR(DEFINE_AST_NODE)
#undef DEFINE_AST_NODE
#undef DEFINE_SETTERS
#undef DEFINE_GETTERS
#undef DEFINE_MEMBERS
#undef INITALIZER_LIST
#undef EXPAND_INITIALIZER_LIST

#define ALIAS_AST_NODES(ClassName, ITERATOR) \
  using ClassName##Ptr = SharedPtr<ClassName>;

AST_NODE_ITERATOR(ALIAS_AST_NODES)
#undef ALIAS_AST_NODES

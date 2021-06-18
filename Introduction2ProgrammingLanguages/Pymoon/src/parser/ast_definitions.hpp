#pragma once
#include "../types.hpp"
#include "../lexer/token_types.hpp"

#define EMPTY_BODY(MACRO)

/* asdl-builtin */
#define IDENTIFIER_NODE_MEMBERS(MACRO) \
  MACRO(Id, String)

/* mod */
#define PY_MODULE_NODE_MEMBERS(MACRO) \
  MACRO(Body, Vector<AstNodePtr>)

/* stmt */
#define FUNCTION_DEF_NODE_MEMBERS(MACRO) \
  MACRO(Name, AstNodePtr) \
  MACRO(Args, AstNodePtr) \
  MACRO(Body, Vector<AstNodePtr>) \
  MACRO(Decorator_list, Vector<AstNodePtr>) \
  MACRO(Returns, AstNodePtr)

#define ASYNC_FUNCTION_DEF_NODE_MEMBERS(MACRO) \
  MACRO(Name, AstNodePtr) \
  MACRO(Args, AstNodePtr) \
  MACRO(Body, Vector<AstNodePtr>) \
  MACRO(Decorator_list, Vector<AstNodePtr>) \
  MACRO(Returns, AstNodePtr)

#define CLASS_DEF_NODE_MEMBERS(MACRO) \
  MACRO(Name, AstNodePtr) \
  MACRO(Bases, Vector<AstNodePtr>) \
  MACRO(Keywords, Vector<AstNodePtr>) \
  MACRO(Body, Vector<AstNodePtr>) \
  MACRO(Decorator_list, Vector<AstNodePtr>)

#define RETURN_NODE_MEMBERS(MACRO) \
  MACRO(Value, AstNodePtr)

#define DELETE_NODE_MEMBERS(MACRO) \
  MACRO(Targets, Vector<AstNodePtr>)

#define ASSIGN_NODE_MEMBERS(MACRO) \
  MACRO(Targets, Vector<AstNodePtr>) \
  MACRO(Values, Vector<AstNodePtr>)

#define AUG_ASSIGN_NODE_MEMBERS(MACRO) \
  MACRO(Target, AstNodePtr) \
  MACRO(Op, Operator) \
  MACRO(Value, AstNodePtr)

#define FOR_NODE_MEMBERS(MACRO) \
  MACRO(Target, AstNodePtr) \
  MACRO(Iter, AstNodePtr) \
  MACRO(Body, Vector<AstNodePtr>) \
  MACRO(Orelse, Vector<AstNodePtr>)

#define ASYNC_FOR_NODE_MEMBERS(MACRO) \
  MACRO(Target, AstNodePtr) \
  MACRO(Iter, AstNodePtr) \
  MACRO(Body, Vector<AstNodePtr>) \
  MACRO(Orelse, Vector<AstNodePtr>)

#define WHILE_NODE_MEMBERS(MACRO) \
  MACRO(Test, AstNodePtr) \
  MACRO(Body, Vector<AstNodePtr>) \
  MACRO(Orelse, Vector<AstNodePtr>)

#define IF_NODE_MEMBERS(MACRO) \
  MACRO(Test, AstNodePtr) \
  MACRO(Body, Vector<AstNodePtr>) \
  MACRO(Orelse, Vector<AstNodePtr>)

#define WITH_NODE_MEMBERS(MACRO) \
  MACRO(Items, Vector<AstNodePtr>) \
  MACRO(Body, Vector<AstNodePtr>)

#define ASYNC_WITH_NODE_MEMBERS(MACRO) \
  MACRO(Items, Vector<AstNodePtr>) \
  MACRO(Body, Vector<AstNodePtr>)

#define RAISE_NODE_MEMBERS(MACRO) \
  MACRO(Exc, AstNodePtr) \
  MACRO(Cause, AstNodePtr)

#define TRY_NODE_MEMBERS(MACRO) \
  MACRO(Body, Vector<AstNodePtr>) \
  MACRO(Handlers, Vector<AstNodePtr>) \
  MACRO(Orelse, Vector<AstNodePtr>) \
  MACRO(Finalbody, Vector<AstNodePtr>)

#define ASSERT_NODE_MEMBERS(MACRO) \
  MACRO(Test, AstNodePtr) \
  MACRO(Msg, AstNodePtr)

#define IMPORT_NODE_MEMBERS(MACRO) \
  MACRO(Names, Vector<AstNodePtr>)

#define IMPORT_FROM_NODE_MEMBERS(MACRO) \
  MACRO(Module, AstNodePtr) \
  MACRO(Names, Vector<AstNodePtr>) \
  MACRO(Level, int)

#define GLOBAL_NODE_MEMBERS(MACRO) \
  MACRO(Names, Vector<AstNodePtr>)

#define NONLOCAL_NODE_MEMBERS(MACRO) \
  MACRO(Names, Vector<AstNodePtr>)

#define EXPR_NODE_MEMBERS(MACRO) \
  MACRO(Value, AstNodePtr)

/* expr */
#define BOOL_OP_NODE_MEMBERS(MACRO) \
  MACRO(Op, BoolOp) \
  MACRO(Values, Vector<AstNodePtr>)

#define BIN_OP_NODE_MEMBERS(MACRO) \
  MACRO(Left, AstNodePtr) \
  MACRO(Op, Operator) \
  MACRO(Right, AstNodePtr)

#define UNARY_OP_NODE_MEMBERS(MACRO) \
  MACRO(Op, UnaryOp) \
  MACRO(Operand, AstNodePtr)

#define LAMBDA_NODE_MEMBERS(MACRO) \
  MACRO(Args, AstNodePtr) \
  MACRO(Body, AstNodePtr)

#define IF_EXP_NODE_MEMBERS(MACRO) \
  MACRO(Test, AstNodePtr) \
  MACRO(Body, AstNodePtr) \
  MACRO(Orelse, AstNodePtr)

#define DICT_NODE_MEMBERS(MACRO) \
  MACRO(Keys, Vector<AstNodePtr>) \
  MACRO(Values, Vector<AstNodePtr>)

#define SET_NODE_MEMBERS(MACRO) \
  MACRO(Elts, Vector<AstNodePtr>)

#define LIST_COMP_NODE_MEMBERS(MACRO) \
  MACRO(Elt, AstNodePtr) \
  MACRO(Generators, Vector<AstNodePtr>)

#define SET_COMP_NODE_MEMBERS(MACRO) \
  MACRO(Elt, AstNodePtr) \
  MACRO(Generators, Vector<AstNodePtr>)

#define DICT_COMP_NODE_MEMBERS(MACRO) \
  MACRO(Key, AstNodePtr) \
  MACRO(Elt, AstNodePtr) \
  MACRO(Generators, Vector<AstNodePtr>)

#define GENERATR_EXPR_NODE_MEMBERS(MACRO) \
  MACRO(Elt, AstNodePtr) \
  MACRO(Generators, Vector<AstNodePtr>)

#define AWAIT_NODE_MEMBERS(MACRO) \
  MACRO(Value, AstNodePtr)

#define YIELD_NODE_MEMBERS(MACRO) \
  MACRO(Value, Vector<AstNodePtr>)

#define YIELD_FROM_NODE_MEMBERS(MACRO) \
  MACRO(Value, AstNodePtr)

#define COMPARE_NODE_MEMBERS(MACRO) \
  MACRO(Left, AstNodePtr) \
  MACRO(Ops, Vector<CmpOp>) \
  MACRO(Comparators, Vector<AstNodePtr>)

#define CALL_NODE_MEMBERS(MACRO) \
  MACRO(Func, AstNodePtr) \
  MACRO(Args, Vector<AstNodePtr>) \
  MACRO(Keywords, Vector<AstNodePtr>)

#define NUM_NODE_MEMBERS(MACRO) \
  MACRO(N, double)

#define STR_NODE_MEMBERS(MACRO) \
  MACRO(S, String)

#define NAME_CONSTANT_NODE_MEMBERS(MACRO) \
  MACRO(Value, SingletonTokenType)

#define ATTRIBUTE_NODE_MEMBERS(MACRO) \
  MACRO(Value, AstNodePtr) \
  MACRO(Attr, AstNodePtr) \
  MACRO(Ctx, ExprContext)

#define SUBSCRIPT_NODE_MEMBERS(MACRO) \
  MACRO(Value, AstNodePtr) \
  MACRO(Slice, AstNodePtr) \
  MACRO(Ctx, ExprContext)

#define STARRED_NODE_MEMBERS(MACRO) \
  MACRO(Value, AstNodePtr) \
  MACRO(Ctx, ExprContext)

#define NAME_NODE_MEMBERS(MACRO) \
  MACRO(Id, AstNodePtr) \
  MACRO(Ctx, ExprContext)

#define LIST_NODE_MEMBERS(MACRO) \
  MACRO(Elts, Vector<AstNodePtr>) \
  MACRO(Ctx, ExprContext)

#define TUPLE_NODE_MEMBERS(MACRO) \
  MACRO(Elts, Vector<AstNodePtr>) \
  MACRO(Ctx, ExprContext)

/* slice */
#define SLICE_NODE_MEMBERS(MACRO) \
  MACRO(Lower, AstNodePtr) \
  MACRO(Upper, AstNodePtr) \
  MACRO(Step, AstNodePtr)

#define EXT_SLICE_NODE_MEMBERS(MACRO) \
  MACRO(Dims, Vector<AstNodePtr>)

#define INDEX_NODE_MEMBERS(MACRO) \
  MACRO(Value, AstNodePtr)

/* comprehension */
#define COMPREHENSION_NODE_MEMBERS(MACRO) \
  MACRO(Target, AstNodePtr) \
  MACRO(Iter, AstNodePtr) \
  MACRO(Ifs, Vector<AstNodePtr>)

/* excepthandler */
#define EXCEPT_HANDLER_NODE_MEMBERS(MACRO) \
  MACRO(Type, AstNodePtr) \
  MACRO(Name, AstNodePtr) \
  MACRO(Body, AstNodePtr)

/* arguments */
#define ARGUMENTS_NODE_MEMBERS(MACRO) \
  MACRO(Args, Vector<AstNodePtr>) \
  MACRO(Vararg, AstNodePtr) \
  MACRO(Kwarg, AstNodePtr) \
  MACRO(Defaults, Vector<AstNodePtr>)

/* arg */
#define ARG_NODE_MEMBERS(MACRO) \
  MACRO(Arg, AstNodePtr) \
  MACRO(Annotation, AstNodePtr)

/* keyword */
#define KEYWORD_NODE_MEMBERS(MACRO) \
  MACRO(Arg, AstNodePtr) \
  MACRO(Value, AstNodePtr)

/* alias */
#define ALIAS_NODE_MEMBERS(MACRO) \
  MACRO(Name, AstNodePtr) \
  MACRO(Asname, AstNodePtr)

/* withitem */
#define WITH_ITEM_NODE_MEMBERS(MACRO) \
  MACRO(Context_expr, AstNodePtr) \
  MACRO(Optional_var, AstNodePtr)

#define DECORATOR_NODE_MEMBERS(MACRO) \
  MACRO(Id, AstNodePtr) \
  MACRO(Args, Vector<AstNodePtr>)

/* macro iterator */
#define AST_NODE_ITERATOR(MACRO) \
  MACRO(Identifier, IDENTIFIER_NODE_MEMBERS) \
  MACRO(PyModule, PY_MODULE_NODE_MEMBERS) \
  MACRO(Statement, EMPTY_BODY) \
  MACRO(FunctionDefStatement, FUNCTION_DEF_NODE_MEMBERS) \
  MACRO(AsyncFunctionDefStatement, ASYNC_FUNCTION_DEF_NODE_MEMBERS) \
  MACRO(ClassDefStatement, CLASS_DEF_NODE_MEMBERS) \
  MACRO(ReturnStatement, RETURN_NODE_MEMBERS) \
  MACRO(DeleteStatement, DELETE_NODE_MEMBERS) \
  MACRO(AssignStatement, ASSIGN_NODE_MEMBERS) \
  MACRO(AugAssignStatement, AUG_ASSIGN_NODE_MEMBERS) \
  MACRO(ForStatement, FOR_NODE_MEMBERS) \
  MACRO(AsyncForStatement, ASYNC_FOR_NODE_MEMBERS) \
  MACRO(WhileStatement, WHILE_NODE_MEMBERS) \
  MACRO(IfStatement, IF_NODE_MEMBERS) \
  MACRO(WithStatement, WITH_NODE_MEMBERS) \
  MACRO(AsyncWithStatement, ASYNC_WITH_NODE_MEMBERS) \
  MACRO(RaiseStatement, RAISE_NODE_MEMBERS) \
  MACRO(TryStatement, TRY_NODE_MEMBERS) \
  MACRO(AssertStatement, ASSERT_NODE_MEMBERS) \
  MACRO(ImportStatement, IMPORT_NODE_MEMBERS) \
  MACRO(ImportFromStatement, IMPORT_FROM_NODE_MEMBERS) \
  MACRO(GlobalStatement, GLOBAL_NODE_MEMBERS) \
  MACRO(NonlocalStatement, NONLOCAL_NODE_MEMBERS) \
  MACRO(ExprStatement, EXPR_NODE_MEMBERS) \
  MACRO(PassStatement, EMPTY_BODY) \
  MACRO(BreakStatement, EMPTY_BODY) \
  MACRO(ContinueStatement, EMPTY_BODY) \
  MACRO(Expression, EMPTY_BODY) \
  MACRO(BoolOpExpression, BOOL_OP_NODE_MEMBERS) \
  MACRO(BinOpExpression, BIN_OP_NODE_MEMBERS) \
  MACRO(UnaryOpExpression, UNARY_OP_NODE_MEMBERS) \
  MACRO(LambdaExpression, LAMBDA_NODE_MEMBERS) \
  MACRO(IfExpression, IF_EXP_NODE_MEMBERS) \
  MACRO(DictExpression, DICT_NODE_MEMBERS) \
  MACRO(SetExpression, SET_NODE_MEMBERS) \
  MACRO(ListCompExpression, LIST_COMP_NODE_MEMBERS) \
  MACRO(SetCompExpression, SET_COMP_NODE_MEMBERS) \
  MACRO(DictCompExpression, DICT_COMP_NODE_MEMBERS) \
  MACRO(GeneratorExpression, GENERATR_EXPR_NODE_MEMBERS) \
  MACRO(AwaitExpression, AWAIT_NODE_MEMBERS) \
  MACRO(YieldExpression, YIELD_NODE_MEMBERS) \
  MACRO(YieldFromExpression, YIELD_FROM_NODE_MEMBERS) \
  MACRO(CompareExpression, COMPARE_NODE_MEMBERS) \
  MACRO(CallExpression, CALL_NODE_MEMBERS) \
  MACRO(NumExpression, NUM_NODE_MEMBERS) \
  MACRO(StrExpression, STR_NODE_MEMBERS) \
  MACRO(NameConstantExpression, NAME_CONSTANT_NODE_MEMBERS) \
  MACRO(EllipsisExpression, EMPTY_BODY) \
  MACRO(AttributeExpression, ATTRIBUTE_NODE_MEMBERS) \
  MACRO(SubscriptExpression, SUBSCRIPT_NODE_MEMBERS) \
  MACRO(StarredExpression, STARRED_NODE_MEMBERS) \
  MACRO(NameExpression, NAME_NODE_MEMBERS) \
  MACRO(ListExpression, LIST_NODE_MEMBERS) \
  MACRO(TupleExpression, TUPLE_NODE_MEMBERS) \
  MACRO(Slice, SLICE_NODE_MEMBERS) \
  MACRO(ExtSlice, EXT_SLICE_NODE_MEMBERS) \
  MACRO(IndexSlice, INDEX_NODE_MEMBERS) \
  MACRO(Comprehension, COMPREHENSION_NODE_MEMBERS) \
  MACRO(ExceptHandler, EXCEPT_HANDLER_NODE_MEMBERS) \
  MACRO(Arguments, ARGUMENTS_NODE_MEMBERS) \
  MACRO(Arg, ARG_NODE_MEMBERS) \
  MACRO(Keyword, KEYWORD_NODE_MEMBERS) \
  MACRO(Alias, ALIAS_NODE_MEMBERS) \
  MACRO(WithItem, WITH_ITEM_NODE_MEMBERS) \
  MACRO(Decorator, DECORATOR_NODE_MEMBERS)

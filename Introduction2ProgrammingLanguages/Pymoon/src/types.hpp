#pragma once
#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <unordered_map>

#define _TODO_

template <typename Key, typename Value>
using HashTable = std::unordered_map<Key, Value>;

template <typename T>
using Vector = std::vector<T>;

template <typename T>
using VectorPos = typename Vector<T>::iterator;

template <typename T>
using VectorCPos = typename Vector<T>::const_iterator;

using String = std::string;
using Byte = char;

template <typename T>
using SharedPtr = std::shared_ptr<T>;

template <typename T>
using EnableShared = std::enable_shared_from_this<T>;

template <typename T, typename... Args>
inline SharedPtr<T> MakeShared(Args&&... args)
{
  return std::make_shared<T, Args...>(args...);
}

class AstNode;
using AstNodePtr = SharedPtr<AstNode>;

using OutputStream = std::ostream;

/*
 * -----------------------------------------------------------------------------
 * File: stack.hpp
 * -----------------------------------------------------------------------------
 * Author: Mincho Paskaleff
 * -----------------------------------------------------------------------------
 */

#ifndef STACK_HPP
#define STACK_HPP

#include <cassert>

template <typename T>
class Stack {
private:
    struct Node {
        T data;
        Node* next;
    };
    Node* top;
    int length;

    void eraseStack();
    void copy(Node*);
    void copyStack(const Stack&);
public:
    Stack();
    Stack(const Stack&);
    ~Stack();
    Stack& operator=(const Stack&);

    int getLength() const;
    bool empty() const;
    void push(const T&);
    T pop();
    T peek() const;
};

// Private functions
template <typename T>
void Stack<T>::eraseStack() {
    while(!empty()){
        Node* old = top;
        top = top->next;
        delete old;
        length = 0;
    }
}

template <typename T>
void Stack<T>::copy(Node* otherTop) {
    if(otherTop == nullptr)
        return;
    copy(otherTop->next);
    push(otherTop->data);
}

template <typename T>
void Stack<T>::copyStack(const Stack& other) {
    top = nullptr;
    copy(other.top);
}

// Public functions
template <typename T>
Stack<T>::Stack() : top(nullptr), length(0) { }

template <typename T>
Stack<T>::Stack(const Stack& other) : top(nullptr), length(0)  {
    copyStack(other);
}

template <typename T>
Stack<T>::~Stack() {
  eraseStack();
}

template <typename T>
Stack<T>& Stack<T>::operator=(const Stack& other) {
    if(this != &other) {
        eraseStack();
        copyStack(other);
    }
    return *this;
}

template <typename T>
int Stack<T>::getLength() const {
    return length;
}

template <typename T>
bool Stack<T>::empty() const {
    return top == nullptr;
}

template <typename T>
void Stack<T>::push(const T& data) {
    Node* newTop = new Node;
    newTop->data = data;
    newTop->next = top;
    top = newTop;
    ++length;
}

template <typename T>
T Stack<T>::pop() {
    assert(!empty());
    Node* popped = top;
    top = top->next;
    T data = popped->data;
    --length;

    delete popped;
    return data;
}

template <typename T>
T Stack<T>::peek() const {
    assert(!empty());
    return top->data;
}

#endif

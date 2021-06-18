#ifndef STACK_CPP
#define STACK_CPP

#include <iostream>
#include "Stack.h"

template <typename T>
Stack<T>::Stack() : head(nullptr), size(0) {}

template <typename T>
Stack<T>::Stack(const Stack& other) : head(nullptr), size(0) {
    copy(other.head);
}

template <typename T>
Stack<T>& Stack<T>::operator=(const Stack & other) {
    if(this != &other) {
        size = 0;
        copy(other.head);
    }
    return *this;
}

template <typename T>
Stack<T>::~Stack() {
    while(head != nullptr) {
        pop();
    }
}

template <typename T>
void Stack<T>::copy(const node* other) {
    if(other == nullptr)
        return;
    copy(other->next);
    push(other->data);
}

template <typename T>
void Stack<T>::push(const T& data) {
    node* newHead = new node(data, head);
    head = newHead;
    ++size;
}

template <typename T>
T* Stack<T>::pop() {
    if(empty())
        return nullptr;
    node* tmp = head;
    head = head->next;
    T* returnData = new T(tmp->data);
    delete tmp;
    --size;
    return returnData;
}

template <typename T>
T* Stack<T>::peek() const {
    if(empty())
        return nullptr;

    return new T(head->data);
}

template <typename T>
void Stack<T>::printRec(const node* head) const {
    if(!head->next) {
        std::cout << head->data << ' ';
        return;
    }
    printRec(head->next);
    std::cout << head->data << ' ';
}

template <typename T>
void Stack<T>::print() const {
    if(empty()) {
        std::cout << "Stack is empty!";
        return;
    }
    printRec(head);
}

#endif

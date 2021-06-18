//
// Created by ellie on 17.10.17.
//
#ifndef _LIST_CPP
#define _LIST_CPP

#include <iostream>
#include <iomanip>
#include "List.h"

template <typename T>
List<T>::List() : head(nullptr), tail(nullptr), end_el(), size(0) {}

template <typename T>
List<T>::List(const List& other) : head(nullptr), tail(nullptr), end_el(), size(0) {
    *this = other;
}

template <typename T>
List<T>& List<T>::operator=(const List& rhs) {
    if(this != &rhs) {
        emptyList();
        for(iterator it = rhs.begin(); it != rhs.end(); ++it) {
            push_back(*it);
        }
    }
    return *this;
}

template <typename T>
void List<T>::emptyList() {
    while(!empty())
        pop_front();
}

template <typename T>
List<T>::~List() {
    emptyList();
}

template <typename T>
void List<T>::push_back(const T& data) {
    if(empty()) {
        head = new node(data, nullptr, end_el);
        tail = head;
    } else {
        node* tmp = new node(data, tail, end_el);
        tail->next = tmp;
        tail = tmp;
    }
    ++size;
}

template <typename T>
bool List<T>::pop_back() {
    if(empty())
        return false;
    node* tmp = tail;
    tail = tail->prev;
    if(tail) {
        tail->next = end_el;
    }
    delete tmp;
    --size;
    if(empty()) {
        head = tail = end_el;
    }
    return true;
}

template <typename T>
void List<T>::push_front(const T& data) {
    if(empty()){
        head = new node(data, nullptr, end_el);
        tail = head;
    } else{
        node* tmp = new node(data, nullptr, head);
        head->prev = tmp;
        head = tmp;
    }
    ++size;
}

template <typename T>
bool List<T>::pop_front() {
    if(empty())
        return false;
    node* tmp = head;
    head = head->next;
    if(head != end_el) {
        head->prev = nullptr;
    } else {
        head = tail = end_el;
    }
    delete tmp;
    --size;
    return true;
}

template <typename T>
bool List<T>::removeAt(iterator pos) {
  if(!pos.valid() || pos == end()) return false;
  if(size == 1) {
    delete head;
    delete tail;
    head = tail = nullptr;
    --size;
    return true;
  }

  node* curr = pos.ptr;
  node* prv = curr->prev;
  node* nxt = curr->next;
  prv->next = nxt;
  if(nxt && nxt != end_el) {
    nxt->prev = prv;
  }
  delete curr;
  --size;
  return true;
}

template <typename T>
bool List<T>::insertAt(const T& data, iterator pos) {
  if(!pos.valid() || pos == end()) return false;
  node* curr = pos.ptr;
  node* nxt = curr->next;
  node* newNode = new node(data, curr, nxt);
  curr->next = newNode;
  if(nxt && nxt != end_el) nxt->prev = newNode;
  if(newNode->next == end_el) tail = newNode;
  ++size;
  return true;
}

template <typename T>
void List<T>::reverse() {
    node* tmp = head;
    head = tail;
    tail = tmp;

}

template <typename T>
void List<T>::print() const {
    node* iter = head;
    while(iter) {
        std::cout << iter->data << " ";
        iter = iter->next;
    }
    std::cout << std::endl;
}

#endif // _LIST_CPP

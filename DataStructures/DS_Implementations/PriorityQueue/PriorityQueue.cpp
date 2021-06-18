//
// Created by ellie on 23.10.17.
//
#ifndef _PRIORITY_QUEUE_CPP
#define _PRIORITY_QUEUE_CPP

#include <iostream>
#include "PriorityQueue.h"

template <class T, class S, class Compare>
PriorityQueue<T,S,Compare>::PriorityQueue() : head(nullptr), size(0) { }

template <class T, class S, class Compare>
PriorityQueue<T,S,Compare>::PriorityQueue(const PriorityQueue& other)
        : head(nullptr), size(0)
{
    copy(other.head);
}

template <class T, class S, class Compare>
PriorityQueue<T,S,Compare>& PriorityQueue<T,S,Compare>::operator=(const PriorityQueue& rhs) {
    if(this != &rhs) {
        copy(rhs.head);
    }
    return *this;
}

template <class T, class S, class Compare>
PriorityQueue<T,S,Compare>::~PriorityQueue() {
    erase();
}

template <class T, class S, class Compare>
void PriorityQueue<T,S,Compare>::copy(node* otherHead) {
    erase();

    node* tmp = new node(otherHead->data, otherHead->priority);
    head = tmp;
    otherHead = otherHead->next;
    while(otherHead) {
        tmp->next = new node(otherHead->data, otherHead->priority);
        tmp = tmp->next;
        otherHead = otherHead->next;
    }
}

template <class T, class S, class Compare>
void PriorityQueue<T,S,Compare>::erase() {
    while(head)
        pop();
}

template <class T, class S, class Compare>
void PriorityQueue<T,S,Compare>::print() const {
    std::cout << "Empty: " << (empty() ? "true" : "false") << std::endl;
    std::cout << "Size: " << getSize() << std::endl;
    std::cout << "Top: " << top() << std::endl;
    node* tmp = head;
    while(tmp) {
        std::cout << tmp->data << " ";
        tmp = tmp->next;
    }
    std::cout << std::endl;
}

template <class T, class S, class Compare>
void PriorityQueue<T,S,Compare>::pop() {
    node* tmp = head;
    head = head->next;
    --size;
    delete tmp;
}

template <class T, class S, class Compare>
void PriorityQueue<T,S,Compare>::push(T data, S priority) {
    node* tmp = head;
    node* prev = nullptr;

    while(tmp && compareFunc(priority, tmp->priority)) {
        prev = tmp;
        tmp = tmp->next;
    }
    node* newNode = new node(data, priority, tmp);
    if(!prev) {
        head = newNode;
    } else {
        prev->next = newNode;
    }
    ++size;
}

#endif //_PRIORITY_QUEUE_CPP

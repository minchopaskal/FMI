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
void List<T>::sort() {
    head = mergeSort(head);
}

template <typename T>
typename List<T>::node* List<T>::mergeSort(node* lst) {
    if(lst->next == nullptr)
        return lst;
    node* slow;
    node* fast;

    slow = fast = lst;

    // Slicing list in two halves.
    while(fast) {
        slow = slow->next;
        fast = fast->next;
        if(fast)
            fast = fast->next;
    }

    node* sndHalf = slow;
    (slow->prev)->next = nullptr;
    slow->prev = nullptr;

    // Recursively sort each one of the halves.
    node* lst1 = mergeSort(lst);
    node* lst2 = mergeSort(sndHalf);
    
    // Merging the already sorted halves.
    return mergeLists(lst1, lst2);
}

template <typename T>
typename List<T>::node* List<T>::mergeLists(node* lst1, node* lst2) {
    // Iterators
    node* h1 = lst1;
    node* h2 = lst2;

    node* newHead;
    node* curr = h1->data < h2->data ? h1 : h2;

    newHead = curr;

    // After getting the smallest element
    // advance the coresponding iterator forward.
    if(h1->data < h2->data) {
        h1 = h1->next;
        if(h1)
            h1->prev = nullptr;
    } else {
        h2 = h2->next;
        if(h2)
            h2->prev = nullptr;
    }

    curr->next = nullptr;

    // Do the above thing in a loop
    while(h1 && h2) {
        if(h1->data <= h2->data) {
            curr->next = h1;
            h1 = h1->next;
        } else {
            curr->next = h2;
            h2 = h2->next;
        }
        curr->next->prev = curr;
        curr = curr->next;
    }

    // At the end one of the lists is null (or both)
    // We concatenate the full list at the end of the already
    // made one.
    if(!h1) {
        curr->next = h2;
        if(h2)
            h2->prev = curr;
    }
    if(!h2) {
        curr->next = h1;
        if(h1)
            h1->prev = curr;
    }

    return newHead;
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

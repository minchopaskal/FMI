//
// Created by ellie on 17.10.17.
//

#ifndef _LIST_H
#define _LIST_H

#include <cstddef>
#include <iostream>

template <typename T>
class List {
private:
    struct node {
        T data;
        node* prev;
        node* next;
        node(const T& data = T(), node* prev = nullptr, node* next = nullptr) {
            this->data = data;
            this->prev = prev;
            this->next = next;
        }
    };
    
    class Iterator {
    public:
      node* ptr;
    
      using I = Iterator;
      explicit Iterator(node* ptr = nullptr) : ptr(ptr)  {}

      I next() const {
        return I(ptr->next);
      }

      I prev() const {
	return I(ptr->prev);
      }

      T& get() const {
        return ptr->data;
      }

      T const& cget() const {
        return ptr->data;
      }

      bool valid() const {
        return ptr != nullptr;
      }

      bool operator==(I const& it) const {
        return ptr == it.ptr;
      }

      bool operator!=(I const& it) const {
        return !(*this == it);
      }

      T& operator*() {
        return get();
      }

      I operator++() {
        return *this = next();
      }

      I operator++(int) {
        I prev = *this;
        *this = ++(*this);
        return prev;
      }

      I operator--() {
	return *this = prev();
      }

      I operator--(int) {
	I curr = *this;
	*this = --(*this);
	return curr;
      }
      
      operator bool() const {
        return valid();
      }
    };

    node* head;
    node* tail;
    node* end_el;
    std::size_t size;

    void emptyList();

    node* mergeSort(node*);
    node* mergeLists(node*, node*);

public:
    using iterator = Iterator;
    
    // Big four
    List();
    List(const List&);
    List&operator=(const List&);
    ~List();

    // Itarators
    iterator begin() const { return iterator(head); }
    iterator last() const { return iterator(tail); }
    iterator end() const { return iterator(end_el); }

    // Capacity
    bool empty() const { return size == 0; }
    std::size_t getSize() const { return size; }

    // Element access
    T front() const { return head->data; }
    T back() const { return tail->data; }

    // Modifiers
    void push_back(const T&);
    bool pop_back();
    void push_front(const T&);
    bool pop_front();
    bool removeAt(iterator pos);
    bool insertAt(const T&, iterator pos);
    
    // Operations
    template <class Compare>
    void sort() {
      head = mergeSort<Compare>(head);
    }

    template <class Compare>
    node* mergeSort(node* lst) {
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
      node* lst1 = mergeSort<Compare>(lst);
      node* lst2 = mergeSort<Compare>(sndHalf);
    
      // Merging the already sorted halves.
      return mergeLists<Compare>(lst1, lst2);
    }

    template <class Compare>
    node* mergeLists(node* lst1, node* lst2) {
      // Comparator
      Compare comp;

      // Iterators
      node* h1 = lst1;
      node* h2 = lst2;

      node* newHead;
      node* curr = comp(h1->data, h2->data) ? h1 : h2;
      newHead = curr;

      // After getting the smallest element
      // advance the coresponding iterator forward.
      if(comp(h1->data, h2->data)) {
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
	if(comp(h1->data, h2->data)) {
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
    void reverse();
    void print() const;
    
};

#include "List.cpp"


#endif //_LIST_H

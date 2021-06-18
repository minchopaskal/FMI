//
// Created by ellie on 09.10.17.
//

#ifndef SDP_SEMINAR_STACK_H
#define SDP_SEMINAR_STACK_H

template <typename T>
class Stack {
private:
    struct node {
        T data;
        node* next;
        node(const T& data, node* next) {
            this->data = data;
            this->next = next;
        }
    };

    node* head;
    int size;

    void copy(const node*);
    void printRec(const node*) const;
public:
    Stack();
    Stack(const Stack&);
    Stack&operator=(const Stack&);
    ~Stack();

    void push(const T&);
    T* pop();
    T* peek() const;

    bool empty() const { return size == 0; }
    int getSize() const { return size; }
    void print() const;
};

#include "Stack.cpp"

#endif //SDP_SEMINAR_STACK_H

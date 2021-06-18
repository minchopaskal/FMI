//
// Created by ellie on 28.10.17.
//

#ifndef _BTREE_H
#define _BTREE_H

#include <sstream>
#include <string>
#include <vector>

using OP = double (*)(double, double);

template <class T = int, class Compare = std::less<int>>
class BTree {
private:
    struct node {
        T data;
        node* left;
        node* right;

        node(T data = T(), node* left = nullptr, node* right = nullptr) :
                data(data), left(left), right(right) {}
    };

    node* root;

private:
    node* copy(const node*);
    void erase(node*&);
    void insertOrdered(const T&, node*&);

    int count(const node*) const;
    //int countEvens(const node*) const;
    int searchCount(bool (*)(const T&), const node*) const;
    int height(const node*) const;
    int countLeaves(const node*) const;
    const node* maxLeaf(const node*) const;
    const node* getElement(const char*, const node*) const;
    const node* getNodeBySymbol(char, const node*) const;
    std::vector<T> listLeaves(const node*) const;
    std::string findTrace(const node*, const T&, std::string) const;
    void prettyPrint(const node*, int, int&) const;
    double calculateExpressionTree(const node*) const;
    OP getOp(char) const;
    T& get(node*, int) const;

    void printOrdered(const node*, std::ostream& os = std::cout) const;
    void printScheme(const node*, std::ostream& os = std::cout) const;

public:
    BTree();
    BTree(const T& data,
          const BTree<T, Compare>& lt = BTree(),
          const BTree<T,Compare>& rt = BTree());
    BTree(const BTree&);
    BTree&operator=(BTree);
    ~BTree();

    bool empty() const { return root == nullptr; }
    void insertOrdered(const T&);

    // Redundant function after implementing operator<<
    //void print() const { print(root); };

    void printScheme() const;

    /*
     * Exercises from
     * https://github.com/stranxter/lecture-notes/blob/master/homeworks/03_sdp/03_trees/trees.pdf
     */
    int count() const;
    int countEvens() const;
    int searchCount(bool (*)(const T&)) const;
    int height() const;
    int countLeaves() const;
    const T& maxLeaf() const;
    const T& getElement(const char*) const;

    // Exercises from second homework
    std::vector<T> listLeaves() const;
    std::string findTrace(const T&) const;
    void prettyPrint() const;
    static BTree<char>* parseExpression(std::string);
    // ? ? ?
    // void parseExpression(std::string);
    double calculateExpressionTree() const;
    const T&operator[](int) const;

    // Friend functions:

    // Implementing the copy-and-swap idiom
    template <class S, class Comp>
    friend void swap(BTree<S, Comp>&, BTree<S, Comp>&);
    // Output
    template <class S, class Comp>
    friend std::ostream&operator<<(std::ostream&, const BTree<S, Comp>&);

};

template <class T, class Compare>
std::ostream& operator<<(std::ostream&, const BTree<T, Compare>&);






#include "BTree.cpp"

#endif //_BTREE_H

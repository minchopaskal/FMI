//
// Created by ellie on 28.10.17.
//

#ifndef _BTREE_CPP
#define _BTREE_CPP

#include <algorithm>
#include <cmath>
#include <iostream>
#include <iomanip>

#include "BTree.h"


/*
 * Friend functions
 */
template <class T, class Compare>
void swap(BTree<T, Compare>& tree, BTree<T, Compare>& other) {
    using std::swap;
    std::swap(tree.root, other.root);
}

template <class T, class Compare>
std::ostream& operator<<(std::ostream& os, const BTree<T,Compare>& tree) {
    os << "Ordered: ";
    tree.printOrdered(tree.root, os);
    os << std::endl << "Scheme: ";
    tree.printScheme(tree.root, os);
    os << std::endl;
    return os;
};

/*
 * Big Four
 */
template <class T, class Compare>
BTree<T, Compare>::BTree() : root(nullptr) { }

template <class T, class Compare>
BTree<T, Compare>::BTree(const T& data,
                         const BTree<T, Compare>& lt,
                         const BTree<T, Compare>& rt) {

    root = new BTree<T,Compare>::node(data, copy(lt.root), copy(rt.root));
}

template <class T, class Compare>
BTree<T, Compare>::BTree(const BTree& other) : root(nullptr) {
    root = copy(other.root);
}

template <class T, class Compare>
BTree<T, Compare>& BTree<T, Compare>::operator=(BTree rhs) {
    swap(*this, rhs);
    return *this;
}

template <class T, class Compare>
BTree<T, Compare>::~BTree() {
    erase(root);
}


/*
 * Private help functions
 */
template <class T, class Compare>
typename BTree<T, Compare>::node*
BTree<T, Compare>::copy(const node* root) {
    if(root == nullptr)
        return nullptr;

    return new node(root->data,
                    copy(root->left),
                    copy(root->right));
}

template <class T, class Compare>
void BTree<T, Compare>::erase(node*& root) {
    if(!root)
        return;

    erase(root->right);
    erase(root->left);
    delete root;
}

template <class T, class Compare>
void BTree<T, Compare>::insertOrdered(const T& data) {
    insertOrdered(data, root);
}


/*
 * Functionality
 */
template <class T, class Compare>
void BTree<T, Compare>::insertOrdered(const T& data, node*& root) {
    if(empty()) {
        this->root = new node(data, nullptr, nullptr);
        return;
    }
    if(Compare()(data, root->data)) {
        if(!root->left) {
            root->left = new node(data, nullptr, nullptr);
            return;
        }
        insertOrdered(data, root->left);
    } else {
        if(!root->right) {
            root->right = new node(data, nullptr, nullptr);
            return;
        }
        insertOrdered(data, root->right);
    }
}

template <class T, class Compare>
void BTree<T, Compare>::printOrdered(const node* root, std::ostream& os) const {
    if(root == nullptr) {
        return;
    }

    os << "(";
    printOrdered(root->left, os);
    os << ") ";
    os << root->data;
    os << " (";
    printOrdered(root->right, os);
    os << ")";
}

template <class T, class Compare>
void BTree<T, Compare>::printScheme(const node* root, std::ostream& os) const {
    if(root == nullptr) {
        os << "()";
        return;
    }

    os << "(";

    os << root->data << " ";
    printScheme(root->left, os);
    os << " ";
    printScheme(root->right, os);

    os << ")";
}

template <class T, class Compare>
void BTree<T, Compare>::printScheme() const {
    printScheme(root, std::cout);
}

/*
 * Functions from exercise
 */
template <class T, class Compare>
int BTree<T,Compare>::count(const node* root) const {
    if(root == nullptr) {
        return 0;
    }
    return 1 + count(root->left) + count(root->right);
}

template <class T, class Compare>
int BTree<T, Compare>::count() const {
    return searchCount([](const T& data) -> bool {
                           return true;
                       }, root);
};


template <class T, class Compare>
int BTree<T, Compare>::countEvens() const {
    return searchCount([](const T& data) -> bool {
                           return data % 2 == 0;
                       }, root);
};


template <class T, class Compare>
int BTree<T, Compare>::searchCount(bool (*pred)(const T &)) const {
    return searchCount(pred, root);
}

template <class T, class Compare>
int BTree<T, Compare>::searchCount(bool (*pred)(const T &), const node* root) const {
    if(root == nullptr) {
        return 0;
    }

    return (pred(root->data) ? 1 : 0) +
           searchCount(pred, root->right) +
           searchCount(pred, root->left);
}

template <class T, class Compare>
int BTree<T, Compare>::height() const {
    return height(root);
}

template <class T, class Compare>
int BTree<T, Compare>::height(const node* root) const {
    if(root == nullptr) {
        return 0;
    }

    return 1 + std::max(height(root->right), height(root->left));
}

template <class T, class Compare>
int BTree<T, Compare>::countLeaves() const {
    return countLeaves(root);
}

template <class T, class Compare>
int BTree<T, Compare>::countLeaves(const node* curr) const {
    if (curr == nullptr) {
        return 0;
    }

    if (curr->left == nullptr && curr->right == nullptr) {
        return 1;
    }

    return countLeaves(curr->left) + countLeaves(curr->right);
}

template <class T, class Compare>
const T& BTree<T, Compare>::maxLeaf() const {
    const node* leaf = maxLeaf(root);
    if(leaf) {
        return leaf->data;
    }
    return *(new T());
};

template <class T, class Compare>
const typename BTree<T,Compare>::node*
BTree<T, Compare>::maxLeaf(const node* curr) const {
    if(curr == nullptr)
        return nullptr;

    if(curr->left == nullptr && curr->right == nullptr) {
        return curr;
    }

    const node* lData = maxLeaf(curr->left);
    const node* rData = maxLeaf(curr->right);

    if(lData == nullptr)
        return rData;
    if(rData == nullptr)
        return lData;

    return Compare()(lData->data, rData->data) ? rData : lData;
};

template <class T, class Compare>
const T& BTree<T, Compare>::getElement(const char* s) const {
    const node* element = getElement(s, root);
    if(element != nullptr) {
        return element->data;
    }
    return *(new T());
};

template <class T, class Compare>
const typename BTree<T,Compare>::node*
BTree<T, Compare>::getElement(const char* s, const node* curr) const {
    if(curr == nullptr) {
        return nullptr;
    }
    if(*s == '\0') {
        return curr;
    }

    return getElement(++s, getNodeBySymbol(*s, curr));
};

template <class T, class Compare>
const typename BTree<T,Compare>::node*
BTree<T, Compare>::getNodeBySymbol(char symbol, const node* root) const {
    switch(symbol) {
        case 'L':
            return root->left;
        case 'R':
            return root->right;
        default:
            return nullptr;
    }
}

template <class T, class Compare>
std::vector<T> BTree<T, Compare>::listLeaves() const {
    return listLeaves(root);
}

template <class T, class Compare>
std::vector<T> BTree<T, Compare>::listLeaves(const node* root) const {
    if(root == nullptr) {
        std::vector<T> empty;
        return empty;
    }

    if(root->right == nullptr && root->left == nullptr) {
        std::vector<T> one;
        one.push_back(root->data);
        return one;
    }

    std::vector<T> left = listLeaves(root->left);
    std::vector<T> right = listLeaves(root->right);

    left.insert(left.end(), right.begin(), right.end());
    return left;
}

template <class T, class Compare>
std::string BTree<T, Compare>::findTrace(const T& x) const {
    return findTrace(root, x, "");
}

template <class T, class Compare>
std::string BTree<T, Compare>::findTrace(const node* root, const T& x, std::string curr) const {
    if(root == nullptr) {
        return "error";
    }

    if(root->data == x) {
        return curr;
    }

    std::string leftStr = curr + 'L';
    std::string left = findTrace(root->left, x, leftStr);
    if(left != "error") {
        return left;
    }

    std::string rgtStr = curr + 'R';
    std::string right = findTrace(root->right, x, rgtStr);
    if(right != "error") {
        return right;
    }

    return "error";
}

template <class T, class Compare>
void BTree<T, Compare>::prettyPrint() const {
    int line = 0;
    prettyPrint(root, 0, line);
}

template <class T, class Compare>
void BTree<T, Compare>::prettyPrint(const node* root, int depth, int& line) const {
    if(root == nullptr) {
        return;
    }

    prettyPrint(root->right, depth + 1, line); std::cout << std::endl;
    ++line;
    std::cout << line << ": ";
    std::cout << std::setw(depth * 4);
    std::cout << root->data;
    prettyPrint(root->left, depth + 1, line);
}

// TODO Specialize for BTree<char> only!!!
template <class T, class Compare>
BTree<char>* BTree<T, Compare>::parseExpression(std::string e) {
    std::istringstream expr(e);
    char curr;
    // Read '(' or digit
    expr >> curr;

    //Read next
    expr >> curr;

    BTree<char>* left;
    std::string s;

    if(isdigit(curr)) {
        left = new BTree<char>(curr);
    }
        // Else curr == '('
    else {
        expr.putback('('); // <- We've lost the '(' so we add it again
        getline(expr, s);
        left = parseExpression(s);
        //Read ')'
        expr >> curr;
    }

    // Read operation (root)
    expr >> curr;
    char root = curr;

    // Read either digit or new expr
    expr >> curr;
    BTree<char>* right;

    if(isdigit(curr)) {
        right = new BTree<char>(curr);
    } else if(curr == '(') {
        expr.putback('('); // <- We've lost the '(' so we add it again
        getline(expr, s);
        right = parseExpression(s);
        // Read ')'
        expr >> curr;
    }

    return new BTree<char>(root, *left, *right);
}

// TODO Specialize for BTree<char> only!!!
template <class T, class Compare>
double BTree<T, Compare>::calculateExpressionTree() const {
    return calculateExpressionTree(root);
}

template <class T, class Compare>
double BTree<T, Compare>::calculateExpressionTree(const node* root) const {
    if(isdigit(root->data)) {
        return root->data - '0';
    }

    return getOp(root->data)(calculateExpressionTree(root->left),
                             calculateExpressionTree(root->right));
}

template <class T, class Compare>
OP BTree<T, Compare>::getOp(char c) const {
    switch(c) {
        case '+':
            return [](double A , double B) {
                return A + B;
            };
        case '-':
            return [](double A, double B) {
                return A - B;
            };
        case '*':
            return [](double A, double B) {
                return A * B;
            };
        default:
            return [](double A, double B){
                return A / B;
            };
    }
}

template <class T, class Compare>
const T& BTree<T, Compare>::operator[](int i) const {
    return get(root, i);
}

template <class T, class Compare>
T& BTree<T, Compare>::get(node* root, int i) const {
    if(i == 0) {
        return root->data;
    }
    int countLeft = count(root->left);
    int countRight = count(root->right);

    if(countLeft >= i) {
        return get(root->left, i-1);
    }
    if(countRight >= i - countLeft) {
        return get(root->right, i - countLeft - 1);
    }
    return *(new T());
}

#endif //_BTREE_CPP

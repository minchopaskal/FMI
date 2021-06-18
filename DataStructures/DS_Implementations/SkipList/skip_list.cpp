#include <cmath>
#include <ctime>
#include <functional>
#include <memory>

template <class T, class Compare = std::less<T>>
class SkipList {
private:
    using uint = unsigned int;

    struct node {
        T data;
        uint ptrCnt;
        node** ptrs;

        node(uint ptrCnt, const T& data = T()) : ptrCnt(ptrCnt), data(data) {
            ptrs = new node*[ptrCnt];
            for(int i = 0; i < ptrCnt; ++i) {
                ptrs[i] = nullptr;
            }
        }
    };

private:
    double probability;
    uint maxHeight;
    node* head;
    Compare cmp;

private:
    void erase(node* root) {
        while(root != nullptr) {
            node* tmp = root;
            root = root->ptrs[0];
            delete tmp;
        }
    }

    uint coinFlip() const {
        uint height = 1;

        while ((((double) std::rand() / RAND_MAX)) < probability &&
               std::abs(height) < maxHeight) {
            height += 1;
        }
        return (uint) std::abs(height);
    }

    bool findNode(const T& data, node**& backTrack) const {
        node* curr = nullptr;
        node* iterator = head;

        int i = maxHeight;
        backTrack = new node*[i--];
        for(int j = i; j >= 0; --j) {
            backTrack[j] = head;
        }

        while(i >= 0) {
            curr = iterator->ptrs[i];
            bool what = curr == nullptr;

            while(curr != nullptr && cmp(curr->data, data)) {
                iterator = curr;
                curr = iterator->ptrs[i];
            }
            backTrack[i] = iterator;
            --i;
        }
        if(curr == nullptr) {
            return false;
        }

        return curr->data == data;
    }


public:
    SkipList(std::size_t expectedSize = 1024, double probability = 0.5)
            : head(nullptr), probability(probability) {
        maxHeight = (uint) floor(log2(expectedSize));
        head = new node(maxHeight);
    }

    ~SkipList() {
        erase(head);
    }

    bool insert(const T& data) {
        node** backTrack;
        bool inList = findNode(data, backTrack);

        if(inList) {
            return false;
        }

        int height = coinFlip();
        node* n = new node(height, data);

        node* curr;
        --height;
        for(; height >= 0; --height) {
            curr = backTrack[height]->ptrs[height];
            backTrack[height]->ptrs[height] = n;
            n->ptrs[height] = curr;
        }
        return true;
    }

    bool del(const T& data) {
        node** backTrack;
        if(!findNode(data, backTrack)) {
            return false;
        }

        node* n = backTrack[0]->ptrs[0];
        int height = n->ptrCnt - 1;
        for(; height >= 0; --height) {
            backTrack[height]->ptrs[height] = n->ptrs[height];
        }
        delete n;
        return true;
    }

    bool find(const T& data) {
        node** history;
        return findNode(data, history);
    }

};

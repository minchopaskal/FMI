template <class T>
class BinTree {
private:
    struct node {
        T data;
        node* left;
        node* right;

        node(const T& data, node* left = nullptr, node* right = nullptr) :
                data(data), left(left), right(right) {}
    };

    node* root;

    node* copy(node* toCopy) {
        if(toCopy == nullptr) {
            return nullptr;
        }

        return new node(toCopy->data,
                        copy(toCopy->left),
                        copy(toCopy->right));
    }

    void erase(node* root) {
        if(root == nullptr) {
            return;
        }
        erase(root->left);
        erase(root->right);
        delete root;
    }

    bool insertNode(const T &data, node *&root) {
        if(root == nullptr) {
            root = new node(data);
            return true;
        }

        if(data == root->data) {
            return false;
        }

        if(data < root->data) {
            insertNode(data, root->left);
        } else {
            insertNode(data, root->right);
        }
        return true;
    }

    bool findNode(const T &data, node *root) const {
        if(root == nullptr) {
            return false;
        }

        if(data == root->data) {
            return true;
        }

        if(data < root->data) {
            return findNode(data, root->left);
        } else {
            return findNode(data, root->right);
        }
    }

    bool deleteNode(const T& data, node*& root) {
        if(root == nullptr) {
            return false;
        }

        if(data == root->data) {
            if(root->left == nullptr) {
                node* tmp = root->right;
                delete root;
                root = tmp;
            } else if (root->right == nullptr) {
                node* tmp = root->left;
                delete root;
                root = tmp;
            } else {
                const T& minimalMax = findMin(root->right);
                root->data = minimalMax;
                deleteNode(minimalMax, root->right);
            }
            return true;
        } else if(data < root->data) {
            return deleteNode(data, root->left);
        } else {
            return deleteNode(data, root->right);
        }
    }

    const T& findMin(node* root) {
        if(root->left == nullptr) {
            return root->data;
        }
        return findMin(root->left);
    }

public:
    BinTree() : root(nullptr) {}

    BinTree(const BinTree& other) : root(nullptr) {
        root = copy(other.root);
    }

    BinTree& operator=(const BinTree& rhs) {
        if(this != &rhs) {
            erase(root);
            root = copy(rhs.root);
        }
        return *this;
    }

    ~BinTree() {
        erase(root);
    }

    bool insert(const T& data) {
        return insertNode(data, root);
    }

    bool find(const T& data) {
        return findNode(data, root);
    }

    bool del(const T& data) {
        return deleteNode(data, root);
    }
};

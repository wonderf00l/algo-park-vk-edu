#include <iostream>
#include <algorithm>
#include <utility>

template <class T>
struct DefaultComparator {
    int operator()(const T& l, const T& r) const {
        if (l < r) return -1;
        if (l > r) return 1;
        return 0;
    }
};

template <class Key, class Value, class Comparator = DefaultComparator<Key> >
class AVLTree {
    struct Node {
        Key key;
        Value value;

        uint8_t height;

        Node* left;
        Node* right;

        Node(const Key& key, const Value& value) : key(key), value(value), height(1), left(nullptr), right(nullptr) {}
    };

   public:
    AVLTree(Comparator comp = Comparator()) : root(nullptr), tree_size(0), comp(comp) {}

    AVLTree(const AVLTree&) = delete;
    AVLTree(AVLTree&&) = delete;
    AVLTree& operator=(const AVLTree&) = delete;
    AVLTree& operator=(AVLTree&&) = delete;

    ~AVLTree() = default;

    Value* find(const Key& key) { return _find(root, key); }
    const Value* find(const Key& key) const { return find(key); }

    void insert(const Key& key, const Value& value) { root = _insert(root, key, value); }

    void erase(const Key& key) { root = _erase(root, key); }

   private:
    Value* _find(Node* node, const Key& key) {
        if (!node) {
            return nullptr;
        }
        int comp_res = comp(key, node->key);
        if (comp_res == -1) {
            return _find(node->left, key);
        } else if (comp_res == 1) {
            return _find(node->right, key);
        }
        return node;
    }

    Node* _insert(Node* node, const Key& key, const Value& value) {
        if (!node) {
            tree_size++;
            return new Node(key, value);
        }

        int comp_res = comp(key, node->key);
        if (comp_res == -1) {
            node->left = _insert(node->left, key, value);
        } else {
            node->right = _insert(node->right, key, value);
        }
        return balance(node);
    }

    Node* _erase(Node* node, const Key& key) {
        if (!node) {
            return nullptr;
        }

        int comp_res = comp(key, node->key);
        if (comp_res == -1) {
            node->left = _erase(node->left, key);
        } else if (comp_res == 1) {
            node->right = _erase(node->right, key);
        } else {
            Node* left = node->left;
            Node* right = node->right;

            delete node;

            if (!right) {
                return left;
            }

            std::pair<Node*, Node*> nodes = find_and_remove_min(right);

            Node* min_node = nodes.first;
            min_node->right = nodes.second;
            min_node->left = left;

            return balance(min_node);
        }
        return balance(node);
    }

    std::pair<Node*, Node*> find_and_remove_min(Node* node) {
        if (!node->left) {
            return std::make_pair(node, node->right);
        }
        return find_and_remove_min(node->left);
    }

    Node* find_min(Node* node) {
        if (!node->left) {
            return node;
        }
        return find_min(node->left);
    }

    Node* remove_min(Node* node) {
        if (!node->left) {
            return node->right;
        }
        node->left = remove_min(node->left);
        return balance(node);
    }

    uint8_t height(Node* node) const { return node ? node->height : 0; }

    void fix_height(Node* node) { node->height = std::max(height(node->left), height(node->right)) + 1; }

    int bfactor(Node* node) const { return height(node->right) - height(node->left); }

    Node* rotate_left(Node* node) {
        Node* tmp = node;
        node = tmp->right;
        tmp->right = node->left;
        node->left = tmp;
        fix_height(tmp);
        fix_height(node);
    }
    Node* rotate_right(Node* node) {
        Node* tmp = node;
        node = tmp->left;
        tmp->left = node->right;
        node->right = tmp;
        fix_height(tmp);
        fix_height(node;)
    }

    Node* balance(Node* node) {
        fix_height(node);
        int bf = bfactor(node);

        if (bf == 2) {
            if (bfactor(node->right) < 0) {
                node->right = rotate_right(node->right);
            }
            return rotate_left(node);
        } else if (bf == -2) {
            if (bfactor(node->left) > 0) {
                node->left = rotate_left(node->left);
            }
            return rotate_right(node);
        }
        return node;
    }

    Node* root;
    size_t tree_size;
    Comparator comp;
};

#include <iostream>
#include <queue>
#include <sstream>
#include <string>
#include <vector>

struct Node {
    int data_;
    Node* left_;
    Node* right_;

    explicit Node(int data) : data_(data), left_(nullptr), right_(nullptr){};
};

class BTree {
   public:
    BTree() : root(nullptr){};

    void add(Node*& root, int item) {
        if (!root) {
            root = new Node(item);
            return;
        }

        if (root->data_ < item) {
            add(root->right_, item);
        } else {
            add(root->left_, item);
        }
    }

    Node*& get_root() { return root; }

    bool is_equal(Node* root) {
        if (!root) {
            return true;
        }

        if (root->left_) {
            if (root->left_->data_ != root->data_) {
                return false;
            }
        }
        if (root->right_) {
            if (root->right_->data_ != root->data_) {
                return false;
            }
        }

        is_equal(root->left_);
        is_equal(root->right_);
        return true;
    }

    std::vector<int> get_items() {
        std::vector<int> items;
        if (!root) {
            return items;
        }
        std::queue<Node*> q;
        while (!q.empty()) {
            Node* node = q.front();
            items.push_back(node->data_);
            q.pop();
            if (node->left_) {
                q.push(node->left_);
            }
            if (node->right_) {
                q.push(node->right_);
            }
        }
        return items;
    }

    void clear(Node* root) {
        if (!root) {
            return;
        }
        if (root->left_) {
            clear(root->left_);
        }
        if (root->right_) {
            clear(root->right_);
        }
        delete root;
    }

    ~BTree() {
        clear(root);
        this->root = nullptr;
    }

   private:
    Node* root;
};

int main() {
    BTree tree;
    Node*& tree_root = tree.get_root();

    int tree_item = 0;
    std::string input_str;
    std::getline(std::cin, input_str);
    std::stringstream input(input_str);

    while (input >> tree_item) {
        tree.add(tree_root, tree_item);
    }

    std::vector<int> items = tree.get_items();

    for (const int item : items) {
        if (item != items[0]) {
            std::cout << 0 << std::endl;
            return 0;
        }
    }

    std::cout << 1 << std::endl;
}

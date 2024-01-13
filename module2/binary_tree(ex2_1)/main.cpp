/*Дано число N < 106 и последовательность целых чисел из [-231..231] длиной N.
Требуется построить бинарное дерево, заданное наивным порядком вставки.
Т.е., при добавлении очередного числа K в дерево с корнем root, если root→Key ≤ K, то узел K добавляется в правое поддерево
root; иначе в левое поддерево root. Требования: Рекурсия запрещена. Решение должно поддерживать передачу функции сравнения
снаружи.

2_1. Выведите элементы в порядке in-order (слева направо).*/

#include <functional>
#include <iostream>
#include <stack>

struct Node {
    int data;
    Node* left;
    Node* right;

    explicit Node(int data) : data(data), left(nullptr), right(nullptr){};
};

struct DefaultComparator {
    bool operator()(const int& lhs, const int& rhs) { return lhs < rhs; }
};

void print(Node* node) { std::cout << node->data << " "; }

void erase(Node* node) { delete node; }

template <typename Comparator = DefaultComparator>
class BinaryTree {
   public:
    BinaryTree() : root(nullptr){};
    explicit BinaryTree(int data) : root(new Node(data)){};
    BinaryTree(const BinaryTree& other) = delete;
    BinaryTree(BinaryTree&& other) = delete;
    BinaryTree& operator=(const BinaryTree& other) = delete;
    BinaryTree& operator=(BinaryTree&& other) = delete;

    void add(int data) {
        if (!root) {
            root = new Node(data);
            return;
        }

        Node* curr = root;
        while (curr) {
            if (comp(curr->data, data)) {
                if (!curr->right) {
                    curr->right = new Node(data);
                    return;
                }
                curr = curr->right;
            } else {
                if (!curr->left) {
                    curr->left = new Node(data);
                    return;
                }
                curr = curr->left;
            }
        }
    }

    friend std::ostream& operator<<(std::ostream& stream, BinaryTree& tree) {
        tree.in_order(print);
        return stream;
    }

    ~BinaryTree() { in_order(erase); }

   private:
    Node* root;
    Comparator comp;

    void in_order(std::function<void(Node*)> processor) {
        std::stack<Node*> stack;
        Node* curr = root;

        while (!stack.empty() || curr) {
            while (curr) {
                stack.push(curr);
                curr = curr->left;
            }

            curr = stack.top();
            stack.pop();

            Node* tmp = curr;
            curr = curr->right;
            processor(tmp);
        }
    }
};

int main() {
    BinaryTree<> tree;

    unsigned int N;
    std::cin >> N;

    int item = 0;
    for (unsigned int i = 0; i != N; ++i) {
        std::cin >> item;
        tree.add(item);
    }

    std::cout << tree;

    return 0;
}
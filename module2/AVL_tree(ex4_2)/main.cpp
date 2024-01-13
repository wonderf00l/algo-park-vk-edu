#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
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
        unsigned int quantity;

        Node* left;
        Node* right;

        Node(const Key& key, const Value& value)
            : key(key), value(value), height(1), quantity(1), left(nullptr), right(nullptr) {}
    };

   public:
    AVLTree(Comparator comp = Comparator()) : root(nullptr), tree_size(0), comp(comp) {}

    AVLTree(const AVLTree&) = delete;
    AVLTree(AVLTree&&) = delete;
    AVLTree& operator=(const AVLTree&) = delete;
    AVLTree& operator=(AVLTree&&) = delete;

    ~AVLTree() {
        clear(root);
        root = nullptr;
    };

    Value* find(const Key& key) { return _find(root, key); }
    const Value* find(const Key& key) const { return find(key); }

    void insert(const Key& key, const Value& value) { root = _insert(root, key, value); }

    void erase(const Key& key) { root = _erase(root, key); }

    Value kth_stat(size_t k) const {
        assert(k < quantity(root));  // изначально, например, 100 элементов(размер корня)
        Node* curr = root;
        size_t node_id = quantity(curr->left);  // начальный id - "середина дерева", т.е. кол-во элементов левого потомка

        while (k != node_id) {
            if (k < node_id) {  //если нужный id меньше текущего -- берем левого потомка, считаем кол-во элементов ее правого
                                //потомка(сколько еще элементов больше новой текущей ноды), из текущего id вычитаем
                                //получившееся значение
                curr = curr->left;
                node_id -= quantity(curr->right) + 1;
            } else {  // если больше,то берем левого потомка текущей ноды, к имеющемуся размеру (текущий id, сколько
                      // элементов "левее" curr, прибавляем кол-во элементов левого потомка новый текущей ноды), тем самым
                      // получаем кол-во элементов, которые левее curr, то есть ее id в отсортированной последовательности
                curr = curr->right;
                node_id += quantity(curr->left) + 1;
            }
        }
        return curr->value;
    }

   private:
    void clear(Node* node) {
        if (!node) {
            return;
        }
        clear(node->left);
        clear(node->right);
        delete node;
    }

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
        return balance(node);  // прокидываем отбалансированную ноду на выходе из каждого рекурсивного вызова, тем самым в
                               // node->left/right будут записываться новые корни левого и правого поддеревьев
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
            Node* right = node->right;  // ищем мин элемент правого поддерева, ставим его на место  удаленного, паренту мин
                                        // элемента ставим правого потомка мин элемента

            delete node;

            if (!right) {
                return left;
            }

            std::pair<Node*, Node*> nodes = find_and_remove_min(right);

            Node* min_node = nodes.first;    // мин элемент
            min_node->right = nodes.second;  // правый потомок мин элемента
            min_node->left = left;

            return balance(min_node);
        }
        return balance(node);
    }

    std::pair<Node*, Node*> find_and_remove_min(Node* node) {
        if (!node->left) {
            return std::make_pair(node, node->right);  // в последнем вызове возвращаем мин элемент и его правого потомка
        }
        std::pair<Node*, Node*> nodes = find_and_remove_min(node->left);
        node->left = nodes.second;  // привязываем паренту мин элемента(происходит при возвращении из последнего рекурсивного
                                    // вызова) адрес правого потомка мин элемента
        return std::make_pair(
            nodes.first, balance(node));  // через все вызову прокидываем мин элемент и отбалансированную ноду, на
                                          // промежуточынх вызовах будет происходить балансировка и перепривязка указателей
    }

    Node* find_min(Node* node) {
        if (!node->left) {
            return node;
        }
        return find_min(node->left);
    }

    Node* remove_min(Node* node) {
        if (!node->left) {
            return node->right;  // возврат правого поддерева минимального элемента
        }
        node->left = remove_min(
            node->left);  // перепривязка указателя (затирание мин элемента) при выходе из последнего вызова функции
        return balance(node);  // далее при разматывании стека возвращаем сбадансированную ножу и присваиваем ее node->left
    }

    uint8_t height(Node* node) const { return node ? node->height : 0; }

    unsigned int quantity(Node* node) const { return node ? node->quantity : 0; }

    void fix_quantity(Node* node) { node->quantity = quantity(node->left) + quantity(node->right) + 1; }

    void fix_height(Node* node) { node->height = std::max(height(node->left), height(node->right)) + 1; }

    int bfactor(Node* node) const { return height(node->right) - height(node->left); }

    Node* rotate_left(Node* node) {
        Node* tmp = node;
        node = tmp->right;
        tmp->right = node->left;
        node->left = tmp;
        fix_height(tmp);
        fix_height(node);
        fix_quantity(tmp);
        fix_quantity(node);
        return node;
    }
    Node* rotate_right(Node* node) {
        Node* tmp = node;
        node = tmp->left;
        tmp->left = node->right;
        node->right = tmp;
        fix_height(tmp);
        fix_height(node);
        fix_quantity(tmp);
        fix_quantity(node);
        return node;
    }

    Node* balance(Node* node) {
        fix_height(node);
        fix_quantity(node);
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

int main() {
    AVLTree<int, int> tree;

    // for (int i = 0; i != 100; ++i) {
    //     tree.insert(i, i);
    // }
    // for (int i = 50; i != 100; ++i) {
    //     tree.erase(i);
    // }

    unsigned int operations = 0;

    std::cin >> operations;

    for (size_t i = 0; i != operations; ++i) {
        int item = 0;
        size_t kth_stat = 0;

        std::cin >> item >> kth_stat;
        if (item > 0) {
            tree.insert(item, item);
        } else {
            tree.erase(std::abs(item));
        }
        std::cout << tree.kth_stat(kth_stat) << std::endl;
    }

    return 0;
}
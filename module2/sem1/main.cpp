#include <iostream>
#include <string>

size_t str_hash(const std::string& data) {
    size_t hash = 0;
    for (char i : data)
        hash = hash * 13 + i;
    return hash;
}


static constexpr size_t BUCKETS_SIZES_LIST[] = {
    7, 17, 37, 73, 149, 251, 509, 1021, 2027, 5003, 10837
};

template< class Key > struct hash { // обобщенная декларация структуры(требуется специализация шаблона)
    size_t operator()(const Key& value) { // при вызове без создания объекта не будет ошибки компиляции
        return value; // но при создании объекта и конкретизации в main (т.е. hash<int, int> h;) без написанной конкретизации будет ошибка, т.к. компилятор не найдет какую-либо реализацию
    }
};

template<> struct hash<std::string> { // специализация шаблона (реализация для конкретного типа данных)
    size_t operator()(const std::string& value) {
        return str_hash(value);
    }
}; // ; обязательно, т.к. реализция структуры


template<typename Key, typename Value, typename Hash=hash<Key>>
class HashMap {
    struct Node {
        Node(const Key& k, const Value& v, Node* n) : key(k), value(v), next(n) {

        };

        Key key;
        Value value;

        Node* next;
    };

    static constexpr size_t MAX_LEN = 7;
public:
    HashMap() : buckets(nullptr), buckets_count(0), buckets_sizes_list_idx(0), items_count(0) {

    }
    ~HashMap() {
        for (int i = 0; i < buckets_count; ++i) {
            Node* cur = buckets[i];
            while(cur) {
                Node* tmp = cur;
                cur = cur->next;
                delete tmp;
            }
        }
        delete[] buckets;
    }

    HashMap(const HashMap&)=delete;
    HashMap(HashMap&&)=delete;

    HashMap& operator=(const HashMap&)=delete;
    HashMap& operator=(HashMap&&)=delete;


    Value* find(const Key& key) {
        if (!buckets) {
            return nullptr;
        }

        size_t idx = hasher(key) % buckets_count;
        Node* cur = buckets[idx];
        while(cur) {
            if (cur->key == key) {
                return &cur->value;
            }
            cur = cur->next;
        }
        return nullptr;
    }

    bool insert(const Key& key, const Value& value) {
        if (find(key)) {
            return false;
        }

        if (buckets_count * MAX_LEN >= items_count) {
            grow();
        }

        size_t idx = hasher(key) % buckets_count;
        buckets[idx] = new Node(key, value, buckets[idx]); // новые нод вставляются в начало списка(бакета)
        items_count++;
        return true;
    }

    bool erase(const Key& key) {
        if (!buckets) {
            return false;
        }
        size_t idx = hasher(key) % buckets_count;
        Node* cur = buckets[idx];
        Node* prev = nullptr;
        while(cur) {
            if (cur->key == key) {
                if (prev == nullptr) { // если первый элемент бакета - привязывем его prev к след элементу(nullptr)
                    buckets[idx] = cur->next; // это и будет новый элемент в массиве бакетов автоматически
                } else {
                    prev->next = cur->next;
                }
                items_count--;
                delete cur;
                return true;
            }
            prev = cur;
            cur = cur->next;
        }
        return false;
    }


private:
    void grow() {
        size_t new_buckets_count = BUCKETS_SIZES_LIST[buckets_sizes_list_idx++];
        Node** new_buckets = new Node*[new_buckets_count];
        for (int i = 0; i < new_buckets_count; ++i) {
            new_buckets[i] = nullptr;
        }
        for (int i = 0; i < buckets_count; ++i) {
            Node* cur = buckets[i];
            while(cur) {
                Node* tmp = cur;
                cur = cur->next;
                size_t new_idx = hasher(tmp->key) % new_buckets_count; // поулчаем новую позицию в новом bucket_ara для ноды
                tmp->next = new_buckets[new_idx]; // т.к. вставляем в начало списка, то след элементом для втсавляемого является то, что уже находится в бакете(nullptr, который проинициализировали заранее, или же др. нода)
                new_buckets[new_idx] = tmp;
            }
        }
        delete[] buckets;

        buckets = new_buckets;
        buckets_count = new_buckets_count;
    }

    Node** buckets;
    size_t buckets_count; // size of iniial arr(bucket arr)

    size_t buckets_sizes_list_idx; // sizes_arr id
    size_t items_count; // quantity of nodes in one bucket

    Hash hasher;


};


int main() {
    HashMap<std::string, int> hash;
    std::string key;
    char operation;
    while(std::cin >> operation >> key) {
        bool result = false;
        switch (operation) {
            case '+':
                result = hash.insert(key, 0);
                break;
            case '-':
                result = hash.erase(key);
                break;
            case '?':
                result = (hash.find(key) != nullptr);
                break;
            default:
                break;
        }
        if (result) {
            std::cout << "OK" << std::endl;
        } else {
            std::cout << "FAIL" << std::endl;
        }
    }
}
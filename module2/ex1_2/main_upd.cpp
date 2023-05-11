#include <cassert>
#include <iostream>
#include <string>
#include <vector>

/*Реализуйте структуру данных типа “множество строк” на основе динамической хеш-таблицы с открытой адресацией. Хранимые
строки непустые и состоят из строчных латинских букв. Хеш-функция строки должна быть реализована с помощью вычисления
значения многочлена методом Горнера. Начальный размер таблицы должен быть равным 8-ми. Перехеширование выполняйте при
добавлении элементов в случае, когда коэффициент заполнения таблицы достигает 3/4. Структура данных должна поддерживать
операции добавления строки в множество, удаления строки из множества и проверки принадлежности данной строки множеству.

Вариант 1. Для разрешения коллизий используйте квадратичное пробирование.
i-ая проба g(k, i)=g(k, i-1) + i (mod m). m - степень двойки.
Вариант 2. Для разрешения коллизий используйте двойное хеширование.*/

template <typename Key>
struct DefaultComparator {  // по умолчанию поля и методы класса private!
    bool operator()(const Key& lhs, const Key& rhs) const { return lhs == rhs; }
};

template <class Key>
class Hasher {};

template <>
class Hasher<std::string> {
   public:
    size_t operator()(const std::string& value, size_t m) const { return str_hash(value, m); }

   private:
    size_t str_hash(const std::string& str, size_t m) const {
        size_t hash = 0;
        int a = m - 1;  // a и m: взамно-простые числа
        for (char ch : str) {
            hash = (hash * a + ch) % m;
        }
        return hash;
    }
};

template <class Key>
class ProbHasher {};

template <>
class ProbHasher<std::string> {
   public:
    size_t operator()(int hash) const { return str_hash(hash); }

   private:
    // size_t str_hash(const std::string& str, size_t m) const {
    //     size_t hash = 0;
    //     int a = m - 1;
    //     for (char ch : str) {
    //         hash = (hash * a + ch) % m;
    //     }
    //     if (!(hash % 2)) {  // h2(k) должна возвращать нечетные значения
    //         hash += 1;
    //     }
    //     return hash;
    // }
    size_t str_hash(int hash) const { return !(hash % 2) ? hash + 1 : hash; }
};

template <typename Key, typename Hasher = Hasher<Key>, typename ProbHasher = ProbHasher<Key>,
          typename Comparator = DefaultComparator<Key>>
class HashTable {
    enum class states { EMPTY, DELETED, FILLED };

    struct Bucket {
        Key key;
        states status;
        Bucket() : status(states::EMPTY) {}
        Bucket(const Key& key, const states& stat) : key(key), status(stat) {}
        friend std::ostream& operator<<(std::ostream& stream, const Bucket& bucket) {
            stream << bucket.key;
            return stream;
        }
    };

   public:
    HashTable() : buckets_size(8), buckets_quantity(0) { buckets.resize(buckets_size); }
    HashTable(const HashTable& other)
        : buckets(other.buckets), buckets_size(other.buckets_size), buckets_quantity(other.buckets_quantity) {}
    HashTable& operator=(const HashTable& other) {
        buckets = other.buckets;
        buckets_size = other.buckets_size;
        buckets_quantity = other.buckets_quantity;
        return *this;
    }

    HashTable(HashTable&& other)
        : buckets(std::move(other.buckets)), buckets_size(other.buckets_size), buckets_quantity(other.buckets_quantity) {}
    HashTable& operator=(HashTable&& other) {
        buckets = std::move(other.buckets);
        buckets_size = other.buckets_size;
        buckets_quantity = other.buckets_quantity;
        return *this;
    }

    ~HashTable() = default;

    size_t size() const { return buckets_quantity; }

    bool operator[](const Key& key) const {
        if (!buckets.size()) {
            return false;
        }
        size_t id = hasher(key, buckets_size), hash2 = prob_hasher(id), i = 1, step_counter = 0;
        while ((!comp(buckets[id].key, key) ||  // !comp(buckets[id].key, Key())
                buckets[id].status != states::FILLED) &&
               step_counter <= buckets_size) {  // // check collision и учесть ситуацию, когда нужно добраться до
                                                // элемента после пласта удаленных
            id = prob_step(id, hash2, i++);
            ++step_counter;
        }
        return step_counter <= buckets_quantity ? true : false;
    }

    bool add(const Key& key) {
        // if (operator[](key)) {
        //     return false;
        // }
        if (4 * buckets_quantity >=
            buckets_size * 3) {  // 4 * buckets_quantity >= buckets_size * 3   static_cast<float>(buckets_quantity) >=
                                 // load_factor * static_cast<float>(buckets_size)
            rehash();
        }
        size_t id = hasher(key, buckets_size), hash2 = prob_hasher(id), i = 1;
        while (buckets[id].status == states::FILLED) {  // check collision
            if (buckets[id].key == key) {
                return false;
            }
            id = prob_step(id, hash2, i++);
            // std::cout << key << " " << id << std::endl;
        }
        // std::cout << "after loop" << key << " " << id << std::endl;
        buckets[id].key = key;
        buckets[id].status = states::FILLED;
        ++buckets_quantity;
        // std::cout << buckets[id].key;
        return true;
    }

    bool erase(const Key& key) {
        // if (!operator[](key)) {
        //     return false;
        // }
        size_t del_id = hasher(key, buckets_size), hash2 = prob_hasher(del_id), i = 1, step_counter = 0;
        while ((!comp(buckets[del_id].key,
                      key) ||  // !comp(buckets[del_id].key, Key())
                buckets[del_id].status == states::DELETED) &&
               step_counter <= buckets_size) {
            del_id = prob_step(del_id, hash2, i++);
            ++step_counter;
        }
        if (step_counter >= buckets_size) {
            return false;
        }
        buckets[del_id].status = states::DELETED;
        buckets[del_id].key = Key();
        --buckets_quantity;
        return true;
    }

    friend std::ostream& operator<<(std::ostream& stream, const HashTable& table) {
        for (size_t i = 0; i != table.buckets_size; ++i) {
            if (table.buckets[i].status == states::FILLED) {
                (i != table.buckets_size - 1) ? stream << table.buckets[i] << " " : stream << table.buckets[i] << std::endl;
            }
        }
        return stream;
    }

    // void foo() const {
    //     for (const Bucket& buck : buckets) { // если функция с пометкой const, то в range-for не может быть не const
    //     ссылок
    //         if (buck.status == states::FILLED) {
    //             std::cout << buck << "|";
    //         }
    //     }
    // }

   private:
    // static constexpr float load_factor = 0.75;

    void rehash() {
        buckets_size *= 2;
        std::vector<Bucket> new_buckets(buckets_size);  // не забываем инициализировать значения вектора
        // std::vector<Bucket> new_buckets;
        // new_buckets.resize(buckets_size);
        for (Bucket& bucket : buckets) {
            if (bucket.status == states::FILLED) {
                size_t new_id = hasher(bucket.key, buckets_size), hash2 = prob_hasher(new_id), i = 1;
                while (new_buckets[new_id].status == states::FILLED) {
                    new_id = prob_step(new_id, hash2, i++);
                }
                new_buckets[new_id] = std::move(bucket);
            }
        }
        buckets = std::move(new_buckets);
    }

    size_t prob_step(int hash1, int hash2, size_t i) const {
        return (hash1 + i * hash2) % buckets_size;
    }  // ... as 'this' discard qualifiers - should add const if func doesn't change anything

    Hasher hasher;
    ProbHasher prob_hasher;
    Comparator comp;
    std::vector<Bucket> buckets;
    size_t buckets_size;
    size_t buckets_quantity;
};

int main() {
    HashTable<std::string> table;
    std::string key;
    char operation;
    while (std::cin >> operation >> key) {
        bool result = false;
        switch (operation) {
            case '+':
                result = table.add(key);
                break;
            case '-':
                result = table.erase(key);
                break;
            case '?':
                result = (table[key]);
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
    // std::string str = "hello";
    // table.add(str);
    // table.add(std::string{"world"});
    // table.add(std::string{"hello"});
    // table.add(std::string{"buddy guy"});
    // table.erase(str);
    // table.erase(std::string{"kalabaka abaka"});
    // table.add("123");
    // table.add("567");
    // table.add("890");
    // table.add("10 11 12");
    // for (int i = 0; i != 100; ++i) {
    //     table.add(std::to_string(i));
    // }
    // for (int i = 0; i != 100; ++i) {
    //     std::cout << table[std::to_string(i)] << std::endl;
    // }
    // std::string str_;
    // while (std::cin >> str_) {
    //     table.add(str_);
    // }
    // std::cout << table["buddy guy"] << std::endl;
    // std::cout << table["pump"] << std::endl;
    // std::cout << table.erase("doens't exist") << std::endl;
    // std::cout << table.erase("123") << std::endl;
    // std::cout << table.erase("123") << std::endl;
    // std::cout << table.erase("world") << std::endl;
    // std::cout << table["world"] << std::endl;
    // std::cout << table.add("10 11 12") << std::endl;
    // std::cout << table.add("qwertt") << std::endl;
    // table.add("12345");
    // // table.foo();
    // // check rehash
    // std::cout << table;

    return 0;
}
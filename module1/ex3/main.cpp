#include <cassert>
#include <iostream>

class Queue {
   private:
    int* buf;
    int size, capacity;

    int head, tail;

    bool _resize() {
        bool resized = 0;
        if (size == capacity) {
            capacity *= 2;
            int* new_buf = new int[capacity];
            _copy(new_buf, buf, size);
            delete[] buf;
            buf = new_buf;
            resized = 1;
        }
        return resized;
    }

    void _copy(int* dest, const int* src, int size) {
        for (int i = 0; i != size; ++i) {
            dest[i] = src[i];
        }
    }

   public:
    Queue(int size = 0) : size(size), head(0), tail(0) {
        if (size) {
            capacity = size * 2;
        } else {
            capacity = 1;
        }

        buf = new int[capacity];
    }

    Queue(const Queue& other)
        : size(other.size), capacity(other.capacity), head(other.head), tail(other.tail) {
        buf = new int[capacity];
        _copy(buf, other.buf, size);
    };

    Queue& operator=(const Queue& other) {
        size = other.size;
        capacity = other.capacity;
        head = other.head;
        tail = other.tail;
        delete[] buf;
        buf = new int[capacity];
        _copy(buf, other.buf, size);

        return *this;
    }

    void push_back(int item) {
        if (head) {
            if (tail >= size) {
                tail = 0;
                buf[tail] = item;
                ++size;
                tail = (tail + 1) % size;
            } else {
                assert(tail % size != head);
                buf[tail] = item;
                ++size;
                tail = (tail + 1) % size;
            }
        } else {
            _resize();
            buf[tail++] = item;
            ++size;
        }
    }

    int pop_front() {
        assert(size);
        int item = buf[head];
        head = (head + 1) % capacity;
        --size;
        return item;
    }

    bool is_empty() const { return size == 0; }

    void print() const {
        for (int i = 0; i != size; ++i) {
            i != size - 1 ? std::cout << buf[i] << ' ' : std::cout << buf[i] << std::endl;
        }
    }

    ~Queue() { delete[] buf; }
};

int run(std::istream& input, std::ostream& output) {
    Queue queue;
    int commands_count = 0;
    input >> commands_count;

    for (int i = 0; i < commands_count; i++) {
        int command = 0;
        int expected = 0;
        input >> command >> expected;
        switch (command) {
            case 2:  // pop
            {
                int value = queue.is_empty() ? -1 : queue.pop_front();
                if (value != expected) {
                    output << "NO" << std::endl;
                    return 0;
                }
            } break;
            case 3:  // push
                queue.push_back(expected);
                break;
            default:
                output << "NO" << std::endl;
                return 0;
        }
    }
    output << "YES" << std::endl;

    return 0;
}

int main() { run(std::cin, std::cout); }
#ifndef VSB_AGU_PRIORITYQUEUE_H
#define VSB_AGU_PRIORITYQUEUE_H

#include <set>
#include <iostream>

template <typename T>
class PriorityQueue {
private:
    std::set<T> queue;

public:
    void insert(const T &t) {
        queue.insert(t);
    }

    T top() {
        return *queue.begin();
    }

    T pop() {
        T t = this->top();
        queue.erase(queue.begin());

        return t;
    }

    bool contains(const T &t) {
        return queue.find(t) != queue.end();
    }

    bool isEmpty() {
        return queue.empty();
    }

    void erase(const T &t) {
        auto tf = queue.find(t);

        if (tf != queue.end()) {
            queue.erase(tf);
        }
    }
};

#endif //VSB_AGU_PRIORITYQUEUE_H

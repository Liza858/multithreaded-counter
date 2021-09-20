#pragma once

#include <iostream>
#include <random>
#include <atomic>

template<class T>
T get_random_number(T from, T to) {
    std::random_device random_device;
    std::mt19937 gen(random_device());
    std::uniform_int_distribution<T> uniform_distribution(from, to);
    return uniform_distribution(gen);
}

class Counter {
private:
    std::atomic_int *counter_array;
    size_t capacity;

public:
    explicit Counter(size_t size) {
        capacity = size;
        counter_array = new std::atomic_int[size];
        for (size_t i = 0; i < size; i++) {
            counter_array[i] = 0;
        }
    }

    std::atomic_int *get_counter_array() const {
        return counter_array;
    }

    size_t get_capacity() const {
        return capacity;
    }

    ~Counter() {
        delete[] counter_array;
    }
};

void inc(Counter *c) {
    size_t counter_capacity = c->get_capacity();
    std::atomic_int *counter_array = c->get_counter_array();
    auto random_num = get_random_number<size_t>(0, counter_capacity - 1);
    counter_array[random_num].fetch_add(1);
}

int get(Counter *c) {
    int sum = 0;
    for (size_t i = 0; i < c->get_capacity(); i++) {
        std::atomic_int *counter_array = c->get_counter_array();
        sum += counter_array[i].load();
    }
    return sum;
}
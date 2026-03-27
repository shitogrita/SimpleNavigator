#pragma once

#include <utility>
#include <initializer_list>
#include <ostream>
#include "stack.h"

namespace s21 {
    template <typename T>
    class Queue {
        using value_type = T;
        using reference = T&;
        using const_reference = const T&;
        using size_type = size_t;
        using pointer = T*;

        size_type _size = 0;
        Stack<T> _front;
        Stack<T> _back;

      public:
        Queue() = default;
        Queue(std::initializer_list<T> const &items);
        Queue(const Queue &q);
        Queue(Queue &&q);
        ~Queue();
        Queue& operator=(Queue &&q);
        Queue& operator=(const Queue &q);

        const_reference front() const;
        const_reference back() const;
        reference back();
        reference front();

        bool empty();
        size_type size();

        void push(const_reference value);
        void pop();
        void swap(Queue& other);

        template <typename U>
        friend std::ostream& operator<<(std::ostream& os, const Queue<U>& queue);

      private:
        void flush(bool to_back = true);
    };
};

template <typename T>
void s21::Queue<T>::flush(bool to_back) {
    if (to_back) {
        while (!_front.empty()) {
            _back.push(_front.top());
            _front.pop();
        }
    } else {
        while (!_back.empty()) {
            _front.push(_back.top());
            _back.pop();
        }
    }
}

template <typename T>
s21::Queue<T>::Queue(std::initializer_list<T> const &items) {
    for (const T& elem : items) {
        _back.push(elem);  //  кладём в _back, не в _front
        ++_size;
    }
    flush(false);
}

template <typename T>
s21::Queue<T>::Queue(const Queue &q) : _size(q._size), _front(q._front), _back(q._back) {}

template <typename T>
s21::Queue<T>::Queue(Queue &&q) {
    *this = std::move(q);
}

template <typename T>
s21::Queue<T>::~Queue() {
    flush();
    while (!_back.empty()) {
        _back.pop();
    }
}

template <typename T>
typename s21::Queue<T>& s21::Queue<T>::operator=(const Queue &q) {
    if (this != &q) {
        _front = q._front;
        _back = q._back;
        _size = q._size;
    }
    return *this;
}

template <typename T>
typename s21::Queue<T>& s21::Queue<T>::operator=(Queue &&q) {
    if (this != &q) {
        _front = std::move(q._front);
        _back = std::move(q._back);
        _size = std::move(q._size);
        q._size = 0;
    }
    return *this;
}



template <typename T>
typename s21::Queue<T>::const_reference s21::Queue<T>::front() const {
    if (empty()) throw std::runtime_error("Calling front() on empty queue");
    if (_front.empty()) flush(false);  
    return _front.top(); 
}

template <typename T>
typename s21::Queue<T>::reference s21::Queue<T>::front() {
    if (empty()) throw std::runtime_error("Calling front() on empty queue");
    if (_front.empty()) flush(false);  
    return _front.top(); 
}

template <typename T>
typename s21::Queue<T>::const_reference s21::Queue<T>::back() const {
    if (empty()) throw std::runtime_error("Calling back() on empty queue");
    if (_back.empty()) flush(true);
    return _back.top();
}

template <typename T>
typename s21::Queue<T>::reference s21::Queue<T>::back() {
    if (empty()) throw std::runtime_error("Calling back() on empty queue");
    if (_back.empty()) flush(true);
    return _back.top();
}


template <typename T>
bool s21::Queue<T>::empty() {
    return _size == 0;
}

template <typename T>
typename s21::Queue<T>::size_type s21::Queue<T>::size() {
    return _size;
}

template <typename T>
void s21::Queue<T>::push(const_reference value) {
    _front.push(value);
    ++_size;
}

template <typename T>
void s21::Queue<T>::pop() {
    if (_size == 0) throw std::runtime_error("Queue is empty");
    if (_back.empty()) flush(true);
    _back.pop();
    --_size;
}

template <typename T>
void s21::Queue<T>::swap(Queue& other) {
    std::swap(_front, other._front);
    std::swap(_back, other._back);
    std::swap(_size, other._size);
}

namespace s21 {
    template <typename T>
    std::ostream& operator<<(std::ostream& os, const s21::Queue<T>& queue) {
        auto tmp = queue;
        while (!tmp.empty()) {
            os << tmp.front() << ' ';
            tmp.pop();
        }
        return os;
    }
};
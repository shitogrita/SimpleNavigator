#pragma once

#include <utility> 
#include <initializer_list>
#include <iostream> 

namespace s21 {

    template <typename T>
    struct Node {
        T val;
        Node* prev;
    }; 

    template <typename T>
    class Stack{
    
        using value_type = T;
        using reference = T&;
        using const_reference = const T&;  
        using size_type = size_t;
    
        Node<T>* _top = nullptr;
        size_type _size = 0;
    
      public:
        Stack() = default;
        Stack(std::initializer_list<T> const &items);
        Stack(const Stack &s);
        Stack(Stack &&s);
        ~Stack();
        Stack& operator=(Stack &&s);
        Stack& operator=(const Stack& s);
    
        reference top();
        const_reference top() const;
        bool empty();
        size_type size();

        void pop();
        void push(const_reference value);
        void swap(Stack& other);

        template <typename U>
        friend std::ostream& operator<<(std::ostream& os, const Stack<U>& stack);

        template <typename... Args>
        void insert_many_top(Args&&... args);
    }; 
};

template <typename T> 
s21::Stack<T>::Stack(std::initializer_list<T> const &items) { 
    for (const T& elem : items) {
        Node<T>* new_node = new Node<T>();
        new_node->val = elem;
        new_node->prev =_top;
        _top = new_node;
    }
}

template <typename T> 
s21::Stack<T>::Stack(const s21::Stack<T>& s) {
    if (s._top == nullptr) return;
    s21::Stack<T> temp;
    typename s21::Node<T>* current = s._top;
    while (current != nullptr) {
        temp.push(current->val);
        current = current->prev;
    }

    while (!temp.empty()) {
        push(temp.top());
        temp.pop();
    }
}

template <typename T> 
s21::Stack<T>::~Stack() {
    while (_top != nullptr) {
        auto temp = _top;
        _top = _top->prev;
        delete temp;
    }
}

template <typename T> 
s21::Stack<T>::Stack(s21::Stack<T> &&s){
    _size = std::move(s._size);
    _top = std::move(s._top);

    s._size = 0;
    s._top = nullptr;
}

template <typename T> 
s21::Stack<T>& s21::Stack<T>::operator=(s21::Stack<T> &&s) {
    if (this != &s) {
        _size = std::move(s._size);
        _top = std::move(s._top);

        s._top = nullptr;
        s._size = 0;
    }
    return *this;
}

template <typename T> 
typename s21::Stack<T>& s21::Stack<T>::operator=(const s21::Stack<T> &s) {
    if (this != &s) {

        while (!empty()) {
            pop();
        }
        
        s21::Stack<T> temp;
        typename s21::Node<T>* current = s._top;
        while (current != nullptr) {
            temp.push(current->val);
            current = current->prev;
        }
    
        while (!temp.empty()) {
            push(temp.top());
            temp.pop();
        }
    }
    return *this;
}

template <typename T> 
bool s21::Stack<T>::empty() {
    return _size == 0;
}

template <typename T> 
typename s21::Stack<T>::size_type s21::Stack<T>::size() { 
    return _size;
}

template <typename T> 
typename s21::Stack<T>::const_reference s21::Stack<T>::top() const {
    return _top->val;
}

template <typename T>
typename s21::Stack<T>::reference s21::Stack<T>::top() {
    return _top->val;
}

template <typename T>
void s21::Stack<T>::push(const_reference value) {
    Node<T>* new_node = new Node<T>();
    new_node->val = value;
    new_node->prev = _top;
    _top = new_node;
    ++_size;
}

template <typename T>
void s21::Stack<T>::pop() {
    if (_size != 0) {
        auto tmp = _top;
        _top = _top->prev;
        --_size;
        delete tmp;
    }
}

template <typename T>
void s21::Stack<T>::swap(s21::Stack<T>& other) {
    std::swap(_top, other._top);
    std::swap(_size, other._size);
}

namespace s21 { //remove
    template <typename T> 
    std::ostream& operator<<(std::ostream& os, const s21::Stack<T>& stack) {
        auto node = stack._top;
        while (node != nullptr) {
            os << node->val << ' ';
            node = node->prev;
        }
        return os;
    }
};

template <typename T>
template <typename... Args>
void s21::Stack<T>::insert_many_top(Args&&... args) {
    (push(std::forward<Args>(args)), ...);  
}
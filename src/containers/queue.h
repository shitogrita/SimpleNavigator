#pragma once

#include <initializer_list>
#include <ostream>
#include <stdexcept>
#include <utility>

#include "stack.h"

namespace s21 {

template <typename T>
class Queue {
 public:
  using value_type = T;
  using reference = T&;
  using const_reference = const T&;
  using size_type = size_t;

  Queue() = default;
  Queue(std::initializer_list<T> const& items);
  Queue(const Queue& q) = default;
  Queue(Queue&& q) = default;
  ~Queue() = default;

  Queue& operator=(const Queue& q) = default;
  Queue& operator=(Queue&& q) = default;

  reference front();
  const_reference front() const;

  reference back();
  const_reference back() const;

  bool empty() const;
  size_type size() const;

  void push(const_reference value);
  void pop();
  void swap(Queue& other);

 private:
  void MoveToPopStack();
  void MoveToPushStack() const;

  size_type size_ = 0;
  Stack<T> push_stack_;
  Stack<T> pop_stack_;
};

template <typename T>
Queue<T>::Queue(std::initializer_list<T> const& items) {
  for (const T& elem : items) {
    push(elem);
  }
}

template <typename T>
void Queue<T>::MoveToPopStack() {
  if (pop_stack_.empty()) {
    while (!push_stack_.empty()) {
      pop_stack_.push(push_stack_.top());
      push_stack_.pop();
    }
  }
}

template <typename T>
typename Queue<T>::reference Queue<T>::front() {
  if (empty()) {
    throw std::runtime_error("Queue is empty");
  }
  MoveToPopStack();
  return pop_stack_.top();
}

template <typename T>
typename Queue<T>::const_reference Queue<T>::front() const {
  if (empty()) {
    throw std::runtime_error("Queue is empty");
  }
  return const_cast<Queue*>(this)->front();
}

template <typename T>
typename Queue<T>::reference Queue<T>::back() {
  if (empty()) {
    throw std::runtime_error("Queue is empty");
  }

  if (!push_stack_.empty()) {
    return push_stack_.top();
  }

  while (!pop_stack_.empty()) {
    push_stack_.push(pop_stack_.top());
    pop_stack_.pop();
  }

  return push_stack_.top();
}

template <typename T>
typename Queue<T>::const_reference Queue<T>::back() const {
  if (empty()) {
    throw std::runtime_error("Queue is empty");
  }
  return const_cast<Queue*>(this)->back();
}

template <typename T>
bool Queue<T>::empty() const {
  return size_ == 0;
}

template <typename T>
typename Queue<T>::size_type Queue<T>::size() const {
  return size_;
}

template <typename T>
void Queue<T>::push(const_reference value) {
  push_stack_.push(value);
  ++size_;
}

template <typename T>
void Queue<T>::pop() {
  if (empty()) {
    throw std::runtime_error("Queue is empty");
  }

  MoveToPopStack();
  pop_stack_.pop();
  --size_;
}

template <typename T>
void Queue<T>::swap(Queue& other) {
  std::swap(push_stack_, other.push_stack_);
  std::swap(pop_stack_, other.pop_stack_);
  std::swap(size_, other.size_);
}

}
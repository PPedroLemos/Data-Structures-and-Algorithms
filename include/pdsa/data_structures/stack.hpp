#ifndef STACK_HPP
#define STACK_HPP

#include "./vector.hpp"

namespace pdsa
{

template <typename T, typename Container = pdsa::vector<T>> class Stack
{
  private:
    Container container;

  public:
    Stack() : container() {}

    std::size_t size() const { return container.size(); }
    std::size_t capacity() const { return container.capacity(); }
    bool empty() const { return container.empty(); }

    void push(const T& value) { container.push_back(value); }
    T& top() { container.back(); }
    T& pop() { container.pop_back(); }
};

} // namespace pdsa

#endif

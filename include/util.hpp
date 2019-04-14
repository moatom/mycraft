#ifndef __UTIL_HPP__
#define __UTIL_HPP__

#include <array>

template<typename T, int n>
void printArray(std::array<T, n> array)
{
  for (auto e: array) {// assume that e is of primitive data type
    std::cout << e << std::endl;
  }
};

#endif
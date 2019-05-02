#ifndef __UTIL_HPP__
#define __UTIL_HPP__

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#define M_SIZE(type, member) sizeof(((type *)0)->member)

void test(const char *a)
{
  std::cout << a << std::endl;
}

void v3Print(const char *title, glm::vec3 vec)
{
  std::cout << title << ": "
            << vec.x << ", "
            << vec.y << ", "
            << vec.z << std::endl;
}

void m3Print(const char *title, glm::mat3 mat)
{
  std::cout << title << ":\n"
            << glm::to_string(mat)
            << std::endl;
}

void nPrint()
{
  std::cout << std::endl;
}

#endif
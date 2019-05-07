#include <array>
#include <util.hpp>

void test(const char* a)
{
  std::cout << a << std::endl;
}

void v3Print(const char* title, glm::vec3 vec)
{
  std::cout << title << ": "
            << vec.x << ", "
            << vec.y << ", "
            << vec.z << std::endl;
}

void m3Print(const char* title, glm::mat3 mat)
{
  std::cout << title << ":\n"
            << glm::to_string(mat)
            << std::endl;
}

void arrayPrint(std::array<glm::vec3, 27> array)
{
  for (const auto& e: array) {
    v3Print("", e);
  }
}

void nPrint()
{
  std::cout << std::endl;
}

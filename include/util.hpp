#ifndef __UTIL_HPP__
#define __UTIL_HPP__

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#define M_SIZE(type, member) sizeof(((type *)0)->member)


void test(const char*);
void v3Print(const char*, glm::vec3);
void m3Print(const char*, glm::mat3);
void arrayPrint(std::array<glm::vec3, 27>);
void nPrint();

#endif
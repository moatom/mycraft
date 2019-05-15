#ifndef INCLUDED_BLOCK_STRUCT_HPP
#define INCLUDED_BLOCK_STRUCT_HPP

#include <glm/glm.hpp>

typedef struct{
  unsigned char* body;
  int width, height;
} Image;

typedef struct {
  glm::vec3 position;
  glm::vec4 color;
  glm::vec2 texcoord;
} Vertex;

#endif
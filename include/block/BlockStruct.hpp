#ifndef __BLOCK_STRUCT_HPP__
#define __BLOCK_STRUCT_HPP__

#include <glm/glm.hpp>

typedef struct{
  unsigned char *body;
  int width, height;
} Image;

typedef struct {
  glm::vec3 position;
  glm::vec4 color;
  glm::vec2 texcoord;
} Vertex;

#endif
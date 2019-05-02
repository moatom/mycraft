#ifndef __MODEL_HPP__
#define __MODEL_HPP__

#include <GL/glew.h>
#include <array>

#include "BlockStruct.hpp"


class Model {
  std::array<Vertex, 4*6> Cube;
  std::array<GLuint, 6*6> CubeIndex;

  public:
    Model():
    Cube{{
      // Top
      {{-1,1,1},    {1.0f,1.0f,1.0f,1.0f},  {0.0f, 0.0f}},
      {{1,1,1},     {1.0f,1.0f,1.0f,1.0f},  {0.25f, 0.0f}},
      {{1,1,-1},    {1.0f,1.0f,1.0f,1.0f},  {0.25f, 1.0f}},
      {{-1,1,-1},   {1.0f,1.0f,1.0f,1.0f},  {0.0f, 1.0f}},
      // Bottom
      {{1,-1,1},    {1.0f,1.0f,1.0f,1.0f},  {0.55f, 0.0f}},
      {{1,-1,-1},   {1.0f,1.0f,1.0f,1.0f},  {0.75f, 0.0f}},
      {{-1,-1,-1},  {1.0f,1.0f,1.0f,1.0f},  {0.75f, 1.0f}},
      {{-1,-1,1},   {1.0f,1.0f,1.0f,1.0f},  {0.55f, 1.0f}},
      // Front
      {{-1,-1,1},   {1.0f,1.0f,1.0f,1.0f},  {0.255f, 0.0f}},
      {{1,-1,1},    {1.0f,1.0f,1.0f,1.0f},  {0.5f, 0.0f}},
      {{1,1,1},     {1.0f,1.0f,1.0f,1.0f},  {0.5f, 1.0f}},
      {{-1,1,1},    {1.0f,1.0f,1.0f,1.0f},  {0.255f, 1.0f}},
      // Back
      {{1,-1,-1},   {1.0f,1.0f,1.0f,1.0f},  {0.255f, 0.0f}},
      {{-1,-1,-1},  {1.0f,1.0f,1.0f,1.0f},  {0.5f, 0.0f}},
      {{-1,1,-1},   {1.0f,1.0f,1.0f,1.0f},  {0.5f, 1.0f}},
      {{1,1,-1},    {1.0f,1.0f,1.0f,1.0f},  {0.255f, 1.0f}},
      // Right
      {{1,-1,1},    {1.0f,1.0f,1.0f,1.0f},  {0.255f, 0.0f}},
      {{1,-1,-1},   {1.0f,1.0f,1.0f,1.0f},  {0.5f, 0.0f}},
      {{1,1,-1},    {1.0f,1.0f,1.0f,1.0f},  {0.5f, 1.0f}},
      {{1,1,1},     {1.0f,1.0f,1.0f,1.0f},  {0.255f, 1.0f}},
      // Left
      {{-1,-1,-1},  {1.0f,1.0f,1.0f,1.0f},  {0.255f, 0.0f}},
      {{-1,-1,1},   {1.0f,1.0f,1.0f,1.0f},  {0.5f, 0.0f}},
      {{-1,1,1},    {1.0f,1.0f,1.0f,1.0f},  {0.5f, 1.0f}},
      {{-1,1,-1},   {1.0f,1.0f,1.0f,1.0f},  {0.255f, 1.0f}},
    }},
    CubeIndex{{
      // Top 0-3
      0,1,2, 2,3,0,
      // Bottom 4-7
      4,5,6, 6,7,4,
      // Front 8-11
      8,9,10, 10,11,8,
      // Back 12-15
      12,13,14, 14,15,12,
      // Right 16-19
      16,17,18, 18,19,16,
      // Left 20-23
      20,21,22, 22,23,20
    }}
    {}

    std::array<Vertex, 4*6> &getCube() {return Cube;}
    std::array<GLuint, 6*6> &getCubeIndex() {return CubeIndex;}
};
#endif